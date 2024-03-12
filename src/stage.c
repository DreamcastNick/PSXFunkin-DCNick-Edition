/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "stage.h"

#include "mem.h"
#include "timer.h"
#include "audio.h"
#include "event.h"
#include "pad.h"
#include "main.h"
#include "random.h"
#include "mutil.h"
#include "debug.h"
#include "save.h"

#include "menu.h"
#include "pause.h"
#include "trans.h"
#include "loadscr.h"

#include "object/combo.h"
#include "object/splash.h"

//Stage constants
//#define STAGE_NOHUD //Disable the HUD

//4K
static fixed_t note_x4k[8] = {
	//BF
	 FIXED_DEC(26,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(60,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(94,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(128,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	//Opponent
	 FIXED_DEC(-128,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-94,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(-60,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-26,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

static u16 note_key4k[] = {INPUT_LEFT, INPUT_DOWN, INPUT_UP, INPUT_RIGHT};

static u8 note_anims4k[4][3] = {
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

//6K
static fixed_t note_x6k[12] = {
	//BF
	 FIXED_DEC(26,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(50,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+24
	 FIXED_DEC(74,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(98,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(122,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	//Opponent
	 FIXED_DEC(-146,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-122,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-98,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-74,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+24
	 FIXED_DEC(-50,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-26,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

static u16 note_key6k[] = {PAD_LEFT, PAD_UP, PAD_RIGHT, PAD_SQUARE, PAD_CROSS, PAD_CIRCLE};

static u8 note_anims6k[6][3] = {
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

//7K
static fixed_t note_x7k[14] = {
	//BF
	 FIXED_DEC(26,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(46,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+20
	 FIXED_DEC(66,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(86,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(106,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(126,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	//Opponent
	 FIXED_DEC(-146,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-126,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-106,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-86,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-66,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+20
	 FIXED_DEC(-46,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-26,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

static u16 note_key7k[] = {PAD_LEFT, PAD_UP, PAD_RIGHT, INPUT_MIDDLE, PAD_SQUARE, PAD_CROSS, PAD_CIRCLE};

static u8 note_anims7k[7][3] = {
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

//9K
static fixed_t note_x9k[18] = {
	//BF
	 FIXED_DEC(13,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(30,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+17
	 FIXED_DEC(47,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(64,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(81,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(98,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(115,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(132,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(149,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	//Opponent
	 FIXED_DEC(-149,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-132,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+17
	 FIXED_DEC(-115,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-98,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-81,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-64,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-47,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-30,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-13,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

static u16 note_key9k[] = {PAD_LEFT, PAD_DOWN, PAD_UP, PAD_RIGHT, INPUT_MIDDLE, PAD_SQUARE, PAD_CROSS, PAD_TRIANGLE, PAD_CIRCLE};

static u8 note_anims9k[9][3] = {
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

//Note pointers for the keys
static u16* note_key;
static u8 (*note_anims)[3];


//Stage definitions
boolean noteshake;

static u32 Sounds[7];

#include "character/bf.h"
#include "character/dad.h"
#include "character/spook.h"
#include "character/monster.h"
#include "character/pico.h"
#include "character/gf.h"

#include "stage/dummy.h"
#include "stage/week1.h"
#include "stage/week2.h"
#include "stage/week3.h"

static const StageDef stage_defs[StageId_Max] = {
	#include "stagedef_disc1.h"
};

//Stage states
Stage stage;
Debug debug;

//Stage note functions
static u16 Stage_GetNoteType(Note* note)
{
	u16 note_xor = 0;
	u16 note_type = note->type;
	
	static u16 note_types[] = {NOTE_FLAG_SUSTAIN, NOTE_FLAG_SUSTAIN_END, NOTE_FLAG_HIT};
	
	for (u16 i = 0; i < COUNT_OF(note_types); i++)
	{
		if (note_type & note_types[i])
			note_xor |= note_types[i];
	}
	
	note_type ^= note_xor;
	
	return note_type;
}

static void Stage_CheckAnimations(PlayerState *this, u8 type)
{
	this->character->set_anim(this->character, type);
}

Character* Stage_ChangeChars(Character* oldcharacter, Character* newcharacter)
{
		oldcharacter->pad_held = 0;

		return newcharacter;
}

//Stage music functions
static void Stage_StartVocal(void)
{
	if (!(stage.flag & STAGE_FLAG_VOCAL_ACTIVE))
	{
		Audio_ChannelXA(stage.stage_def->music_channel);
		stage.flag |= STAGE_FLAG_VOCAL_ACTIVE;
	}
}

static void Stage_CutVocal(void)
{
	if (stage.flag & STAGE_FLAG_VOCAL_ACTIVE)
	{
		Audio_ChannelXA(stage.stage_def->music_channel + 1);
		stage.flag &= ~STAGE_FLAG_VOCAL_ACTIVE;
	}
}

//Stage camera functions
static void Stage_FocusCharacter(Character *ch)
{
	//Use character focus settings to update target position and zoom
	stage.camera.tx = ch->x + ch->focus_x + ((stage.state == StageState_Play) ? stage.camera.offset.x : 0);
	stage.camera.ty = ch->y + ch->focus_y + ((stage.state == StageState_Play) ? stage.camera.offset.y : 0);
	stage.camera.tz = FIXED_MUL(ch->focus_zoom, event.zoom);
}

static void Stage_ScrollCamera(void)
{
	if (stage.prefs.debug)
		Debug_ScrollCamera();
	else if (stage.freecam)
	{
		if (pad_state.held & PAD_LEFT)
			stage.camera.x -= FIXED_DEC(2,1);
		if (pad_state.held & PAD_UP)
			stage.camera.y -= FIXED_DEC(2,1);
		if (pad_state.held & PAD_RIGHT)
			stage.camera.x += FIXED_DEC(2,1);
		if (pad_state.held & PAD_DOWN)
			stage.camera.y += FIXED_DEC(2,1);
		if (pad_state.held & PAD_TRIANGLE)
			stage.camera.zoom -= FIXED_DEC(1,100);
		if (pad_state.held & PAD_CROSS)
			stage.camera.zoom += FIXED_DEC(1,100);
	}
	else if (!stage.paused)
	{
		//Scroll based off current divisor
		stage.camera.x = lerp(stage.camera.x, stage.camera.tx, stage.camera.speed);
		stage.camera.y = lerp(stage.camera.y, stage.camera.ty, stage.camera.speed);
		stage.camera.zoom = lerp(stage.camera.zoom, FIXED_MUL(stage.camera.tz,stage.camera.offset.zoom), stage.camera.speed);
		stage.camera.angle = lerp(stage.camera.angle, stage.camera.ta << FIXED_SHIFT, stage.camera.speed);
		stage.camera.hudangle = lerp(stage.camera.hudangle, stage.camera.hudta << FIXED_SHIFT, stage.camera.speed);
	}
	
	//Update other camera stuff
	stage.camera.bzoom = FIXED_MUL(stage.camera.zoom, stage.bump);
}

//Stage section functions
static void Stage_ChangeBPM(u16 bpm, u16 step)
{
	//Update last BPM
	stage.last_bpm = bpm;
	
	//Update timing base
	if (stage.step_crochet)
		stage.time_base += FIXED_DIV(((fixed_t)step - stage.step_base) << FIXED_SHIFT, stage.step_crochet);
	stage.step_base = step;
	
	//Get new crochet and times
	stage.step_crochet = ((fixed_t)bpm << FIXED_SHIFT) * 8 / 240; //15/12/24
	stage.step_time = FIXED_DIV(FIXED_DEC(12,1), stage.step_crochet);
	
	//Get new crochet based values
	stage.early_safe = stage.late_safe = stage.step_crochet / 6; //10 frames
	stage.late_sus_safe = stage.late_safe;
	stage.early_sus_safe = stage.early_safe * 2 / 5;
}

static Section *Stage_GetPrevSection(Section *section)
{
	if (section > stage.sections)
		return section - 1;
	return NULL;
}

static u16 Stage_GetSectionStart(Section *section)
{
	Section *prev = Stage_GetPrevSection(section);
	if (prev == NULL)
		return 0;
	return prev->end;
}

//Section scroll structure
typedef struct
{
	fixed_t start;   //Seconds
	fixed_t length;  //Seconds
	u16 start_step;  //Sub-steps
	u16 length_step; //Sub-steps
	
	fixed_t size; //Note height
} SectionScroll;

static void Stage_GetSectionScroll(SectionScroll *scroll, Section *section)
{
	//Get BPM
	u16 bpm = section->flag & SECTION_FLAG_BPM_MASK;
	
	//Get section step info
	scroll->start_step = Stage_GetSectionStart(section);
	scroll->length_step = section->end - scroll->start_step;
	
	//Get section time length
	scroll->length = (scroll->length_step * FIXED_DEC(15,1) / 12) * 24 / bpm;
	
	//Get note height
	scroll->size = FIXED_MUL(stage.speed, scroll->length * (12 * 150) / scroll->length_step) + FIXED_UNIT;
}

//Note hit detection
static u8 Stage_HitNote(PlayerState *this, u8 type, fixed_t offset)
{
	//Get hit type
	if (offset < 0)
		offset = -offset;
	
	u8 hit_type;
	if (offset > stage.late_safe * 9 / 11)
		hit_type = 3; //SHIT
	else if (offset > stage.late_safe * 6 / 11)
		hit_type = 2; //BAD
	else if (offset > stage.late_safe * 3 / 11)
		hit_type = 1; //GOOD
	else
		hit_type = 0; //SICK
	
	//Increment combo and score
	this->combo++;
	
	static const s32 score_inc[] = {
		35, //SICK
		20, //GOOD
		10, //BAD
		 5, //SHIT
	};
	this->score += score_inc[hit_type];

	this->min_accuracy += 20;

	this->max_accuracy += 20 + (hit_type*7);
	this->refresh_accuracy = true;
	this->refresh_score = true;
	
	//Restore vocals and health
	Stage_StartVocal();
	this->health += 230;
	
	//Create combo object telling of our combo
	Obj_Combo *combo = Obj_Combo_New(
		this->character->focus_x,
		this->character->focus_y,
		hit_type,
		this->combo >= 10 ? this->combo : 0xFFFF
	);
	if (combo != NULL)
		ObjectList_Add(&stage.objlist_fg, (Object*)combo);
	
	//Create note splashes if SICK
	if (hit_type == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			//Create splash object
			Obj_Splash *splash = Obj_Splash_New(
				stage.note.x[type],
				stage.note.y * (stage.prefs.downscroll ? -1 : 1),
				type % stage.keys
			);
			if (splash != NULL)
				ObjectList_Add(&stage.objlist_splash, (Object*)splash);
		}
	}
	
	return hit_type;
}

static void Stage_MissNote(PlayerState *this)
{
	this->max_accuracy += 20;
	this->refresh_accuracy = true;
	this->miss += 1;
	this->refresh_miss = true;

	if (this->combo)
	{
		//Kill combo
		if (stage.gf != NULL && this->combo > 5)
			stage.gf->set_anim(stage.gf, CharAnim_DownAlt); //Cry if we lost a large combo
		this->combo = 0;
		
		//Create combo object telling of our lost combo
		Obj_Combo *combo = Obj_Combo_New(
			this->character->focus_x,
			this->character->focus_y,
			0xFF,
			0
		);
		if (combo != NULL)
			ObjectList_Add(&stage.objlist_fg, (Object*)combo);
	}
}

static void Stage_NoteCheck(PlayerState *this, u8 type)
{
	//Perform note check
	for (Note *note = stage.cur_note;; note++)
	{
		if (!(note->type & NOTE_FLAG_MINE))
		{
			//Check if note can be hit
			fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
			if (note_fp - stage.early_safe > stage.note_scroll)
				break;
			if (note_fp + stage.late_safe < stage.note_scroll)
				continue;
			if ((note->type & NOTE_FLAG_HIT) || (Stage_GetNoteType(note) % stage.max_keys) != type || (note->type & NOTE_FLAG_SUSTAIN))
				continue;
			
			//Hit the note
			note->type |= NOTE_FLAG_HIT;

		   Stage_CheckAnimations(this, note_anims[type % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);

			u8 hit_type = Stage_HitNote(this, type, stage.note_scroll - note_fp);
			this->arrow_hitan[type % stage.keys] = stage.step_time;
			(void)hit_type;
			return;
		}
		else
		{
			//Check if mine can be hit
			fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
			if (note_fp - (stage.late_safe * 3 / 5) > stage.note_scroll)
				break;
			if (note_fp + (stage.late_safe * 2 / 5) < stage.note_scroll)
				continue;
			if ((note->type & NOTE_FLAG_HIT) || (Stage_GetNoteType(note) % stage.max_keys) != type || (note->type & NOTE_FLAG_SUSTAIN))
				continue;
			
			//Hit the mine
			note->type |= NOTE_FLAG_HIT;
	
			this->health -= 2000;

			if (this->character->spec & CHAR_SPEC_MISSANIM)
				this->character->set_anim(this->character, note_anims[type % stage.keys][2]);
			else
				this->character->set_anim(this->character, note_anims[type % stage.keys][0]);
			this->arrow_hitan[type % stage.keys] = -1;
			
			return;
		}
	}
	
	//Missed a note
	this->arrow_hitan[type % stage.keys] = -1;
	
	if (!stage.prefs.ghost)
	{	
		if (this->character->spec & CHAR_SPEC_MISSANIM)
			this->character->set_anim(this->character, note_anims[type % stage.keys][2]);

		else
			this->character->set_anim(this->character, note_anims[type % stage.keys][0]);
		Stage_MissNote(this);
		
		this->health -= 400;
		this->score -= 10;
		this->refresh_score = true;
	}
}

static void Stage_SustainCheck(PlayerState *this, u8 type)
{
	//Perform note check
	for (Note *note = stage.cur_note;; note++)
	{
		//Check if note can be hit
		fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
		if (note_fp - stage.early_sus_safe > stage.note_scroll)
			break;
		if (note_fp + stage.late_sus_safe < stage.note_scroll)
			continue;
		if ((note->type & NOTE_FLAG_HIT) || (Stage_GetNoteType(note) % stage.max_keys) != type || !(note->type & NOTE_FLAG_SUSTAIN))
			continue;
		
		//Hit the note
		note->type |= NOTE_FLAG_HIT;
		
		Stage_CheckAnimations(this, note_anims[type % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);

		Stage_StartVocal();
		this->health += 230;
		this->arrow_hitan[type % stage.keys] = stage.step_time;
			
	}
}

static void CheckNewScore()
{
	if (stage.prefs.mode != StageMode_2P && !stage.prefs.botplay)
	{
		 if (stage.player_state[0].score >= stage.prefs.savescore[stage.stage_id][stage.stage_diff])
			stage.prefs.savescore[stage.stage_id][stage.stage_diff] = stage.player_state[0].score;			
	}
}

static void Stage_ProcessPlayer(PlayerState *this, Pad *pad, boolean playing)
{
	//Handle player note presses
	if (!stage.prefs.botplay) {
		if (playing)
		{
			u8 is_opponent = (this->character == stage.opponent) ? stage.keys : 0;
			
			this->pad_held = this->character->pad_held = pad->held;
			this->pad_press = pad->press;
			
			for (int i = 0; i < stage.keys; i++)
			{
				if (this->pad_held & note_key[i])
					Stage_SustainCheck(this, i + is_opponent);
				
				if (this->pad_press & note_key[i])
					Stage_NoteCheck(this, i + is_opponent);
			}
		}
		else
		{
			this->pad_held = this->character->pad_held = 0;
			this->pad_press = 0;
		}
	}
	
	if (stage.prefs.botplay) {
		//Do perfect note checks
		if (playing)
		{
			u8 is_opponent = (this->character == stage.opponent);
			u8 i = (is_opponent) ? stage.keys : 0;
	
			u8 hit[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
			for (Note *note = stage.cur_note;; note++)
			{
				//Check if note can be hit
				fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
				if (note_fp - stage.early_safe - FIXED_DEC(12,1) > stage.note_scroll)
					break;
				if (note_fp + stage.late_safe < stage.note_scroll)
					continue;
			
				if ((note->type & NOTE_FLAG_MINE) || (stage.prefs.mode != StageMode_Swap && note->is_opponent != is_opponent) || (stage.prefs.mode == StageMode_Swap && note->is_opponent == is_opponent))
					continue;
				
				//Handle note hit
				if (!(note->type & NOTE_FLAG_SUSTAIN))
				{
					if (note->type & NOTE_FLAG_HIT)
						continue;
					if (stage.note_scroll >= note_fp)
						hit[Stage_GetNoteType(note) % stage.keys] |= 1;
					else if (!(hit[Stage_GetNoteType(note) % stage.keys] & 8))
						hit[Stage_GetNoteType(note) % stage.keys] |= 2;
				}
				else if (!(hit[Stage_GetNoteType(note) % stage.keys] & 2))
				{
					if (stage.note_scroll <= note_fp)
						hit[Stage_GetNoteType(note) % stage.keys] |= 4;
					hit[Stage_GetNoteType(note) % stage.keys] |= 8;
				}
			}
			
			//Handle input
			this->pad_held = 0;
			this->pad_press = 0;
			
			for (u8 j = 0; j < 9; j++)
			{
				if (hit[j] & 5)
				{
					this->pad_held |= note_key[j];
					Stage_SustainCheck(this, j + i);
				}
				if (hit[j] & 1)
				{
					this->pad_press |= note_key[j];
					Stage_NoteCheck(this, j + i);
				}
			}
			this->character->pad_held = this->pad_held;
		}
		else
		{
			this->pad_held = this->character->pad_held = 0;
			this->pad_press = 0;
		}
	}
}

//Stage drawing functions
void Stage_DrawRect(const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb)
{
    fixed_t xz = dst->x;
    fixed_t yz = dst->y;
    fixed_t wz = dst->w;
    fixed_t hz = dst->h;
    
    fixed_t l = (SCREEN_WIDTH2  * FIXED_UNIT) + FIXED_MUL(xz, zoom);// + FIXED_DEC(1,2);
    fixed_t t = (SCREEN_HEIGHT2 * FIXED_UNIT) + FIXED_MUL(yz, zoom);// + FIXED_DEC(1,2);
    fixed_t r = l + FIXED_MUL(wz, zoom);
    fixed_t b = t + FIXED_MUL(hz, zoom);
    
    l /= FIXED_UNIT;
    t /= FIXED_UNIT;
    r /= FIXED_UNIT;
    b /= FIXED_UNIT;
    
    RECT sdst = {
        l,
        t,
        r - l,
        b - t,
    };
    Gfx_DrawRect(&sdst, cr, cg, cb);
}

void Stage_BlendRect(const RECT_FIXED *dst, fixed_t zoom, u8 cr, u8 cg, u8 cb, int mode)
{
    fixed_t xz = dst->x;
    fixed_t yz = dst->y;
    fixed_t wz = dst->w;
    fixed_t hz = dst->h;
    
    fixed_t l = (SCREEN_WIDTH2  * FIXED_UNIT) + FIXED_MUL(xz, zoom);// + FIXED_DEC(1,2);
    fixed_t t = (SCREEN_HEIGHT2 * FIXED_UNIT) + FIXED_MUL(yz, zoom);// + FIXED_DEC(1,2);
    fixed_t r = l + FIXED_MUL(wz, zoom);
    fixed_t b = t + FIXED_MUL(hz, zoom);
    
    l /= FIXED_UNIT;
    t /= FIXED_UNIT;
    r /= FIXED_UNIT;
    b /= FIXED_UNIT;
    
    RECT sdst = {
        l,
        t,
        r - l,
        b - t,
    };
    Gfx_BlendRect(&sdst, cr, cg, cb, mode);
}

void Stage_DrawTexRotateCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, u8 angle, fixed_t hx, fixed_t hy, u8 cr, u8 cg, u8 cb, fixed_t zoom, fixed_t rotation)
{
    fixed_t xz = dst->x;
    fixed_t yz = dst->y;
    fixed_t wz = dst->w;
    fixed_t hz = dst->h;
    
    // Calculate the rotated coordinates
    u8 rotationAngle = rotation / FIXED_UNIT;  // Specify the desired rotation angle (in degrees)
    fixed_t rotatedX = FIXED_MUL(xz,FIXED_DEC(MUtil_Cos(rotationAngle),256)) - FIXED_MUL(yz,FIXED_DEC(MUtil_Sin(rotationAngle),256));
    fixed_t rotatedY = FIXED_MUL(xz,FIXED_DEC(MUtil_Sin(rotationAngle),256)) + FIXED_MUL(yz,FIXED_DEC(MUtil_Cos(rotationAngle),256));
	
    fixed_t l = (SCREEN_WIDTH2  * FIXED_UNIT) + FIXED_MUL(rotatedX, zoom);// + FIXED_DEC(1,2);
    fixed_t t = (SCREEN_HEIGHT2 * FIXED_UNIT) + FIXED_MUL(rotatedY, zoom);// + FIXED_DEC(1,2);
    fixed_t r = l + FIXED_MUL(wz, zoom);
    fixed_t b = t + FIXED_MUL(hz, zoom);
    
    l /= FIXED_UNIT;
    t /= FIXED_UNIT;
    r /= FIXED_UNIT;
    b /= FIXED_UNIT;
    
    RECT sdst = {
        l,
        t,
        r - l,
        b - t,
    };
    Gfx_DrawTexRotateCol(tex, src, &sdst, angle + rotationAngle, hx, hy, cr, cg, cb);
}

void Stage_DrawTexRotate(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, u8 angle, fixed_t hx, fixed_t hy, fixed_t zoom, fixed_t rotation)
{
    Stage_DrawTexRotateCol(tex, src, dst, angle, hx, hy, 128, 128, 128, zoom, rotation);
}

void Stage_DrawTexCol(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 cr, u8 cg, u8 cb)
{
    fixed_t xz = dst->x;
    fixed_t yz = dst->y;
    fixed_t wz = dst->w;
    fixed_t hz = dst->h;
    
    // Calculate the rotated coordinates
    u8 rotationAngle = rotation / FIXED_UNIT;  // Specify the desired rotation angle (in degrees)
    fixed_t rotatedX = FIXED_MUL(xz,FIXED_DEC(MUtil_Cos(rotationAngle),256)) - FIXED_MUL(yz,FIXED_DEC(MUtil_Sin(rotationAngle),256));
    fixed_t rotatedY = FIXED_MUL(xz,FIXED_DEC(MUtil_Sin(rotationAngle),256)) + FIXED_MUL(yz,FIXED_DEC(MUtil_Cos(rotationAngle),256));
	
    fixed_t l = (SCREEN_WIDTH2  * FIXED_UNIT) + FIXED_MUL(rotatedX, zoom) + FIXED_DEC(1,2);
    fixed_t t = (SCREEN_HEIGHT2 * FIXED_UNIT) + FIXED_MUL(rotatedY, zoom) + FIXED_DEC(1,2);
    fixed_t r = l + FIXED_MUL(wz, zoom);
    fixed_t b = t + FIXED_MUL(hz, zoom);
    
    l /= FIXED_UNIT;
    t /= FIXED_UNIT;
    r /= FIXED_UNIT;
    b /= FIXED_UNIT;
    
    RECT sdst = {
        l,
        t,
        r - l,
        b - t,
    };
	
    Gfx_DrawTexRotateCol(tex, src, &sdst, rotationAngle, 0, 0, cr, cg, cb);
}

void Stage_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation)
{
    Stage_DrawTexCol(tex, src, dst, zoom, rotation, 0x80, 0x80, 0x80);
}

void Stage_DrawTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, u8 r, u8 g, u8 b, fixed_t zoom, fixed_t rotation)
{
    u8 rotationAngle = rotation / FIXED_UNIT;  // Specify the desired rotation angle (in degrees)
    fixed_t cosAngle = FIXED_DEC(MUtil_Cos(rotationAngle), 256);
    fixed_t sinAngle = FIXED_DEC(MUtil_Sin(rotationAngle), 256);

    fixed_t x0 = FIXED_MUL(p0->x, cosAngle) - FIXED_MUL(p0->y, sinAngle);
    fixed_t y0 = FIXED_MUL(p0->x, sinAngle) + FIXED_MUL(p0->y, cosAngle);
    fixed_t x1 = FIXED_MUL(p1->x, cosAngle) - FIXED_MUL(p1->y, sinAngle);
    fixed_t y1 = FIXED_MUL(p1->x, sinAngle) + FIXED_MUL(p1->y, cosAngle);
    fixed_t x2 = FIXED_MUL(p2->x, cosAngle) - FIXED_MUL(p2->y, sinAngle);
    fixed_t y2 = FIXED_MUL(p2->x, sinAngle) + FIXED_MUL(p2->y, cosAngle);
    fixed_t x3 = FIXED_MUL(p3->x, cosAngle) - FIXED_MUL(p3->y, sinAngle);
    fixed_t y3 = FIXED_MUL(p3->x, sinAngle) + FIXED_MUL(p3->y, cosAngle);

    // Get screen-space points
    POINT s0 = {SCREEN_WIDTH2 + (FIXED_MUL(x0, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y0, zoom) / FIXED_UNIT)};
    POINT s1 = {SCREEN_WIDTH2 + (FIXED_MUL(x1, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y1, zoom) / FIXED_UNIT)};
    POINT s2 = {SCREEN_WIDTH2 + (FIXED_MUL(x2, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y2, zoom) / FIXED_UNIT)};
    POINT s3 = {SCREEN_WIDTH2 + (FIXED_MUL(x3, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y3, zoom) / FIXED_UNIT)};

    Gfx_DrawTexArbCol(tex, src, &s0, &s1, &s2, &s3, r, g, b);
}

void Stage_DrawTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation)
{
    Stage_DrawTexArbCol(tex, src, p0, p1, p2, p3, 0x80, 0x80, 0x80, zoom, rotation);
}

void Stage_BlendTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation, u8 r, u8 g, u8 b, u8 mode)
{
    u8 rotationAngle = rotation / FIXED_UNIT;  // Specify the desired rotation angle (in degrees)
    fixed_t cosAngle = FIXED_DEC(MUtil_Cos(rotationAngle), 256);
    fixed_t sinAngle = FIXED_DEC(MUtil_Sin(rotationAngle), 256);

    fixed_t x0 = FIXED_MUL(p0->x, cosAngle) - FIXED_MUL(p0->y, sinAngle);
    fixed_t y0 = FIXED_MUL(p0->x, sinAngle) + FIXED_MUL(p0->y, cosAngle);
    fixed_t x1 = FIXED_MUL(p1->x, cosAngle) - FIXED_MUL(p1->y, sinAngle);
    fixed_t y1 = FIXED_MUL(p1->x, sinAngle) + FIXED_MUL(p1->y, cosAngle);
    fixed_t x2 = FIXED_MUL(p2->x, cosAngle) - FIXED_MUL(p2->y, sinAngle);
    fixed_t y2 = FIXED_MUL(p2->x, sinAngle) + FIXED_MUL(p2->y, cosAngle);
    fixed_t x3 = FIXED_MUL(p3->x, cosAngle) - FIXED_MUL(p3->y, sinAngle);
    fixed_t y3 = FIXED_MUL(p3->x, sinAngle) + FIXED_MUL(p3->y, cosAngle);

    // Get screen-space points
    POINT s0 = {SCREEN_WIDTH2 + (FIXED_MUL(x0, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y0, zoom) / FIXED_UNIT)};
    POINT s1 = {SCREEN_WIDTH2 + (FIXED_MUL(x1, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y1, zoom) / FIXED_UNIT)};
    POINT s2 = {SCREEN_WIDTH2 + (FIXED_MUL(x2, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y2, zoom) / FIXED_UNIT)};
    POINT s3 = {SCREEN_WIDTH2 + (FIXED_MUL(x3, zoom) / FIXED_UNIT), SCREEN_HEIGHT2 + (FIXED_MUL(y3, zoom) / FIXED_UNIT)};
    
    Gfx_BlendTexArbCol(tex, src, &s0, &s1, &s2, &s3, r, g, b, mode);
}

void Stage_BlendTexArb(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, fixed_t zoom, fixed_t rotation, u8 mode)
{
    Stage_BlendTexArbCol(tex, src, p0, p1, p2, p3, zoom, rotation, 0x80, 0x80, 0x80, mode);
}

void Stage_BlendTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 mode)
{
	fixed_t xz = dst->x;
	fixed_t yz = dst->y;
	fixed_t wz = dst->w;
	fixed_t hz = dst->h;
	
    // Calculate the rotated coordinates
    u8 rotationAngle = rotation / FIXED_UNIT;  // Specify the desired rotation angle (in degrees)
    fixed_t rotatedX = FIXED_MUL(xz,FIXED_DEC(MUtil_Cos(rotationAngle),256)) - FIXED_MUL(yz,FIXED_DEC(MUtil_Sin(rotationAngle),256));
    fixed_t rotatedY = FIXED_MUL(xz,FIXED_DEC(MUtil_Sin(rotationAngle),256)) + FIXED_MUL(yz,FIXED_DEC(MUtil_Cos(rotationAngle),256));
	
    fixed_t l = (SCREEN_WIDTH2  * FIXED_UNIT) + FIXED_MUL(rotatedX, zoom);// + FIXED_DEC(1,2);
    fixed_t t = (SCREEN_HEIGHT2 * FIXED_UNIT) + FIXED_MUL(rotatedY, zoom);// + FIXED_DEC(1,2);
    fixed_t r = l + FIXED_MUL(wz, zoom);
    fixed_t b = t + FIXED_MUL(hz, zoom);
    
    l /= FIXED_UNIT;
    t /= FIXED_UNIT;
    r /= FIXED_UNIT;
    b /= FIXED_UNIT;
    
    RECT sdst = {
        l,
        t,
        r - l,
        b - t,
    };
	
    Gfx_BlendTexRotate(tex, src, &sdst, rotationAngle, 0, 0, mode);
}

//Stage HUD functions
static void Stage_DrawHealth(s16 health, u8 i, s8 ox)
{
	//Check if we should use 'dying' frame
	s8 dying;
	if (ox < 0)
		dying = (health >= 18000) * 36;
	else
		dying = (health <= 2000) * 36;
	
	//Get src and dst
	fixed_t hx = (128 << FIXED_SHIFT) * (10000 - health) / 10000;
	RECT src = {
		(i % 3) * 72 + dying,
		(i / 3) * 36,
		36,
		36
	};
	RECT_FIXED dst = {
		hx + ox * FIXED_DEC(16,1) - FIXED_DEC(16,1),
		FIXED_DEC(70,1),
		src.w << FIXED_SHIFT,
		src.h << FIXED_SHIFT
	};
	if (stage.prefs.downscroll)
		dst.y = -dst.y - FIXED_DEC(44,1);

	//invert icon
	if (stage.prefs.mode == StageMode_Swap)
	{
	dst.x += dst.w;
	dst.w = -dst.w;
	}
	
	//Draw health icon
	Stage_DrawTex(&stage.tex_hud1, &src, &dst, FIXED_MUL(stage.bump, stage.sbump), stage.camera.hudangle);
}

static void Stage_DrawHealthBar(s16 x, s32 color)
{	
	//colors for health bar
	u8 red = (color >> 16) & 0xFF;
	u8 blue = (color >> 8) & 0xFF;
	u8 green = (color) & 0xFF;
	//Get src and dst
	RECT src = {
		0,
	  252,
	    x,
		4
	};
	RECT_FIXED dst = {
		FIXED_DEC(-128,1), 
		(SCREEN_HEIGHT2 - 32) << FIXED_SHIFT, 
		FIXED_DEC(src.w,1), 
		FIXED_DEC(10,1)
	};
	if (stage.prefs.downscroll)
		dst.y = -dst.y - dst.h;
	
	Stage_DrawTexCol(&stage.tex_hud1, &src, &dst, stage.bump, stage.camera.hudangle, red >> 1, blue >> 1, green >> 1);
}

static void Stage_DrawStrum(u8 i, RECT *note_src, RECT_FIXED *note_dst)
{
	(void)note_dst;
	
	PlayerState *this;
	if (stage.prefs.mode == StageMode_Swap)
		this = &stage.player_state[(i < stage.keys)];
	else
		this = &stage.player_state[(i >= stage.keys)];
	
	i %= stage.keys;
	
	if (this->arrow_hitan[i] > 0)
	{
		//Play hit animation
		u8 frame = ((this->arrow_hitan[i] << 1) / stage.step_time) & 1;
		note_src->x = i * stage.note.size;
		note_src->y = stage.note.size * 3 - (frame * stage.note.size);
		
		this->arrow_hitan[i] -= timer_dt;
		if (this->arrow_hitan[i] <= 0)
		{
			if (this->pad_held & note_key[i])
				this->arrow_hitan[i] = 1;
			else
				this->arrow_hitan[i] = 0;
		}
	}
	else if (this->arrow_hitan[i] < 0)
	{
		//Play depress animation
		note_src->x = i * stage.note.size;
		note_src->y = stage.note.size * 4;
		if (!(this->pad_held & note_key[i]))
			this->arrow_hitan[i] = 0;
	}
	else
	{
		note_src->x = i * stage.note.size;
		note_src->y = 0;
	}
}

static void Stage_DrawNotes(void)
{
	//Initialize scroll state
	SectionScroll scroll;
	scroll.start = stage.time_base;
	
	Section *scroll_section = stage.section_base;
	Stage_GetSectionScroll(&scroll, scroll_section);
	
	//Push scroll back until cur_note is properly contained
	while (scroll.start_step > stage.cur_note->pos)
	{
		//Look for previous section
		Section *prev_section = Stage_GetPrevSection(scroll_section);
		if (prev_section == NULL)
			break;
		
		//Push scroll back
		scroll_section = prev_section;
		Stage_GetSectionScroll(&scroll, scroll_section);
		scroll.start -= scroll.length;
	}
	
	//Draw notes
	for (Note *note = stage.cur_note; note->pos != 0xFFFF; note++)
	{
		//Update scroll
		while (note->pos >= scroll_section->end)
		{
			//Push scroll forward
			scroll.start += scroll.length;
			Stage_GetSectionScroll(&scroll, ++scroll_section);
		}
		
		//Check if opponent should draw as bot
		u8 bot = (stage.prefs.mode >= StageMode_2P) ? 0 : note->is_opponent;
		
		//Get note information
		u8 i = note->is_opponent;
		PlayerState *this = &stage.player_state[i];
		
		fixed_t note_fp = (fixed_t)note->pos << FIXED_SHIFT;
		fixed_t time = (scroll.start - stage.song_time) + (scroll.length * (note->pos - scroll.start_step) / scroll.length_step);
		fixed_t y = stage.note.y + FIXED_MUL(stage.speed, time * 150);
		
		//Check if went above screen
		if (y < FIXED_DEC(-stage.note.size / 2 - SCREEN_HEIGHT2, 1))
		{
			//Wait for note to exit late time
			if (note_fp + stage.late_safe >= stage.note_scroll)
				continue;
			
			//Miss note if player's note
			if (!(note->type & (NOTE_FLAG_HIT | NOTE_FLAG_MINE)) && !bot)
			{
				if (stage.prefs.mode < StageMode_Net1 || i == ((stage.prefs.mode == StageMode_Net1) ? 0 : 1))
				{
					//Missed note
					Stage_CutVocal();
					Stage_MissNote(this);
					this->health -= 475;
				}
			}
			
			//Update current note
			stage.cur_note++;
		}
		else
		{
			//Don't draw if below screen
			RECT note_src;
			RECT_FIXED note_dst;
			if (y > (FIXED_DEC(SCREEN_HEIGHT,2) + scroll.size) || note->pos == 0xFFFF)
				break;
			
			//Draw note
			if (note->type & NOTE_FLAG_SUSTAIN)
			{
				//Check for sustain clipping
				fixed_t clip;
				y -= scroll.size;
				if ((note->type & (NOTE_FLAG_HIT)) || (bot) || ((this->pad_held & note_key[Stage_GetNoteType(note) % stage.keys]) && (note_fp + stage.late_sus_safe >= stage.note_scroll)))
				{
					clip = FIXED_DEC(32 - SCREEN_HEIGHT2, 1) - y;
					if (clip < 0)
						clip = 0;
				}
				else
				{
					clip = 0;
				}
				
				//Draw sustain
				if (note->type & NOTE_FLAG_SUSTAIN_END)
				{
					if (clip < (stage.note.size - (stage.note.size / 4)) << FIXED_SHIFT)
					{
						note_src.x = (Stage_GetNoteType(note) % stage.keys) * stage.note.size;
						note_src.y = (stage.note.size * 5) + (clip >> FIXED_SHIFT);
						note_src.w = stage.note.size;
						note_src.h = stage.note.size - (clip >> FIXED_SHIFT);
						
						note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
						note_dst.y = y + clip;
						note_dst.w = (note_src.w << FIXED_SHIFT);
						note_dst.h = (note_src.h << FIXED_SHIFT);
						
						if (stage.prefs.downscroll)
						{
							note_dst.y = -note_dst.y;
							note_dst.h = -note_dst.h;
						}
						Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
					}
				}
				else
				{
					//Get note height
					fixed_t next_time = (scroll.start - stage.song_time) + (scroll.length * (note->pos + 12 - scroll.start_step) / scroll.length_step);
					fixed_t next_y = stage.note.y + FIXED_MUL(stage.speed, next_time * 150) - scroll.size;
					fixed_t next_size = next_y - y;
					
					if (clip < next_size)
					{
						note_src.x = (Stage_GetNoteType(note) % stage.keys) * stage.note.size;
						note_src.y = stage.note.size * 5;
						note_src.w = stage.note.size;
						note_src.h = stage.note.size / 2;
						
						note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
						note_dst.y = y + clip;
						note_dst.w = note_src.w << FIXED_SHIFT;
						note_dst.h = (next_y - y) - clip;
						
						if (stage.prefs.downscroll)
							note_dst.y = -note_dst.y - note_dst.h;
						Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
					}
				}
			}
			else if (note->type & NOTE_FLAG_MINE)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;
				
				//Draw note body
				note_src.x = 192 + ((note->type & 0x1) << 5);
				note_src.y = (note->type & 0x2) << 4;
				note_src.w = 32;
				note_src.h = 32;
				
				note_dst.x = stage.note.x[(note->type % stage.max_keys)] - FIXED_DEC(16,1);
				note_dst.y = y - FIXED_DEC(16,1);
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;
				
				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
				
				//Draw note fire
				note_src.x = 192 + ((animf_count & 0x1) << 5);
				note_src.y = 64 + ((animf_count & 0x2) * 24);
				note_src.w = 32;
				note_src.h = 48;
				
				if (stage.prefs.downscroll)
				{
					note_dst.y += note_dst.h;
					note_dst.h = note_dst.h * -3 / 2;
				}
				else
				{
					note_dst.h = note_dst.h * 3 / 2;
				}
				Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
			}
			else
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;
				
				//Draw note
				note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
				note_src.y = stage.note.size;
				note_src.w = stage.note.size;
				note_src.h = stage.note.size;
				
				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;
				
				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
			}
		}
	}
}

//Timer Code
#include "audio_def.h"

static void Stage_TimerGetLength(void)
{
	const XA_TrackDef *track_def = &xa_tracks[stage.stage_def->music_track]; //get length of the music
	stage.timerlength = (track_def->length / 75 / IO_SECT_SIZE) - 1; //convert in seconds

	(void)xa_paths; //just for remove boring warning

	//minutes stuff
	stage.timermin = stage.timerlength / 60;

	//seconds stuff
	stage.timersec = stage.timerlength % 60;

	stage.timepassed = 0;
}

static void Stage_TimerTick(void)
{
	if (!stage.paused)
	{
		if (stage.song_step >= 0)
		{
			//increasing variable using delta time to avoid desync
			if (stage.timepassed < stage.timerlength * 60)
				stage.timepassed += timer_dt/12;

			//seconds checker
			if ((stage.timepassed % 60) == 59) //everytime dat variable be a multiple of 59, remove 1 second
			{
				stage.timersec--;

				if (stage.timersec < 0)
					stage.timersec += 60;
			}

			//minutes checker
			if (stage.timersec >= 59 && (stage.timepassed % 60) == 59)
				stage.timermin--;
		}
	}

		
	//don't draw timer if "song timer" option not be enable
	if (stage.prefs. songtimer)
	{
		char text[0x80];

		//format string
		sprintf(text, "%d : %s%d", stage.timermin, (stage.timersec > 9) ?"" :"0", stage.timersec); //making this for avoid cases like 1:4

		//Draw text
		stage.font_cdr.draw(&stage.font_cdr,
		text,
		FIXED_DEC(-18,1),
		(stage.prefs.downscroll) ? FIXED_DEC(103,1) : FIXED_DEC(-111,1),
		FontAlign_Left
	);

		//draw square length
		RECT square_black = {0, 252, 111, 4};
		RECT square_fill = {0, 252, (110 * (stage.timepassed) / (stage.timerlength * 60)), 4};

		RECT_FIXED square_dst = {
		FIXED_DEC(-56,1),
		FIXED_DEC(-110,1),
		0,
		FIXED_DEC(10,1)
	};

		if (stage.prefs.downscroll)
			square_dst.y = -square_dst.y - square_dst.h + FIXED_DEC(1,1);

		square_dst.w = square_fill.w << FIXED_SHIFT;
		Stage_DrawTex(&stage.tex_hud1, &square_fill, &square_dst, stage.bump, stage.camera.hudangle);
		
		square_dst.w = square_black.w << FIXED_SHIFT;
		Stage_DrawTexCol(&stage.tex_hud1, &square_black, &square_dst, stage.bump, stage.camera.hudangle,  0,  0,  0);
	}
}

static void Stage_CountDown(void)
{
	if (((-stage.song_step % 5) == 0) && stage.flag & STAGE_FLAG_JUST_STEP)
		Audio_PlaySound(Sounds[3 - -stage.song_step /6], 0x3fff);

	s8 sequence = 2 - (-stage.song_step) / 5;

	s8 special;

	special = 0;

	RECT sequence_src = {
		((sequence + special) % 2) * 100, 
		((sequence + special) / 2) * 50, 
		100, 
		50
		};	
	RECT_FIXED sequence_dst = {
		FIXED_DEC(-85,1), 
		FIXED_DEC(-50,1), 
		FIXED_DEC(100 * 2,1), 
		FIXED_DEC(50 * 2,1)};	
	
	if (stage.song_step > -15)
	Stage_DrawTex(&stage.tex_hude, &sequence_src, &sequence_dst, stage.bump, stage.camera.hudangle);
}

//Stage loads
static void Stage_LoadPlayer(void)
{
	//Load player character
	Character_Free(stage.player);
	stage.player = stage.stage_def->pchar.new(stage.stage_def->pchar.x, stage.stage_def->pchar.y);
}

static void Stage_LoadPlayer2(void)
{
	//Load player character
	Character_Free(stage.player2);
	if (stage.stage_def->pchar2.new != NULL) {
		stage.player2 = stage.stage_def->pchar2.new(stage.stage_def->pchar2.x, stage.stage_def->pchar2.y);
	}
	else
		stage.player2 = NULL;
}

static void Stage_LoadOpponent(void)
{
	//Load opponent character
	Character_Free(stage.opponent);
	stage.opponent = stage.stage_def->ochar.new(stage.stage_def->ochar.x, stage.stage_def->ochar.y);
}

static void Stage_LoadOpponent2(void)
{
	//Load opponent character
	Character_Free(stage.opponent2);
	if (stage.stage_def->ochar2.new != NULL) {
		stage.opponent2 = stage.stage_def->ochar2.new(stage.stage_def->ochar2.x, stage.stage_def->ochar2.y);
	}
	else
		stage.opponent2 = NULL;
}

static void Stage_LoadGirlfriend(void)
{
	//Load girlfriend character
	Character_Free(stage.gf);
	if (stage.stage_def->gchar.new != NULL)
		stage.gf = stage.stage_def->gchar.new(stage.stage_def->gchar.x, stage.stage_def->gchar.y);
	else
		stage.gf = NULL;
}

static void Stage_LoadStage(void)
{
	//Load back
	if (stage.back != NULL)
		stage.back->free(stage.back);
	stage.back = stage.stage_def->back();
}

static void Stage_LoadChart(void)
{
	//Load stage data
	char chart_path[64];
	//mod path

	if (stage.stage_def->week & 0x80)
	{
		//Use mod path convention
		static const char *mod_format[] = {
			"\\MOD1\\MOD1.%d%c.CHT;1", //Week 1 Shaggy
		};
		
		sprintf(chart_path, mod_format[stage.stage_def->week & 0x7F], stage.stage_def->week_song, "ENH"[stage.stage_diff]);
	}

	else
	{
	//Use standard path convention
	sprintf(chart_path, "\\WEEK%d\\%d.%d%c.CHT;1", stage.stage_def->week, stage.stage_def->week, stage.stage_def->week_song, "ENH"[stage.stage_diff]);
	}
	
	if (stage.chart_data != NULL)
		Mem_Free(stage.chart_data);
	stage.chart_data = IO_Read(chart_path);
	u8 *chart_byte = (u8*)stage.chart_data;
	
	#ifdef PSXF_PC
		//Get lengths
		u16 note_off = chart_byte[0] | (chart_byte[1] << 8);
		
		u8 *section_p = chart_byte + 2;
		u8 *note_p = chart_byte + note_off;
		
		u8 *section_pp;
		u8 *note_pp;
		
		size_t sections = (note_off - 2) >> 2;
		size_t notes = 0;
		
		for (note_pp = note_p;; note_pp += 4)
		{
			notes++;
			u16 pos = note_pp[0] | (note_pp[1] << 8);
			if (pos == 0xFFFF)
				break;
		}
		
		if (notes)
			stage.num_notes = notes - 1;
		else
			stage.num_notes = 0;
		
		//Realloc for separate structs
		size_t sections_size = sections * sizeof(Section);
		size_t notes_size = notes * sizeof(Note);
		size_t notes_off = MEM_ALIGN(sections_size);
		
		u8 *nchart = Mem_Alloc(notes_off + notes_size);
		
		Section *nsection_p = stage.sections = (Section*)nchart;
		section_pp = section_p;
		for (size_t i = 0; i < sections; i++, section_pp += 4, nsection_p++)
		{
			nsection_p->end = section_pp[0] | (section_pp[1] << 8);
			nsection_p->flag = section_pp[2] | (section_pp[3] << 8);
		}
		
		Note *nnote_p = stage.notes = (Note*)(nchart + notes_off);
		note_pp = note_p;
		for (size_t i = 0; i < notes; i++, note_pp += 4, nnote_p++)
		{
			nnote_p->pos = note_pp[0] | (note_pp[1] << 8);
			nnote_p->type = note_pp[2] | (note_pp[3] << 8);
		}
		
		//Use reformatted chart
		Mem_Free(stage.chart_data);
		stage.chart_data = (IO_Data)nchart;
	#else
		//Directly use section and notes pointers
		stage.sections = (Section*)(chart_byte + 4);
		stage.notes = (Note*)(chart_byte + ((u16*)stage.chart_data)[1]);
		
		for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
			stage.num_notes++;
	#endif
	
	//Swap chart
	if (stage.prefs.mode == StageMode_Swap)
	{
		for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
			note->is_opponent = !note->is_opponent;
	}
	
	//Count max scores
	stage.player_state[0].max_score = 0;
	stage.player_state[1].max_score = 0;
	for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
	{
		if (note->type & (NOTE_FLAG_SUSTAIN | NOTE_FLAG_MINE))
			continue;
		if (note->is_opponent)
			stage.player_state[1].max_score += 35;
		else
			stage.player_state[0].max_score += 35;
	}
	if (stage.prefs.mode >= StageMode_2P && stage.player_state[1].max_score > stage.player_state[0].max_score)
		stage.max_score = stage.player_state[1].max_score;
	else
		stage.max_score = stage.player_state[0].max_score;
	
	stage.cur_section = stage.sections;
	stage.cur_note = stage.notes;
	
	stage.speed = stage.stage_def->speed[stage.stage_diff];
	stage.keys = *((u16*)stage.chart_data);
	stage.max_keys = stage.keys * 2;
	
	stage.step_crochet = 0;
	stage.time_base = 0;
	stage.step_base = 0;
	stage.section_base = stage.cur_section;
	Stage_ChangeBPM(stage.cur_section->flag & SECTION_FLAG_BPM_MASK, 0);
}

static void Stage_LoadSFX(void)
{
	//Load SFX
	CdlFILE file;

		//intro sound
		for (u8 i = 0; i < 4;i++)
		{
			char text[0x80];
			sprintf(text, "\\SOUNDS\\INTRO%d.VAG;1", i);
			IO_FindFile(&file, text);
			u32 *data = IO_ReadFile(&file);
			Sounds[i] = Audio_LoadVAGData(data, file.size);
			Mem_Free(data);
	}
}

static void Stage_LoadMusic(void)
{
	//Offset sing ends
	stage.player->sing_end -= stage.note_scroll;
	if (stage.player2 != NULL)
		stage.player2->sing_end -= stage.note_scroll;
	stage.opponent->sing_end -= stage.note_scroll;
	if (stage.opponent2 != NULL)
		stage.opponent2->sing_end -= stage.note_scroll;
	if (stage.gf != NULL)
		stage.gf->sing_end -= stage.note_scroll;
	
	//Find music file and begin seeking to it
	Audio_SeekXA_Track(stage.stage_def->music_track);
	
	//Initialize music state
	stage.note_scroll = FIXED_DEC(-5 * 6 * 12,1);
	stage.song_time = FIXED_DIV(stage.note_scroll, stage.step_crochet);
	stage.interp_time = 0;
	stage.interp_ms = 0;
	stage.interp_speed = 0;
	
	//Offset sing ends again
	stage.player->sing_end += stage.note_scroll;
	if (stage.player2 != NULL)
		stage.player2->sing_end += stage.note_scroll;
	stage.opponent->sing_end += stage.note_scroll;
	if (stage.opponent2 != NULL)
		stage.opponent2->sing_end += stage.note_scroll;
	if (stage.gf != NULL)
		stage.gf->sing_end += stage.note_scroll;
}

static void Stage_LoadState(void)
{
	//Get Length of Song
	Stage_TimerGetLength();
	//Initialize stage state
	stage.flag = STAGE_FLAG_VOCAL_ACTIVE;
	
	stage.gf_speed = 1 << 2;
	
	stage.state = StageState_Play;
	
	if (stage.prefs.mode == StageMode_Swap)
	{
		stage.player_state[0].character = stage.opponent;
		stage.player_state[1].character = stage.player;
	}
	else
	{
		stage.player_state[0].character = stage.player;
		stage.player_state[1].character = stage.opponent;
	}

	for (int i = 0; i < 2; i++)
	{
		memset(stage.player_state[i].arrow_hitan, 0, sizeof(stage.player_state[i].arrow_hitan));
		
		stage.player_state[i].health = 10000;
		stage.player_state[i].combo = 0;
		if (!stage.prefs.debug)
			stage.freecam = 0;
		stage.player_state[i].miss = 0;
		stage.player_state[i].accuracy = 0;
		stage.player_state[i].max_accuracy = 0;
		stage.player_state[i].min_accuracy = 0;
		stage.player_state[i].refresh_score = false;
		stage.player_state[i].score = 0;
		stage.song_beat = 0;
		stage.paused = false;
		strcpy(stage.player_state[i].accuracy_text, "Accuracy: ?");
		strcpy(stage.player_state[i].miss_text, "Misses: 0");
		strcpy(stage.player_state[i].score_text, "Score: ?");
		
		stage.player_state[i].pad_held = stage.player_state[i].pad_press = 0;
	}
	
	//Update the keys
	switch (stage.keys)
	{
		//6 Key
		case 4:
			stage.note.x = note_x4k;
			stage.note.y = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
			note_anims = note_anims4k;
			note_key = note_key4k;
			stage.note.size = 32;
		break;
		//6 Key
		case 6:
			stage.note.x = note_x6k;
			stage.note.y = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
			note_anims = note_anims6k;
			note_key = note_key6k;
			stage.note.size = 24;
		break;
		//7 Key
		case 7:
			stage.note.x = note_x7k;
			stage.note.y = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
			note_anims = note_anims7k;
			note_key = note_key7k;
			stage.note.size = 24;
		break;
		//9 Key
		case 9:
			stage.note.x = note_x9k;
			stage.note.y = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
			note_anims = note_anims9k;
			note_key = note_key9k;
			stage.note.size = 16;
		break;
		//4 Key
		default:	
			stage.note.x = note_x4k;
			stage.note.y = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
			note_anims = note_anims4k;
			note_key = note_key4k;
			stage.note.size = 32;
		break;
	}
	
	ObjectList_Free(&stage.objlist_splash);
	ObjectList_Free(&stage.objlist_fg);
	ObjectList_Free(&stage.objlist_bg);
}

//Stage functions
void Stage_Load(StageId id, StageDiff difficulty, boolean story)
{
	//Get stage definition
	stage.stage_def = &stage_defs[stage.stage_id = id];
	stage.stage_diff = difficulty;
	stage.story = story;
	
	//Load HUD textures
	Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
	Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\HUD1.TIM;1"), GFX_LOADTEX_FREE);
	Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);

	//Load stage background
	Stage_LoadStage();

	//Load SFX
	Stage_LoadSFX();
	
	//Load Events
	Load_Events();

	//load fonts
	FontData_Load(&stage.font_cdr, Font_CDR);
	FontData_Load(&stage.font_bold, Font_Bold);

	//Load characters
	Stage_LoadPlayer();
	Stage_LoadPlayer2();
	Stage_LoadOpponent();
	Stage_LoadOpponent2();
	Stage_LoadGirlfriend();
	
	//Load stage chart
	Stage_LoadChart();
	
	char note_text[32];
	
	sprintf(note_text, "\\STAGE\\NOTE%dK.TIM;1", stage.keys);
	Gfx_LoadTex(&stage.tex_note, IO_Read(note_text), GFX_LOADTEX_FREE);
	
	//Initialize stage state
	stage.story = story;
	
	Stage_LoadState();
	
	//Initialize camera
	if (stage.cur_section->flag & SECTION_FLAG_OPPFOCUS)
		Stage_FocusCharacter(stage.opponent);
	else
		Stage_FocusCharacter(stage.player);

	//Reset Rotations
	stage.camera.ta = 0;
	stage.camera.hudta = 0;
	
	//Initialize Camera
	stage.camera.speed = FIXED_DEC(5,100);
	stage.camera.force = false;
	
	stage.camera.x = stage.camera.tx;
	stage.camera.y = stage.camera.ty;
	stage.camera.zoom = stage.camera.tz;
	stage.camera.angle = stage.camera.ta;
	stage.camera.hudangle = stage.camera.hudta;
	
	stage.camera.offset.x = stage.stage_def->offset_x;
	stage.camera.offset.y = stage.stage_def->offset_y;
	stage.camera.offset.zoom = stage.stage_def->offset_zoom;
	
	stage.bump = FIXED_UNIT;
	stage.sbump = FIXED_UNIT;
	
	//Initialize stage according to mode
	stage.note_swap = (stage.prefs.mode == StageMode_Swap) ? 4 : 0;
	
	//Load music
	stage.note_scroll = 0;
	Stage_LoadMusic();
	
	//Test offset
	stage.offset = 0;
}

void Stage_Unload(void)
{
	//Disable net mode to not break the game
	if (stage.prefs.mode >= StageMode_Net1)
		stage.prefs.mode = StageMode_Normal;
	
	//Unload stage background
	if (stage.back != NULL)
		stage.back->free(stage.back);
	stage.back = NULL;
	
	//Unload stage data
	Mem_Free(stage.chart_data);
	stage.chart_data = NULL;
	
	//Free objects
	ObjectList_Free(&stage.objlist_splash);
	ObjectList_Free(&stage.objlist_fg);
	ObjectList_Free(&stage.objlist_bg);
	
	//Free characters
	Character_Free(stage.player);
	stage.player = NULL;
	Character_Free(stage.player2);
	stage.player2 = NULL;
	Character_Free(stage.opponent);
	stage.opponent = NULL;
	Character_Free(stage.opponent2);
	stage.opponent2 = NULL;
	Character_Free(stage.gf);
	stage.gf = NULL;
}

static boolean Stage_NextLoad(void)
{
	CheckNewScore();

	u8 load = stage.stage_def->next_load;
	if (load == 0)
	{
		//Do stage transition if full reload
		stage.trans = StageTrans_NextSong;
		Trans_Start();
		return false;
	}
	else
	{
		//Get stage definition
		stage.stage_def = &stage_defs[stage.stage_id = stage.stage_def->next_stage];
		
		//Load stage background
		if (load & STAGE_LOAD_STAGE)
			Stage_LoadStage();
		
		//Load characters
		if (load & STAGE_LOAD_PLAYER)
		{
			Stage_LoadPlayer();
		}
		else
		{
			stage.player->x = stage.stage_def->pchar.x;
			stage.player->y = stage.stage_def->pchar.y;
		}
		if (load & STAGE_LOAD_PLAYER2)
		{
			Stage_LoadPlayer2();
		}
		else if (stage.player2 != NULL)
		{
			stage.player2->x = stage.stage_def->pchar2.x;
			stage.player2->y = stage.stage_def->pchar2.y;
		}
		if (load & STAGE_LOAD_OPPONENT)
		{
			Stage_LoadOpponent();
		}
		else
		{
			stage.opponent->x = stage.stage_def->ochar.x;
			stage.opponent->y = stage.stage_def->ochar.y;
		}
		if (load & STAGE_LOAD_OPPONENT2)
		{
			Stage_LoadOpponent2();
		}
		else if (stage.opponent2 != NULL)
		{
			stage.opponent2->x = stage.stage_def->ochar2.x;
			stage.opponent2->y = stage.stage_def->ochar2.y;
		}
		if (load & STAGE_LOAD_GIRLFRIEND)
		{
			Stage_LoadGirlfriend();
		}
		else if (stage.gf != NULL)
		{
			stage.gf->x = stage.stage_def->gchar.x;
			stage.gf->y = stage.stage_def->gchar.y;
		}
		
		//Load stage chart
		Stage_LoadChart();
		
		//Initialize stage state
		Stage_LoadState();
		
		//Load music
		Stage_LoadMusic();
		
		//Reset timer
		Timer_Reset();
		return true;
	}
}

static int deadtimer;
static boolean inctimer;

void Stage_Tick(void)
{
	SeamLoad:;
	
	//Tick transition
	//Return to menu when start is pressed

	if (pad_state.press & (PAD_START | PAD_CROSS) && stage.state != StageState_Play)
	{
		if (deadtimer == 0)
		{
			inctimer = true;
			Audio_StopXA();
		}
	}
	else if (pad_state.press & PAD_CIRCLE && stage.state != StageState_Play)
	{
		stage.trans = StageTrans_Menu;
		Trans_Start();
	}

	if (inctimer)
		deadtimer ++;

	if (deadtimer == 200 && stage.state != StageState_Play)
	{
		stage.trans = StageTrans_Reload;
		Trans_Start();
	}

	if (Trans_Tick())
	{
		stage.paused = false;
		switch (stage.trans)
		{
			case StageTrans_Menu:
				//Load appropriate menu
				Stage_Unload();
				
				LoadScr_Start();
		
				if (stage.story)
					Menu_Load(MenuPage_Story);
				else
					Menu_Load(MenuPage_Freeplay);
				
				LoadScr_End();
				
				gameloop = GameLoop_Menu;
				return;
			case StageTrans_NextSong:
				//Load next song
				Stage_Unload();
				
				LoadScr_Start();
				Stage_Load(stage.stage_def->next_stage, stage.stage_diff, stage.story);
				LoadScr_End();
				break;
			case StageTrans_Reload:
				//Reload song
				Stage_Unload();
				
				LoadScr_Start();
				Stage_Load(stage.stage_id, stage.stage_diff, stage.story);
				LoadScr_End();
				break;
		}
	}
	
	switch (stage.state)
	{
		case StageState_Play:
		{ 
			if (stage.song_step >= 0)
			{
				if (stage.paused == false && pad_state.press & PAD_START)
				{
					stage.pause_scroll = -1;
					Audio_PauseXA();
					stage.paused = true;
					pad_state.press = 0;
				}
			}

			if (stage.paused)
			{
				switch (stage.pause_state)
				{
					case 0:
						PausedState();
						break;
					case 1:
						OptionsState();
						break;
				}
			}
			
			if (stage.prefs.debug)
				Debug_Tick();
			
			//FntPrint("step %d, beat %d", stage.song_step, stage.song_beat);

			if (noteshake) 
			{
				stage.noteshakex = RandomRange(FIXED_DEC(-5,1),FIXED_DEC(5,1));
				stage.noteshakey = RandomRange(FIXED_DEC(-5,1),FIXED_DEC(5,1));
			}
			else
			{
				stage.noteshakex = 0;
				stage.noteshakey = 0;
			}

			//Clear per-frame flags
			stage.flag &= ~(STAGE_FLAG_JUST_STEP | STAGE_FLAG_SCORE_REFRESH);
			
			//Get song position
			boolean playing;
			fixed_t next_scroll;
			
			const fixed_t interp_int = FIXED_UNIT * 8 / 75;

			if (!stage.paused)
			{
				if (stage.note_scroll < 0)
				{
					stage.song_time += timer_dt;
						
					//Update song
					if (stage.song_time >= 0)
					{
						//Song has started
						playing = true;

						Audio_PlayXA_Track(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, 0);
							
						//Update song time
						fixed_t audio_time = (fixed_t)Audio_TellXA_Milli() - stage.offset;
						if (audio_time < 0)
							audio_time = 0;
						stage.interp_ms = (audio_time << FIXED_SHIFT) / 1000;
						stage.interp_time = 0;
						stage.song_time = stage.interp_ms;
					}
					else
					{
						//Still scrolling
						playing = false;
					}
					
					//Update scroll
					next_scroll = FIXED_MUL(stage.song_time, stage.step_crochet);
				}
				else if (Audio_PlayingXA())
				{
					fixed_t audio_time_pof = (fixed_t)Audio_TellXA_Milli();
					fixed_t audio_time = (audio_time_pof > 0) ? (audio_time_pof - stage.offset) : 0;
					
					if (stage.prefs.expsync)
					{
						//Get playing song position
						if (audio_time_pof > 0)
						{
							stage.song_time += timer_dt;
							stage.interp_time += timer_dt;
						}
						
						if (stage.interp_time >= interp_int)
						{
							//Update interp state
							while (stage.interp_time >= interp_int)
								stage.interp_time -= interp_int;
							stage.interp_ms = (audio_time << FIXED_SHIFT) / 1000;
						}
						
						//Resync
						fixed_t next_time = stage.interp_ms + stage.interp_time;
						if (stage.song_time >= next_time + FIXED_DEC(25,1000) || stage.song_time <= next_time - FIXED_DEC(25,1000))
						{
							stage.song_time = next_time;
						}
						else
						{
							if (stage.song_time < next_time - FIXED_DEC(1,1000))
								stage.song_time += FIXED_DEC(1,1000);
							if (stage.song_time > next_time + FIXED_DEC(1,1000))
								stage.song_time -= FIXED_DEC(1,1000);
						}
					}
					else
					{
						//Old sync
						stage.interp_ms = (audio_time << FIXED_SHIFT) / 1000;
						stage.interp_time = 0;
						stage.song_time = stage.interp_ms;
					}
					
					playing = true;
					
					//Update scroll
					next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);
				}
				else
				{
					//Song has ended
					playing = false;
					stage.song_time += timer_dt;
						
					//Update scroll
					next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);
					
					//Transition to menu or next song
					if (stage.story && stage.stage_def->next_stage != stage.stage_id)
					{
						if (Stage_NextLoad())
							goto SeamLoad;
					}
					else
					{
						CheckNewScore();
						stage.trans = StageTrans_Menu;
						Trans_Start();
					}
				}	
				RecalcScroll:;
				//Update song scroll and step
				if (next_scroll > stage.note_scroll)
				{
					if (((stage.note_scroll / 12) & FIXED_UAND) != ((next_scroll / 12) & FIXED_UAND))
						stage.flag |= STAGE_FLAG_JUST_STEP;
					stage.note_scroll = next_scroll;
					stage.song_step = (stage.note_scroll >> FIXED_SHIFT);
					if (stage.note_scroll < 0)
						stage.song_step -= 11;
					stage.song_step /= 12;
					stage.song_beat = stage.song_step / 4;
				}
				
				//Update section
				if (stage.note_scroll >= 0)
				{
					//Check if current section has ended
					u16 end = stage.cur_section->end;
					if ((stage.note_scroll >> FIXED_SHIFT) >= end)
					{
						//Increment section pointer
						stage.cur_section++;
						
						//Update BPM
						u16 next_bpm = stage.cur_section->flag & SECTION_FLAG_BPM_MASK;
						Stage_ChangeBPM(next_bpm, end);
						stage.section_base = stage.cur_section;
						
						//Recalculate scroll based off new BPM
						next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);
						goto RecalcScroll;
					}
				}
			}

			//Play CountDown
			if (stage.song_step < 0)
				Stage_CountDown();
			
			Events();
			
			//Handle bump
			if ((stage.bump = FIXED_UNIT + FIXED_MUL(stage.bump - FIXED_UNIT, FIXED_DEC(95,100))) <= FIXED_DEC(1003,1000))
				stage.bump = FIXED_UNIT;
			stage.sbump = FIXED_UNIT + FIXED_MUL(stage.sbump - FIXED_UNIT, FIXED_DEC(60,100));
			
			if (playing && (stage.flag & STAGE_FLAG_JUST_STEP))
			{
				//Check if screen should bump
				boolean is_bump_step = (stage.song_step & 0xF) == 0;
				
				//Bump screen
				if (is_bump_step)
					stage.bump = FIXED_DEC(103,100);

				//Bump health every 4 steps
				if ((stage.song_step & 0x3) == 0)
					stage.sbump = FIXED_DEC(103,100);
			}
			
			//Scroll camera
			if (stage.cur_section->flag & SECTION_FLAG_OPPFOCUS)
				Stage_FocusCharacter(stage.opponent);
			else
				Stage_FocusCharacter(stage.player);
			Stage_ScrollCamera();

			//Draw Timer
			if (stage.prefs.songtimer)
				Stage_TimerTick();

			if (stage.prefs.botplay)
			{
				//Draw skill issue mode (botplay)
				RECT skill_issue_src = {129, 208, 67, 16};
				RECT_FIXED skill_issue_dst = {
				FIXED_DEC(-33,1), 
				FIXED_DEC(-60,1), 
				FIXED_DEC(67,1), 
				FIXED_DEC(16,1)
			};

				skill_issue_dst.y += stage.noteshakey;
				skill_issue_dst.x += stage.noteshakex;
				
				if (!stage.prefs.debug)
					Stage_DrawTex(&stage.tex_hude, &skill_issue_src, &skill_issue_dst, stage.bump, stage.camera.hudangle);
			}
			
			//Draw score
			for (int i = 0; i < ((stage.prefs.mode == StageMode_2P) ? 2 : 1); i++)
			{
				PlayerState *this = &stage.player_state[i];
						
				//Get string representing number
				if (this->refresh_score)
				{
					if (this->score != 0)
						sprintf(this->score_text, "Score: %d", this->score * 10);
					else
						strcpy(this->score_text, "Score: ?");
					this->refresh_score = false;
				}
							
					//Draw text
					stage.font_cdr.draw(&stage.font_cdr,
						this->score_text,
						(stage.prefs.mode == StageMode_2P && i == 0) ? FIXED_DEC(10,1) : FIXED_DEC(-130,1), 
						(stage.prefs.downscroll) ? FIXED_DEC(-88,1) : FIXED_DEC(98,1),
						FontAlign_Left
					);
				}

			//Draw Miss
			for (u8 i = 0; i < ((stage.prefs.mode == StageMode_2P) ? 2 : 1); i++)
			{
				PlayerState *this = &stage.player_state[i];
						
				//Get string representing number
				if (this->refresh_miss)
				{
					if (this->miss != 0)
						sprintf(this->miss_text, "Misses: %d", this->miss);
					else
						strcpy(this->miss_text, "Misses: 0");
					this->refresh_miss = false;
				}
							
					//Draw text
					stage.font_cdr.draw(&stage.font_cdr,
						this->miss_text,
						(stage.prefs.mode == StageMode_2P && i == 0) ? FIXED_DEC(90,1) : FIXED_DEC(-50,1), 
						(stage.prefs.downscroll) ? FIXED_DEC(-88,1) : FIXED_DEC(98,1),
						FontAlign_Left
					);
				}

			//Draw accuracy
			for (u8 i = 0; i < ((stage.prefs.mode == StageMode_2P) ? 0 : 1); i++)
			{
				PlayerState *this = &stage.player_state[i];

				static const char *rating_text[] = {
				"F", // 0 - 9%
				"F", //10 - 19% repeating this because it's more easy LOL
				"D", //20 - 29%
				"D", //30 - 39% repeating this because it's more easy LOL
				"C", //40 - 49%
				"C", //50 - 59%
				"B", //60 - 69%
				"B", //70 - 79%
				"A", //80 - 89%
				"A", //90 - 99%
				"S", //100%
			};

				static const char *fc_text[] = {
				"- FC", // 0 - 79%
				"- GFC", //80 - 99%
				"- PFC", //100%
			};

				this->accuracy = (this->min_accuracy * 100) / (this->max_accuracy);

				//making this for in the case of special ratings,like Nice!
				static u8 rating;

				switch (this->accuracy)
				{
					default:
						rating = this->accuracy/10;
					break;
				}

				//fc rating
				static u8 fc;

				if (this->accuracy <= 79)
					fc = 0;

				else if (this->accuracy <= 99)
					fc = 1;

				else
					fc = 2;
						
				//Get string representing number
				if (this->refresh_accuracy)
				{
					if (this->accuracy != 0)
						sprintf(this->accuracy_text, "Accuracy: (%d%%) %s %s", this->accuracy, rating_text[rating], (this->miss == 0) ? fc_text[fc] : '\0');
					else
						strcpy(this->accuracy_text, "Accuracy: ?");
					this->refresh_accuracy = false;
				}
							
					//Draw text
					stage.font_cdr.draw(&stage.font_cdr,
						this->accuracy_text,
						FIXED_DEC(15,1), 
						(stage.prefs.downscroll) ? FIXED_DEC(-87,1) : FIXED_DEC(98,1),
						FontAlign_Left
					);
			}
			
			switch (stage.prefs.mode)
			{
				case StageMode_Normal:
				case StageMode_Swap:
				{
					//Handle player 1 inputs
					Stage_ProcessPlayer(&stage.player_state[0], &pad_state, playing);
					
					//Handle opponent notes
					u8 opponent_anote = CharAnim_Idle;
					u8 opponent_snote = CharAnim_Idle;
					
					for (Note *note = stage.cur_note;; note++)
					{
						if (note->pos > (stage.note_scroll >> FIXED_SHIFT))
							break;
						
						//Opponent note hits
						if (playing && !(note->type & NOTE_FLAG_HIT) && note->is_opponent)
						{
							//Opponent hits note
							Stage_StartVocal();
							if (note->type & NOTE_FLAG_SUSTAIN)
								opponent_snote = note_anims[Stage_GetNoteType(note) % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0];
							else
								opponent_anote = note_anims[Stage_GetNoteType(note) % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0];
							note->type |= NOTE_FLAG_HIT;
						}
					}
					
					if (opponent_anote != CharAnim_Idle)
						stage.player_state[1].character->set_anim(stage.player_state[1].character, opponent_anote);
					else if (opponent_snote != CharAnim_Idle)
						stage.player_state[1].character->set_anim(stage.player_state[1].character, opponent_snote);
					break;
				}
				case StageMode_2P:
				{
					//Handle player 1 and 2 inputs
					Stage_ProcessPlayer(&stage.player_state[0], &pad_state, playing);
					Stage_ProcessPlayer(&stage.player_state[1], &pad_state_2, playing);
					break;
				}
			}
			
			if (!stage.prefs.debug)
			{
				if (stage.prefs.mode < StageMode_2P)
				{
					//Perform health checks
					if (stage.player_state[0].health <= 0)
					{
						//Player has died
						stage.player_state[0].health = 0;
							
						stage.state = StageState_Dead;
					}
					if (stage.player_state[0].health > 20000)
						stage.player_state[0].health = 20000;

					//Draw health heads
					Stage_DrawHealth(stage.player_state[0].health, stage.player_state[0].character->health_i,    1);
					Stage_DrawHealth(stage.player_state[0].health, stage.player_state[1].character->health_i, -1);
					
					//Draw health bar
					Stage_DrawHealthBar(251 - (251 * stage.player_state[0].health / 20000), stage.player_state[1].character->health_bar);
					Stage_DrawHealthBar(251, stage.player_state[0].character->health_bar);
				}
			
				//Tick note splashes
				ObjectList_Tick(&stage.objlist_splash);
				
				//Draw stage notes
				Stage_DrawNotes();
				
				//Draw note HUD
				RECT note_src = {0, 0, stage.note.size, stage.note.size};
				RECT_FIXED note_dst = {0, stage.note.y - FIXED_DEC(stage.note.size / 2,1), FIXED_DEC(stage.note.size,1), FIXED_DEC(stage.note.size,1)};
				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				
				for (u8 i = 0; i < stage.keys; i++)
				{
					//BF
					note_dst.x = stage.note.x[i] - FIXED_DEC(stage.note.size / 2,1);
					Stage_DrawStrum(i, &note_src, &note_dst);
					Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
					
					//Opponent
					note_dst.x = stage.note.x[(i + stage.keys)] - FIXED_DEC(stage.note.size / 2,1);
					Stage_DrawStrum(i + stage.keys, &note_src, &note_dst);
					Stage_DrawTex(&stage.tex_note, &note_src, &note_dst, stage.bump, stage.camera.hudangle);
				}
			}
			
			Events_Back();
			
			//Draw stage foreground
			if (stage.back->draw_fg != NULL)
				stage.back->draw_fg(stage.back);
			
			//Tick foreground objects
			ObjectList_Tick(&stage.objlist_fg);
			
			//Tick characters
			if (stage.prefs.mode == StageMode_Swap)
			{
				stage.opponent->tick(stage.opponent);
				stage.player->tick(stage.player);
			}
			else
			{
				stage.player->tick(stage.player);
				stage.opponent->tick(stage.opponent);
			}
      		if (stage.player2 != NULL)
				stage.player2->tick(stage.player2);
			if (stage.opponent2 != NULL)
				stage.opponent2->tick(stage.opponent2);
			
			//Draw stage middle
			if (stage.back->draw_md != NULL)
				stage.back->draw_md(stage.back);
			
			//Tick girlfriend
			if (stage.gf != NULL)
				stage.gf->tick(stage.gf);
			
			//Tick background objects
			ObjectList_Tick(&stage.objlist_bg);
			
			//Draw stage background
			if (stage.back->draw_bg != NULL)
				stage.back->draw_bg(stage.back);
			break;
		}
		case StageState_Dead: //Start BREAK animation and reading extra data from CD
		{
			//Stop music immediately
			Audio_StopXA();
			deadtimer = 0;
			inctimer = false;
			
			//Unload stage data
			Mem_Free(stage.chart_data);
			stage.chart_data = NULL;
			
			//Free background
			stage.back->free(stage.back);
			stage.back = NULL;
			
			//Free objects
			ObjectList_Free(&stage.objlist_fg);
			ObjectList_Free(&stage.objlist_bg);
			
			//Free opponent and girlfriend
			Character_Free(stage.player2);
			stage.player2 = NULL;
			Character_Free(stage.opponent);
			stage.opponent = NULL;
			Character_Free(stage.opponent2);
			stage.opponent2 = NULL;
			Character_Free(stage.gf);
			stage.gf = NULL;
			
			//Reset stage state
			stage.flag = 0;
			stage.bump = stage.sbump = FIXED_UNIT;
			
			//Change background colour to black
			Gfx_SetClear(0, 0, 0);
			
			//Run death animation, focus on player, and change state
			stage.player->set_anim(stage.player, PlayerAnim_Dead0);
			
			Stage_FocusCharacter(stage.player);
			stage.song_time = 0;
			
			stage.state = StageState_DeadLoad;
		}
	//Fallthrough
		case StageState_DeadLoad:
		{
			//Scroll camera and tick player
			if (stage.song_time < FIXED_UNIT)
				stage.song_time += FIXED_UNIT / 60;
			Stage_ScrollCamera();
			stage.player->tick(stage.player);
			
			//Drop mic and change state if CD has finished reading and animation has ended
			if (IO_IsReading() || stage.player->animatable.anim != PlayerAnim_Dead1)
				break;
			
			stage.player->set_anim(stage.player, PlayerAnim_Dead2);
			stage.state = StageState_DeadDrop;
			break;
		}
		case StageState_DeadDrop:
		{
			//Scroll camera and tick player
			Stage_ScrollCamera();
			stage.player->tick(stage.player);
			
			//Enter next state once mic has been dropped
			if (stage.player->animatable.anim == PlayerAnim_Dead3)
			{
				stage.state = StageState_DeadRetry;
				Audio_PlayXA_Track(XA_GameOver, 0x40, 1, true);
			}
			break;
		}
		case StageState_DeadRetry:
		{
			//Randomly twitch
			if (stage.player->animatable.anim == PlayerAnim_Dead3)
			{
				if (RandomRange(0, 29) == 0)
					stage.player->set_anim(stage.player, PlayerAnim_Dead4);
				if (RandomRange(0, 29) == 0)
					stage.player->set_anim(stage.player, PlayerAnim_Dead5);
			}
			
			//Scroll camera and tick player
			Stage_ScrollCamera();
			stage.player->tick(stage.player);
			break;
		}
		default:
			break;
	}
}