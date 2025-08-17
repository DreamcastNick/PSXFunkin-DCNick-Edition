/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "str.h"
#include "psx.h"
#include "gfx.h"
#include "psn00b/strnoob.h"
#include "mem.h"
#include "timer.h"
#include "stage.h"

/* CD and MDEC interrupt handlers */
#define BLOCK_SIZE 16

#define VRAM_X_COORD(x) ((x) / 1)

// Offscreen VRAM location to upload movie frames as a 16bpp texture page
// 16bpp tpages are 256px wide; choose an x aligned to 256px boundaries
// to make UVs start at 0 inside the page when drawing.
#define MOVIE_VRAM_X 448
#define MOVIE_VRAM_Y 256

// All non-audio sectors in .STR files begin with this 32-byte header, which
// contains metadata about the sector and is followed by a chunk of frame
// bitstream data.
// https://problemkaputt.de/psx-spx.htm#cdromfilevideostrstreamingandbspicturecompressionsony
typedef struct 
{
	u16 magic;			// Always 0x0160
	u16 type;			// 0x8001 for MDEC
	u16 sector_id;		// Chunk number (0 = first chunk of this frame)
	u16 sector_count;	// Total number of chunks for this frame
	u32 frame_id;		// Frame number
	u32 bs_length;		// Total length of this frame in bytes

	u16 width, height;
	u8  bs_header[8];
	u32 _reserved;
} STR_Header;

typedef struct 
{
	u16 width, height;
	u32 bs_data[0x2000];	// Bitstream data read from the disc
	u32 mdec_data[0x8000];	// Decompressed data to be fed to the MDEC
} StreamBuffer;

typedef struct 
{
	StreamBuffer frames[2];
	u32     slices[2][BLOCK_SIZE * SCREEN_HEIGHT2];

	int  frame_id, sector_count;
	int  dropped_frames;
	RECT slice_pos;
	int  frame_width;
	int  frame_height;
	RECT page_frame_rect[2];
	u8   page_frame_valid[2];
	int  slices_remaining;
	StreamBuffer* last_frame; // last fully prepared frame for duplication
	u8   decoding_busy;
	int  decode_dst_x;
	int  decode_dst_y;
	int  decode_frame_width;
	int  decode_frame_height;
	u8   current_page_index;
	int  frame_right_x;
	u8   decoding_page_index;

	volatile s8 sector_pending, frame_ready;
	volatile s8 cur_frame, cur_slice;

	// Texture handle for drawing the movie as a quad
	Gfx_Tex tex;
} StreamContext;

typedef struct
{
	const char* name;
	StageId id;
} STR_Def;

static StreamContext* str_ctx;

// This buffer is used by cd_sector_handler() as a temporary area for sectors
// read from the CD. Due to DMA limitations it can't be allocated on the stack
// (especially not in the interrupt callbacks' stack, whose size is very
// limited).
static STR_Header* sector_header;

static const STR_Def str_def[] = {
	#include "strdef.h"
};

void cd_sector_handler(void) 
{
	StreamBuffer *frame = &str_ctx->frames[str_ctx->cur_frame];

	// Fetch the .STR header of the sector that has been read and make sure it
	// is valid. If not, assume the file has ended and set frame_ready as a
	// signal for the main loop to stop playback.
	CdGetSector(sector_header, sizeof(STR_Header) / 4);

	if (sector_header->magic != 0x0160) 
	{
		str_ctx->frame_ready = -1;
		return;
	}

	// Ignore any non-MDEC sectors that might be present in the stream.
	if (sector_header->type != 0x8001)
		return;

	// If this sector is actually part of a new frame, validate the sectors
	// that have been read so far and flip the bitstream data buffers. If the
	// frame number is actually lower than the current one, assume the drive
	// has started reading another .STR file and stop playback.
	if ((int) sector_header->frame_id < str_ctx->frame_id) 
	{
		str_ctx->frame_ready = -1;
		return;
	}

	if ((int) sector_header->frame_id > str_ctx->frame_id) 
	{
		// Do not set the ready flag if any sector has been missed.
		if (str_ctx->sector_count)
			str_ctx->dropped_frames++;
		else
			str_ctx->frame_ready = 1;

		str_ctx->frame_id     = sector_header->frame_id;
		str_ctx->sector_count = sector_header->sector_count;
		str_ctx->cur_frame   ^= 1;

		frame = &str_ctx->frames[str_ctx->cur_frame];

		// Initialize the next frame. Dimensions must be rounded up to the
		// nearest multiple of 16 as the MDEC operates on 16x16 pixel blocks.
		frame->width  = (sector_header->width  + 15) & 0xfff0;
		frame->height = (sector_header->height + 15) & 0xfff0;
	}

	// Append the payload contained in this sector to the current buffer.
	str_ctx->sector_count--;
	CdGetSector(
		&(frame->bs_data[2016 / 4 * sector_header->sector_id]),
		2016 / 4
	);
}

void mdec_dma_handler(void) 
{
	// Handle any sectors that were not processed by cd_event_handler() (see
	// below) while a DMA transfer from the MDEC was in progress. As the MDEC
	// has just finished decoding a slice, they can be safely handled now.
	if (str_ctx->sector_pending) 
	{
		cd_sector_handler();
		str_ctx->sector_pending = 0;
	}

	// Upload the decoded slice to VRAM and start decoding the next slice (into
	// another buffer) if any.
	LoadImage(&str_ctx->slice_pos, str_ctx->slices[str_ctx->cur_slice]);

	str_ctx->cur_slice   ^= 1;
	str_ctx->slice_pos.x += BLOCK_SIZE;
	if (str_ctx->slices_remaining > 0)
		str_ctx->slices_remaining--;

    if (str_ctx->slice_pos.x < str_ctx->frame_right_x)
		DecDCTout(
			str_ctx->slices[str_ctx->cur_slice],
			BLOCK_SIZE * str_ctx->slice_pos.h / 2
		);
	else
	{
		// Finished full frame upload to VRAM, mark this draw page as valid for duplication.
		str_ctx->page_frame_valid[str_ctx->decoding_page_index] = 1;
		str_ctx->decoding_busy = 0;
	}
}

void cd_event_handler(u8 event, u8 *payload) 
{
	// Ignore all events other than a sector being ready.
	if (event != CdlDataReady)
		return;

	// Only handle sectors immediately if the MDEC is not decoding a frame,
	// otherwise defer handling to mdec_dma_handler(). This is a workaround for
	// a hardware conflict between the DMA channels used for the CD drive and
	// MDEC output, which shall not run simultaneously.
	if (DecDCTinSync(1))
		str_ctx->sector_pending = 1;
	else
		cd_sector_handler();
}

StreamBuffer *get_next_frame(void) 
{
	// Non-blocking: return NULL if no new frame is ready.
	if (!str_ctx->frame_ready)
		return 0;

	if (str_ctx->frame_ready < 0)
	{
		return 0;
	}

	str_ctx->frame_ready = 0;
	return &str_ctx->frames[str_ctx->cur_frame ^ 1];
}

static void STR_InitStream(void)
{
	EnterCriticalSection();
	DecDCToutCallback(&mdec_dma_handler);
	CdReadyCallback(&cd_event_handler);
	ExitCriticalSection();

	// Copy the lookup table used for frame decompression to the scratchpad
	// area. This is optional but makes the decompressor slightly faster. See
	// the libpsxpress documentation for more details.
	DecDCTvlcCopyTableV3((VLC_TableV3*) 0x1f800000);

	stage.movie_is_playing = true;
	stage.movie_pos = 0;
	stage.audio_last_pos_before_movie = Audio_TellXA_Milli();

	str_ctx->cur_frame = 0;
	str_ctx->cur_slice = 0;
	str_ctx->last_frame = 0;
	str_ctx->page_frame_valid[0] = 0;
	str_ctx->page_frame_valid[1] = 0;
	str_ctx->current_page_index = 0;

	// Initialize texture descriptor for the offscreen movie texture page(s)
	str_ctx->tex.tpage = getTPage(2 /* 16bpp */, 0, MOVIE_VRAM_X, MOVIE_VRAM_Y);
	str_ctx->tex.clut = 0;
}

static void STR_StopStream(void)
{
    CdControlB(CdlPause, 0, 0);
    EnterCriticalSection();
    CdReadyCallback(NULL);
    DecDCToutCallback(NULL);
    ExitCriticalSection();
    stage.movie_is_playing = false;
}

static void Str_Update(void)
{
	StreamBuffer *new_frame = get_next_frame();
	StreamBuffer *display_frame = 0;

	Gfx_FlipWithoutOT();
	if (stage.note_scroll >= 0)
		Stage_Tick();
	DrawSync(0);

	// If a new frame arrived, decompress it and make it the current display frame.
    if (new_frame)
	{
		VLC_Context vlc_ctx;
		DecDCTvlcStart(&vlc_ctx, new_frame->mdec_data, sizeof(new_frame->mdec_data) / 4, new_frame->bs_data);
		display_frame = new_frame;
		str_ctx->last_frame = new_frame;
		str_ctx->decoding_busy = 1;
	}
	else
	{
		// No new frame available: duplicate the last decoded frame to maintain 60 Hz output.
		display_frame = str_ctx->last_frame;
		// If there is no last frame and the stream ended, stop playback.
		if (!display_frame && str_ctx->frame_ready < 0)
		{
			STR_StopStream();
			Mem_Free(str_ctx);
			Mem_Free(sector_header);
			return;
		}
		// If we still don't have anything to display yet, skip this tick.
		if (!display_frame)
			return;
	}

	// Removed pre-flip fills to avoid modifying the display page during scanout
	// Track which page index we are drawing to after the flip (gfx.c flips db last).
	// Gfx_GetDrawClip() returns draw[db].clip, so we derive page index from its y.
	// Page 0: draw at y=240, Page 1: draw at y=0 (from gfx.c initialization).
	{
		RECT *clip = Gfx_GetDrawClip();
		u8 page_idx = (clip->y == 0) ? 0 : 1; // y=0 => top page index 0, y=240 => bottom page index 1
		str_ctx->current_page_index = page_idx;
	}

    // Place the frame at the center of the currently active framebuffer
	RECT *fb_clip = Gfx_GetDrawClip();
	int  x_offset = 0;
	int  y_offset = 0;
	// Force full-screen movie to known page Y to avoid drift
	int  dst_x = 0;
	int  dst_y = (str_ctx->current_page_index == 0) ? 0 : 240;
	// Decode into an offscreen VRAM texture page instead of the visible framebuffer
	str_ctx->decode_dst_x = MOVIE_VRAM_X;
	str_ctx->decode_dst_y = MOVIE_VRAM_Y;
	str_ctx->decode_frame_width = display_frame->width;
	str_ctx->decode_frame_height = display_frame->height;

	// Draw the uploaded frame as a textured quad. Split horizontally if wider than one 16bpp tpage (256px).
	{
		int frame_w = str_ctx->decode_frame_width;
		int frame_h = str_ctx->decode_frame_height;

		// If the frame spans two 16bpp tpages, drop 1px from the second half to hide a vertical seam
		const int spans_two_pages = (frame_w > 256);
		const int seam_crop = spans_two_pages ? 1 : 0; // crop 1px from the second page
		int effective_w = frame_w - seam_crop;

		// First half (tpage at MOVIE_VRAM_X)
		int src0_w = spans_two_pages ? 255 : effective_w; // PS1 UV max per page is 255
		RECT src0 = {0, 0, src0_w, frame_h};
		RECT dst0 = {0, 0, (SCREEN_WIDTH * src0_w) / effective_w, SCREEN_HEIGHT};
		Gfx_DrawTex(&str_ctx->tex, &src0, &dst0);

		// Second half (tpage at MOVIE_VRAM_X + 256)
		int src1_w_total = frame_w - 256;
		if (src1_w_total > 0)
		{
			Gfx_Tex tex2 = str_ctx->tex;
			tex2.tpage = getTPage(2, 0, MOVIE_VRAM_X + 256, MOVIE_VRAM_Y);
			// Skip 1px at the start of the second page to eliminate the seam
			int src1_x = seam_crop ? 1 : 0;
			int src1_w = src1_w_total - src1_x;
			if (src1_w > 0)
			{
				RECT src1 = {src1_x, 0, src1_w, frame_h};
				RECT dst1 = {dst0.w, 0, SCREEN_WIDTH - dst0.w, SCREEN_HEIGHT};
				Gfx_DrawTex(&tex2, &src1, &dst1);
			}
		}
	}

	if (new_frame)
	{
		// Decoding to offscreen VRAM; no prefill required

		// Start decoding this new frame into the current draw buffer.
		DecDCTin(display_frame->mdec_data, DECDCT_MODE_16BPP);

        str_ctx->slice_pos.x = str_ctx->decode_dst_x;
		str_ctx->slice_pos.y = str_ctx->decode_dst_y;
		str_ctx->slice_pos.w = BLOCK_SIZE;
		str_ctx->slice_pos.h = str_ctx->decode_frame_height;
		str_ctx->frame_width = VRAM_X_COORD(str_ctx->decode_frame_width);
		str_ctx->frame_right_x = str_ctx->slice_pos.x + str_ctx->frame_width;
		str_ctx->slices_remaining = str_ctx->frame_width / BLOCK_SIZE;
		str_ctx->decoding_page_index = 0;

		DecDCTout(
			str_ctx->slices[str_ctx->cur_slice],
			BLOCK_SIZE * str_ctx->slice_pos.h / 2
		);
	}
	else
	{
		// Nothing to duplicate when drawing from offscreen VRAM
	}
}

void Str_Init(void)
{
	DecDCTReset(0);
	stage.movie_is_playing = false;
}

void Str_PlayFile(CdlFILE* file)
{
	str_ctx = Mem_Alloc(sizeof(StreamContext));
	sector_header = Mem_Alloc(sizeof(STR_Header));
	STR_InitStream();

	str_ctx->frame_id       = -1;
	str_ctx->dropped_frames =  0;
	str_ctx->sector_pending =  0;
	str_ctx->frame_ready    =  0;

	// Configure the CD drive to read at 2x speed and to play any XA-ADPCM
	// sectors that might be interleaved with the video data.
	u8 mode = CdlModeRT | CdlModeSpeed;
	CdControlB(CdlSetmode, (u8*) &mode, 0);

	// Start reading in real-time mode (i.e. without retrying in case of read
	// errors).
	CdControl(CdlReadS, (u8*)&file->pos, 0);

	// Wait until the first frame is buffered, but do not consume it here.
	while (!str_ctx->frame_ready && stage.movie_is_playing)
	{
		Timer_Tick();
		Pad_Update();
	}
	
	while (stage.movie_is_playing)
	{
		Timer_Tick();
		Pad_Update();
		Str_Update();
		stage.movie_pos += timer_dt;

		// Early stop condition inside loop (stage-specific cutoff)
		if (stage.stage_id == StageId_5_2 && stage.song_step == 1824)
		{
			stage.movie_is_playing = false;
			break;
		}
	}

	// Stop CD streaming and MDEC cleanly
	STR_StopStream();
	Mem_Free(str_ctx);
	Mem_Free(sector_header);

	Gfx_EnableClear();

	//Prepare CD drive for XA reading
	mode = CdlModeRT | CdlModeSF | CdlModeSize1;
	
	CdControlB(CdlSetmode, &mode, NULL);
	CdControlF(CdlPause, NULL);

}

void Str_Play(const char *filedir)
{
	CdlFILE file;

	IO_FindFile(&file, filedir);
	CdSync(0, 0);

	Str_PlayFile(&file);
}

void Str_CanPlayDef(void)
{
	//Check if have some movie to play
	 for (u8 i = 0; i < COUNT_OF(str_def); i++)
	 {
	   //Play only in story mode
	   if (str_def[i].id == stage.stage_id && stage.story)
			Str_Play(str_def[i].name);
	}
}