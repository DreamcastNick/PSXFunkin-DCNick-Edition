/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_STAGE_H
#define PSXF_GUARD_STAGE_H

#include "io.h"
#include "gfx.h"
#include "pad.h"

#include "fixed.h"
#include "character.h"
#include "player.h"
#include "object.h"
#include "font.h"
#include "debug.h"

//Stage constants
#define INPUT_LEFT  (PAD_LEFT  | PAD_SQUARE | PAD_L2)
#define INPUT_DOWN  (PAD_DOWN  | PAD_CROSS | PAD_L1)
#define INPUT_UP    (PAD_UP    | PAD_TRIANGLE | PAD_R1)
#define INPUT_RIGHT (PAD_RIGHT | PAD_CIRCLE | PAD_R2)

#define INPUT_LEFT5K  (PAD_LEFT  | PAD_SQUARE)
#define INPUT_DOWN5K  (PAD_DOWN  | PAD_CROSS)
#define INPUT_MIDDLE (PAD_L1 | PAD_L2 | PAD_R1 | PAD_R2)
#define INPUT_UP5K    (PAD_UP    | PAD_TRIANGLE)
#define INPUT_RIGHT5K (PAD_RIGHT | PAD_CIRCLE)

#define STAGE_FLAG_JUST_STEP     (1 << 0) //Song just stepped this frame
#define STAGE_FLAG_VOCAL_ACTIVE  (1 << 1) //Song's vocal track is currently active
#define STAGE_FLAG_SCORE_REFRESH (1 << 2) //Score text should be refreshed

#define STAGE_LOAD_PLAYER     (1 << 0) //Reload player character
#define STAGE_LOAD_PLAYER2    (1 << 1) //Reload player 2 character
#define STAGE_LOAD_OPPONENT   (1 << 2) //Reload opponent character
#define STAGE_LOAD_OPPONENT2  (1 << 3) //Reload opponent 2 character
#define STAGE_LOAD_GIRLFRIEND (1 << 4) //Reload girlfriend character
#define STAGE_LOAD_STAGE      (1 << 5) //Reload stage
#define STAGE_LOAD_FLAG       (1 << 7)

//Stage enums
typedef enum
{
	StageId_1_1, //Bopeebo
	StageId_1_2, //Fresh
	StageId_1_3, //Dadbattle
	StageId_1_4, //Tutorial
	
	StageId_2_1, //Spookeez
	StageId_2_2, //South
	StageId_2_3, //Monster
	
	StageId_3_1, //Pico
	StageId_3_2, //Philly
	StageId_3_3, //Blammed

	StageId_4_1, //Where Are You
	StageId_4_2, //Eruption
	StageId_4_3, //Kaio-Ken
	StageId_4_4, //Ferocious
	StageId_4_5, //Monochrome
	StageId_4_6, //Triple Trouble
	StageId_4_7, //Unbeatable
	
	StageId_5_1, //Aethos
	StageId_5_2, //Rotten Smoothie
	StageId_5_3, //Twiddlefinger
	StageId_5_4, //Crimson Awakening
	StageId_5_5, //Well Done
	StageId_5_6, //Hate Boner
	StageId_5_7, //Silly Billy
	
	StageId_Temp, //Placeholder For Stuff
	
	StageId_Max
} StageId;

typedef enum
{
	StageDiff_Easy,
	StageDiff_Normal,
	StageDiff_Hard,

	StageDiff_Max
} StageDiff;

typedef enum
{
	StageMode_Normal,
	StageMode_2P,
	StageMode_Swap,
	StageMode_Net1,
	StageMode_Net2,
} StageMode;

#define STAGE_SIDES 2
// Up to 8 controllers per side (two full multitaps worth per team)
#define STAGE_SIDE_PLAYERS_MAX 8

typedef enum
{
	StageStrumLayout_Default,
	StageStrumLayout_Background,
	StageStrumLayout_ThreeWay,
} StageStrumLayout;

typedef enum
{
	StageStrumLayout_Default,
	StageStrumLayout_Background,
	StageStrumLayout_ThreeWay,
} StageStrumLayout;

typedef enum
{
	StageTrans_Menu,
	StageTrans_NextSong,
	StageTrans_Reload,
} StageTrans;

//Stage background
typedef struct StageBack
{
	//Stage background functions
	void (*draw_hud)(struct StageBack*);
	void (*draw_fg)(struct StageBack*);
	void (*draw_md)(struct StageBack*);
	void (*draw_bg)(struct StageBack*);
	void (*free)(struct StageBack*);
} StageBack;

//Stage definitions
typedef struct
{
	//Characters
	struct
	{
		Character* (*new)();
		fixed_t x, y;
	} pchar, pchar2, ochar, ochar2, gchar;
	
	//Stage background
	StageBack* (*back)();
	
	//Camera Offsets
	fixed_t offset_x, offset_y, offset_zoom;
	
	//Song info
	fixed_t speed[3];
	
	u8 week, week_song;
	u8 music_track, music_channel;
	boolean tim;
	boolean has_note_types;
	
	StageId next_stage;
	u8 next_load;
} StageDef;

//Stage state
#define SECTION_FLAG_OPPFOCUS (1ULL << 15) //Focus on opponent
#define SECTION_FLAG_BPM_MASK 0x7FFF //1/24

typedef struct
{
	u32 end; //1/12 steps
	u16 flag;
} Section;

#define NOTE_FLAG_SUSTAIN     (1 << 5) //Note is a sustain note
#define NOTE_FLAG_SUSTAIN_END (1 << 6) //Is either end of sustain
#define NOTE_FLAG_ALT_ANIM    (1 << 7) //Note plays alt animation
#define NOTE_FLAG_MINE        (1 << 8) //Note is a mine
#define NOTE_FLAG_DANGER      (1 << 9) //Note is a danger
#define NOTE_FLAG_STATIC      (1 << 10) //Note is a static
#define NOTE_FLAG_PHANTOM     (1 << 11) //Note is a phantom
#define NOTE_FLAG_POLICE      (1 << 12) //Note is a police
#define NOTE_FLAG_MAGIC       (1 << 13) //Note is a magic
#define NOTE_FLAG_HIT         (1 << 14) //Note has been hit

typedef struct
{
	u32 pos; //1/12 steps
	u16 type;
	u16 is_opponent;
} Note;

typedef struct
{
	Character *character;
	
	fixed_t arrow_hitan[9]; //Arrow hit animation for presses

	s16 health;
	u16 combo;
	
	boolean refresh_score;
	int score, max_score;
	char score_text[30];

	boolean refresh_miss;
	s32 miss;
	char miss_text[13];
	
	boolean refresh_accuracy;
	s32 min_accuracy;
	s32 accuracy;
	s32 max_accuracy;
	char accuracy_text[21];

	char rank[13];
	
	u16 pad_held, pad_press;
	
	//player prefs for boolean back logic
	boolean visible, hud;
} PlayerState;

typedef struct
{
	DISPENV disp[2];
	DRAWENV draw[2];
	//Stage settings
	int pause_state;
	struct
	{
		s32 mode;
		boolean ghost, downscroll, middlescroll, expsync, debug, songtimer, botplay, flash;
		int savescore[StageId_Max][StageDiff_Max];
	}prefs;	
	u32 offset;

	fixed_t pause_scroll;
	u8 pause_select;
	boolean paused;

	//HUD textures
	u8 hitstatic;
	Gfx_Tex tex_note, tex_type, tex_type2, tex_hud0, tex_hud1, tex_hude, tex_static, tex_strscr;

	//font
	FontData font_cdr, font_bold;
	
	//Stage data
	const StageDef *stage_def;
	StageId stage_id;
	StageDiff stage_diff;
	// Original song info for reliable restarts (unchanged by mid-song swaps)
	StageId original_stage_id;
	StageDiff original_stage_diff;
	boolean original_story;
	// Active music/flow state (persists across mid-game swaps)
	u8 music_disc_active;      // 1, 2, or 3 (disc index)
	u16 music_track_active;    // track enum value for current song
	u8 music_channel_active;   // XA channel for current song
	StageId next_stage_active; // story progression target from original def
	u8 next_load_active;       // flags for partial reload on next
	
	IO_Data chart_data;
	Section *sections;
	Note *notes;
	size_t num_notes;
	u16 keys;
	u16 max_keys;
	
	fixed_t speed;
	fixed_t step_crochet, step_time;
	fixed_t early_safe, late_safe, early_sus_safe, late_sus_safe;
	fixed_t flash, flashspd;
	
	boolean movie_is_playing;

	fixed_t movie_pos;
	fixed_t audio_last_pos_before_movie;
	fixed_t audio_start_pos;

	//STR Lbas
	CdlFILE str_grace_lba;
	CdlFILE str_sillybilly_intro_lba;
	CdlFILE str_sillybilly_final_lba;

	//if stage have intro or no
	boolean intro;
	
	//Stage state
	boolean story;
	u8 flag;
	StageTrans trans;
	
	struct
	{
		// Specs
		boolean force;
		fixed_t speed;
		
		// Positions
		fixed_t x, y, zoom, bzoom, angle, hudangle;
		
		struct
		{
			fixed_t x, y, zoom;
		} offset;
		
		// Targets
		fixed_t tx, ty, tz;
		s16 ta, hudta;
	} camera;
	fixed_t bump, sbump;
	
	StageBack *back;
	
	Character *player;
	Character *player2;
	Character *opponent;
	Character *opponent2;
	Character *gf;
	
	Section *cur_section; //Current section
	Note *cur_note; //First visible and hittable note, used for drawing and hit detection
	
	fixed_t note_scroll, song_time, interp_time, interp_ms, interp_speed;

	struct
	{
		int* x;
		int* y;
		u16 size;
	} note;
	
	u16 last_bpm;

	int timerlength, timermin, timersec, timepassed;
	
	fixed_t time_base;
	u32 step_base;
	Section *section_base;

	// Grace period after swaps to avoid false misses
	u8 swap_grace_frames;
	
	s16 noteshakex;
	s16 noteshakey;

	s32 song_step;
	s32 song_beat;

	boolean freecam;
	
	u8 gf_speed; //Typically 4 steps, changes in Fresh
	
	PlayerState player_state[2];
	int max_score;
	
	enum
	{
		StageState_Play, //Game is playing as normal
		StageState_Dead,       //Start BREAK animation and reading extra data from CD
		StageState_DeadLoad,   //Wait for said data to be read
		StageState_DeadDrop,   //Mic drop
		StageState_DeadRetry,  //Retry prompt
		StageState_DeadDecide, //Decided
	} state;
	
	u8 note_swap;
	u8 strum_layout;
	
	//Object lists
	ObjectList objlist_splash, objlist_fg, objlist_bg;
	
	//Animations
	u16 startscreen;
} Stage;

extern Stage stage;

//Stage drawing functions
void Stage_DrawRect(const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb);
void Stage_BlendRect(const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb, int mode);
void Stage_DrawTexRotateCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, u8 angle, fixed_t hx, fixed_t hy, u8 r, u8 g, u8 b, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexRotate(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, u8 angle, fixed_t hx, fixed_t hy, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 r, u8 g, u8 b);
void Stage_DrawTexCol_FlipX(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 r, u8 g, u8 b);
void Stage_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation);
void Stage_DrawTex_FlipX(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, u8 r, u8 g, u8 b, fixed_t zoom, fixed_t rotation);
void Stage_DrawTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation);
void Stage_BlendTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation, u8 r, u8 g, u8 b, u8 mode);
void Stage_BlendTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation, u8 mode);
void Stage_BlendTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 mode);
void Stage_BlendTexV2(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, u8 mode, u8 opacity);


//Stage functions
void Stage_Load(StageId id, StageDiff difficulty, boolean story);
void Stage_Unload();
void Stage_Tick();

// Mid-game swap API (does not reload chart or music)
// Queue a swap to a specific `StageId` with selected load flags (STAGE_LOAD_*)
void Stage_RequestSwapTo(StageId target, u8 load_flags);
// Queue a swap using current `stage.stage_def->next_stage` and `next_load`
void Stage_RequestNextLoadSwap(void);

#endif
