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
#include "str.h"

#include "menu.h"
#include "pause.h"
#include "trans.h"
#include "loadscr.h"

#include "object/combo.h"
#include "object/splash.h"

#include "disc_swap_disc1.h"
#include "disc_swap_disc2.h"
#include "disc_swap_disc3.h"

//Stage constants
//#define STAGE_NOHUD //Disable the HUD

int hudEnabled = 1;
int noteEnabled = 1;

//4K
int note_x4k_normal[8] = {
	// BF - normal
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(60,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(94,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(128,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - normal
	FIXED_DEC(-128,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-94,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(-60,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x4k_flipped[8] = {
	// BF - flipped
	FIXED_DEC(128,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(94,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(60,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - flipped
	FIXED_DEC(-26,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-60,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(-94,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-128,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x4k_silly_billy[8] = {
	// BF - silly billy opponent
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(60,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(94,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(128,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent
	FIXED_DEC(-128+50,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-94+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(-60+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x4k_silly_billy_middle[8] = {
	// BF - silly billy opponent + player middle scroll
	FIXED_DEC(26-68,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(60-68,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(94-68,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(128-68,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent + player middle scroll
	FIXED_DEC(-128+50,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-94+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(-60+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

static u16 note_key4k[] = {INPUT_LEFT, INPUT_DOWN, INPUT_UP, INPUT_RIGHT};

static u8 note_anims4k[4][3] = {
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

int note_x5k_normal[10] = {
	// BF - normal
	FIXED_DEC(16-10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(50-10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(84-10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(118-10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(150-10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - normal
	FIXED_DEC(-128-10,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-94-10,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(-512,1)    - FIXED_DEC(SCREEN_WIDEADD,4),// special case
	FIXED_DEC(-60-10,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26-10,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x5k_flipped[10] = {
	//BF
	 FIXED_DEC(-150 + 10,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-118 + 10,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(-84 + 10,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-50 + 10,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(-16 + 10,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	//Opponent
	 FIXED_DEC(26 + 10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(60 + 10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(512,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	 FIXED_DEC(94 + 10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	 FIXED_DEC(128 + 10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x5k_silly_billy[10] = {
	// BF - silly billy opponent
	FIXED_DEC(16-10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(50-10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC(84-10,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(118-10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(150-10,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent
	FIXED_DEC((-128-10)+50,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((-94-10)+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC((-512)+50,1)    - FIXED_DEC(SCREEN_WIDEADD,4),// special case
	FIXED_DEC((-60-10)+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((-26-10)+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x5k_silly_billy_middle[10] = {
	// BF - silly billy opponent + player middle scroll
	FIXED_DEC((16-10)-68,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((50-10)-68,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC((84-10)-68,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((118-10)-68,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((150-10)-68,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent + player middle scroll
	FIXED_DEC((-128-10)+50,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((-94-10)+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+34
	FIXED_DEC((-512)+50,1)    - FIXED_DEC(SCREEN_WIDEADD,4),// special case
	FIXED_DEC((-60-10)+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC((-26-10)+50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

static u16 note_key5k[] = {INPUT_LEFT5K, INPUT_DOWN5K, INPUT_MIDDLE, INPUT_UP5K, INPUT_RIGHT5K};

static u8 note_anims5k[5][3] = {
	{CharAnim_Left,  CharAnim_LeftAlt,  PlayerAnim_LeftMiss},
	{CharAnim_Down,  CharAnim_DownAlt,  PlayerAnim_DownMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Up,    CharAnim_UpAlt,    PlayerAnim_UpMiss},
	{CharAnim_Right, CharAnim_RightAlt, PlayerAnim_RightMiss},
};

//6K
int note_x6k_normal[12] = {
	// BF - normal
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(50,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(74,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(122,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - normal
	FIXED_DEC(-146,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-122,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-74,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(-50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x6k_flipped[12] = {
	// BF - flipped
	FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(122,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(98,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(74,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(50,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - flipped
	FIXED_DEC(-26,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-50,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-74,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(-122,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-146,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x6k_silly_billy[12] = {
	// BF - silly billy opponent
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(50,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(74,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(122,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent
	FIXED_DEC(-146+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-122+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-74+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(-50+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x6k_silly_billy_middle[12] = {
	// BF - silly billy opponent + player middle scroll
	FIXED_DEC(26-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(50-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(74-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(122-42,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(146-42,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent + player middle scroll
	FIXED_DEC(-146+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-122+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-74+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+24
	FIXED_DEC(-50+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
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
int note_x7k_normal[14] = {
	// BF - normal
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(46,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(66,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(86,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(106,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(126,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - normal
	FIXED_DEC(-146,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-126,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-106,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-86,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-66,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(-46,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x7k_flipped[14] = {
	// BF - flipped
	FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(126,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(106,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(86,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(66,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(46,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - flipped
	FIXED_DEC(-26,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-46,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-66,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-86,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-106,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(-126,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-146,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x7k_silly_billy[14] = {
	// BF - silly billy opponent
	FIXED_DEC(26,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(46,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(66,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(86,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(106,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(126,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(146,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent
	FIXED_DEC(-146+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-126+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-106+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-86+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-66+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(-46+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x7k_silly_billy_middle[14] = {
	// BF - silly billy opponent + player middle scroll
	FIXED_DEC(26-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(46-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(66-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(86-42,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(106-42,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(126-42,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(146-42,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent + player middle scroll
	FIXED_DEC(-146+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-126+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-106+38,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-86+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-66+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+20
	FIXED_DEC(-46+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-26+38,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
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
int note_x9k_normal[18] = {
	// BF - normal
	FIXED_DEC(13,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(30,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(47,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(64,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(81,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(115,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(132,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(149,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - normal
	FIXED_DEC(-149,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-132,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(-115,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-81,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-64,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-47,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-30,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-13,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x9k_flipped[18] = {
	// BF - flipped
	FIXED_DEC(149,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(132,1) + FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(115,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(81,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(64,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(47,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(30,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(13,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - flipped
	FIXED_DEC(-13,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-30,1)  - FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(-47,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-64,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-81,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-115,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-132,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-149,1) - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x9k_silly_billy[18] = {
	// BF - silly billy opponent
	FIXED_DEC(13,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(30,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(47,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(64,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(81,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(115,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(132,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(149,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent
	FIXED_DEC(-149+9,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-132+9,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(-115+9,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-81+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-64+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-47+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-30+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-13+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
};

int note_x9k_silly_billy_middle[18] = {
	// BF - silly billy opponent + player middle scroll
	FIXED_DEC(13-11,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(30-11,1)  + FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(47-11,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(64-11,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(81-11,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(98-11,1)  + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(115-11,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(132-11,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(149-11,1) + FIXED_DEC(SCREEN_WIDEADD,4),
	// Opponent - silly billy opponent + player middle scroll
	FIXED_DEC(-149+9,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-132+9,1) - FIXED_DEC(SCREEN_WIDEADD,4),//+17
	FIXED_DEC(-115+9,1) - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-98+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-81+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-64+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-47+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-30+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
	FIXED_DEC(-13+9,1)  - FIXED_DEC(SCREEN_WIDEADD,4),
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
int bgx = 256;

static u32 Sounds[7];

#include "character/bf.h"
#include "character/apple.h"
#include "character/bfy.h"

#include "character/dad.h"
#include "character/spook.h"
#include "character/monster.h"
#include "character/pico.h"
#include "character/exep3.h"
#include "character/orange.h"
#include "character/sillybilly.h"
#include "character/jerry.h"

#include "character/gf.h"
#include "character/logan.h"

#include "stage/dummy.h"
#include "stage/week1.h"
#include "stage/week2.h"
#include "stage/week3.h"
#include "stage/trio.h"
#include "stage/kitchen.h"
#include "stage/sillybg.h"

static const StageDef stage_defs[StageId_Max] = {
	#include "stagedef_disc1.h"
	#include "stagedef_disc2.h"
	#include "stagedef_disc3.h"
};

//Stage states
Stage stage;
Debug debug;

int drain = 0;

//Stage note functions
static u16 Stage_GetNoteType(Note* note)
{
	u16 note_xor = 0;
	u16 note_type = note->type;
	
	static u16 note_types[] = {NOTE_FLAG_SUSTAIN, NOTE_FLAG_SUSTAIN_END, NOTE_FLAG_ALT_ANIM, NOTE_FLAG_MINE, NOTE_FLAG_DANGER, NOTE_FLAG_STATIC, NOTE_FLAG_PHANTOM, NOTE_FLAG_POLICE, NOTE_FLAG_MAGIC, NOTE_FLAG_HIT};
	
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
	if (ch != NULL) {
		stage.camera.tx = ch->x + ch->focus_x + ((stage.state == StageState_Play) ? stage.camera.offset.x : 0);
		stage.camera.ty = ch->y + ch->focus_y + ((stage.state == StageState_Play) ? stage.camera.offset.y : 0);
		stage.camera.tz = FIXED_MUL(ch->focus_zoom, event.zoom);
	}
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

static void Stage_DrawStartScreen(void)
{
	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT text_src = {0, 0, 127, 47};
	RECT_FIXED text_dst = {
		FIXED_DEC(bgx,1) - fx,
		FIXED_DEC(-64,1) - fy,
		FIXED_DEC(256,1) + FIXED_DEC(SCREEN_WIDEOADD,1),
		FIXED_DEC(94,1)
	};

	Stage_DrawTex(&stage.tex_strscr, &text_src, &text_dst, stage.camera.bzoom, stage.camera.angle);

	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT logo_src = {0, 48, 128, 94};
	RECT_FIXED logo_dst = {
		FIXED_DEC(-64,1) - FIXED_DEC(SCREEN_WIDEOADD,2) - fx,
		FIXED_DEC(-64,1) - fy,
		FIXED_DEC(128,1) + FIXED_DEC(SCREEN_WIDEOADD,1),
		FIXED_DEC(94,1)
	};
	Stage_DrawTex(&stage.tex_strscr, &logo_src, &logo_dst, stage.camera.bzoom, stage.camera.angle);

	u8 st_col = 255;
	static const RECT st = {0, 0, 500, 500};
	stage.startscreen -= 2;
	if(stage.startscreen < 256)
		st_col = stage.startscreen;
	Gfx_BlendRect(&st, st_col, st_col, st_col, 2);

	if (stage.song_step == -30)
	{
		bgx = 192;
	}
	if (stage.song_step >= -22)
	{
		bgx -= 16;

		if (bgx <= -192)
			bgx = -192;
	}
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
			//Create splash object - determine background based on player's hud field
			boolean splash_background = !this->hud; // If hud is true, splash is foreground; if hud is false, splash is background
			Obj_Splash *splash = Obj_Splash_New(
				stage.note.x[type],
				stage.note.y[type] * (stage.prefs.downscroll ? -1 : 1),
				type % stage.keys,
				splash_background
			);
			if (splash != NULL)
			{
				//Add to appropriate list based on background/foreground
				if (splash_background)
					ObjectList_Add(&stage.objlist_bg, (Object*)splash);
				else
					ObjectList_Add(&stage.objlist_splash, (Object*)splash);
			}
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

static void Static_tick()
{
	if (stage.hitstatic > 0)
	{
		RECT src = { 0, 0, 256, 256 };
		RECT dst = {
			0,
			stage.hitstatic * 10,
			330,
			250
		};
		if (stage.hitstatic > 7)
			Gfx_BlendTex(&stage.tex_static, &src, &dst, 10);
		else if (stage.hitstatic > 4)
			Gfx_DrawTex(&stage.tex_static, &src, &dst);
		else if (stage.hitstatic > 1)
			Gfx_BlendTex(&stage.tex_static, &src, &dst, 10);

		RECT src2 = { 0, 0, 256, 256 };
		RECT dst2 = {
			0,
			-248 + stage.hitstatic * 10,
			330,
			250
		};
		if (stage.hitstatic > 7)
			Gfx_BlendTex(&stage.tex_static, &src2, &dst2, 10);
		else if (stage.hitstatic > 4)
			Gfx_DrawTex(&stage.tex_static, &src2, &dst2);
		else if (stage.hitstatic > 1)
			Gfx_BlendTex(&stage.tex_static, &src2, &dst2, 10);

		stage.hitstatic -= 1;
	}
}

static void Stage_NoteCheck(PlayerState *this, u8 type)
{
	//Perform note check
	for (Note *note = stage.cur_note;; note++)
	{
		if (!(note->type & (NOTE_FLAG_MINE | NOTE_FLAG_DANGER | NOTE_FLAG_STATIC | NOTE_FLAG_PHANTOM | NOTE_FLAG_POLICE | NOTE_FLAG_MAGIC)))
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
		else if (note->type & (NOTE_FLAG_DANGER))
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
				this->health += 230;

			if (this->character->spec & CHAR_SPEC_MISSANIM)
				this->character->set_anim(this->character, note_anims[type % stage.keys][2]);
			else
				this->character->set_anim(this->character, note_anims[type % stage.keys][0]);
			this->arrow_hitan[type % stage.keys] = -1;
			
			return;
		}
		else if (note->type & (NOTE_FLAG_STATIC))
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
				this->health += 230;

			if (this->character->spec & CHAR_SPEC_MISSANIM)
				this->character->set_anim(this->character, note_anims[type % stage.keys][2]);
			else
				this->character->set_anim(this->character, note_anims[type % stage.keys][0]);
			this->arrow_hitan[type % stage.keys] = -1;
			
			return;
		}
		else if (note->type & (NOTE_FLAG_PHANTOM))
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
	
			drain += 400;

			if (this->character->spec & CHAR_SPEC_MISSANIM)
				this->character->set_anim(this->character, note_anims[type % stage.keys][2]);
			else
				this->character->set_anim(this->character, note_anims[type % stage.keys][0]);
			this->arrow_hitan[type % stage.keys] = -1;
			
			return;
		}
		else if (note->type & (NOTE_FLAG_POLICE))
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
		else if (note->type & (NOTE_FLAG_MAGIC))
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
	
			stage.camera.ta += RandomRange(45, 270);
			stage.camera.hudta += RandomRange(45, 270);

			if (this->character->spec & CHAR_SPEC_MISSANIM)
				this->character->set_anim(this->character, note_anims[type % stage.keys][2]);
			else
				this->character->set_anim(this->character, note_anims[type % stage.keys][0]);
			this->arrow_hitan[type % stage.keys] = -1;
			
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
	
	// Suppress miss registration briefly after swap
	if (stage.swap_grace_frames == 0)
	{
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
		
		// Hit sustain only if not in grace, otherwise allow without penalty animations
		note->type |= NOTE_FLAG_HIT;
		if (stage.swap_grace_frames == 0)
		{
			Stage_CheckAnimations(this, note_anims[type % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0]);
		}

		Stage_StartVocal();
		if (!stage.movie_is_playing)
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
			
				if ((note->type & NOTE_FLAG_MINE) || (note->type & NOTE_FLAG_PHANTOM) || (note->type & NOTE_FLAG_POLICE) || (note->type & NOTE_FLAG_MAGIC) || (stage.prefs.mode != StageMode_Swap && note->is_opponent != is_opponent) || (stage.prefs.mode == StageMode_Swap && note->is_opponent == is_opponent))
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
	
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}
    
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
    
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}
	
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

void Stage_DrawTexCol_FlipX(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation, u8 cr, u8 cg, u8 cb)
{
    fixed_t xz = dst->x + dst->w;
    fixed_t yz = dst->y;
    fixed_t wz = -dst->w;
    fixed_t hz = dst->h;
	
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}
    
    // Calculate the rotated coordinates with flipping the x-axis
    u8 rotationAngle = rotation / FIXED_UNIT;  // Specify the desired rotation angle (in degrees)
    fixed_t rotatedX = -FIXED_MUL(xz, FIXED_DEC(MUtil_Cos(rotationAngle), 256)) - FIXED_MUL(yz, FIXED_DEC(MUtil_Sin(rotationAngle), 256));
    fixed_t rotatedY = FIXED_MUL(xz, FIXED_DEC(MUtil_Sin(rotationAngle), 256)) + FIXED_MUL(yz, FIXED_DEC(MUtil_Cos(rotationAngle), 256));
    
    fixed_t l = (SCREEN_WIDTH2  * FIXED_UNIT) + FIXED_MUL(rotatedX, zoom) + FIXED_DEC(1, 2);
    fixed_t t = (SCREEN_HEIGHT2 * FIXED_UNIT) + FIXED_MUL(rotatedY, zoom) + FIXED_DEC(1, 2);
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
	
    Gfx_DrawTexRotateColFlipped(tex, src, &sdst, rotationAngle, 0, 0, cr, cg, cb);
}

void Stage_DrawTex(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation)
{
    Stage_DrawTexCol(tex, src, dst, zoom, rotation, 0x80, 0x80, 0x80);
}

void Stage_DrawTex_FlipX(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, fixed_t rotation)
{
    Stage_DrawTexCol_FlipX(tex, src, dst, zoom, rotation, 0x80, 0x80, 0x80);
}

void Stage_DrawTexArbCol(Gfx_Tex *tex, const RECT *src, const POINT_FIXED *p0, const POINT_FIXED *p1, const POINT_FIXED *p2, const POINT_FIXED *p3, u8 r, u8 g, u8 b, fixed_t zoom, fixed_t rotation)
{
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}
	
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
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}
	
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
	
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}
	
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

void Stage_BlendTexV2(Gfx_Tex *tex, const RECT *src, const RECT_FIXED *dst, fixed_t zoom, u8 mode, u8 opacity)
{
	fixed_t xz = dst->x;
	fixed_t yz = dst->y;
	fixed_t wz = dst->w;
	fixed_t hz = dst->h;

	//Don't draw if HUD and is disabled
	if (tex == &stage.tex_hud0 || tex == &stage.tex_hud1)
	{
		#ifdef STAGE_NOHUD
			return;
		#endif
		
		if (hudEnabled == 0)
			return;
	}

	fixed_t l = (SCREEN_WIDTH2  << FIXED_SHIFT) + FIXED_MUL(xz, zoom);// + FIXED_DEC(1,2);
	fixed_t t = (SCREEN_HEIGHT2 << FIXED_SHIFT) + FIXED_MUL(yz, zoom);// + FIXED_DEC(1,2);
	fixed_t r = l + FIXED_MUL(wz, zoom);
	fixed_t b = t + FIXED_MUL(hz, zoom);

	l >>= FIXED_SHIFT;
	t >>= FIXED_SHIFT;
	r >>= FIXED_SHIFT;
	b >>= FIXED_SHIFT;

	RECT sdst = {
		l,
		t,
		r - l,
		b - t,
	};
	Gfx_BlendTexV2(tex, src, &sdst, mode, opacity);
}

//Stage HUD functions
static void Stage_DrawHealth(s16 health, u16 health_i[2][4], s8 ox) 
{
    // Check if we should use 'dying' frame
	s8 dying;
	if (ox < 0)
		dying = (health >= 18000);
	else
		dying = (health <= 2000);
	
    // Get src and dst
    fixed_t hx = (128 << FIXED_SHIFT) * (10000 - health) / 10000;

    RECT src = {
        health_i[dying][0],
        health_i[dying][1],
        health_i[dying][2],
        health_i[dying][3]
    };
    RECT_FIXED dst = {
        hx + ox * FIXED_DEC(16,1) - FIXED_DEC(16,1),
		FIXED_DEC(70,1),
		src.w << FIXED_SHIFT,
		src.h << FIXED_SHIFT
    };
	if (stage.prefs.downscroll)
		dst.y = -dst.y - FIXED_DEC(44,1);
	
	if (stage.prefs.mode == StageMode_Swap)
	{
		dst.x += dst.w;
		dst.w = -dst.w;
	}

    // Draw health icon
    Stage_DrawTex(&stage.tex_hud1, &src, &dst, FIXED_MUL(stage.bump, stage.sbump), stage.camera.hudangle);
}

static void Stage_DrawOrangeHealth(s16 health, u16 health_i[2][4], s8 ox) 
{
    // Check if we should use 'dying' frame
    s8 dying;
    if (ox < 0)
        dying = (health >= 18000);
    else
        dying = (health <= 2000);
    
    // Determine screen center
    s16 center_x = (SCREEN_WIDTH - 320) >> 1;

    // Calculate destination x position based on ox
    fixed_t hx = (128 << FIXED_SHIFT) * (10000 - 10000) / 10000;

    RECT src = {
        health_i[dying][0],
        health_i[dying][1],
        health_i[dying][2],
        health_i[dying][3]
    };

    RECT_FIXED dst = {
        (ox < 0 ? center_x - FIXED_DEC(155,1) : center_x + FIXED_DEC(90,1)), // Adjust x position for opposite sides
        FIXED_DEC(44,1),
        src.w << FIXED_SHIFT,
        src.h << FIXED_SHIFT
    };

    if (stage.prefs.downscroll)
        dst.y = -dst.y - FIXED_DEC(70,1);
    
    if (stage.prefs.mode == StageMode_Swap)
    {
        dst.x += dst.w;
        dst.w = -dst.w;
    }

    // Draw health icon
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

static void Stage_DrawNote(const RECT *src, RECT_FIXED *dst, boolean hud, Gfx_Tex *texture)
{
    // Don't draw if note is disabled
    if (noteEnabled == 0)
        return;
    
    if (texture == NULL)
        texture = &stage.tex_note;
    
    RECT_FIXED sdst = {
        dst->x,
        dst->y,
        dst->w,
        dst->h
    };
    
    fixed_t zoom = stage.camera.bzoom;
    
    if (hud)
        zoom = stage.bump;
    else
    {
        sdst.x -= stage.camera.x;
        sdst.y -= stage.camera.y;
    }
    
	if (hud)
		Stage_DrawTex(texture, src, &sdst, zoom, stage.camera.hudangle);
	else
		Stage_BlendTex(texture, src, &sdst, zoom, stage.camera.hudangle, 0);
}

static void Stage_DrawPhantomNote(const RECT *src, RECT_FIXED *dst, boolean hud, Gfx_Tex *texture)
{
    // Don't draw if note is disabled
    if (noteEnabled == 0)
        return;
    
    if (texture == NULL)
        texture = &stage.tex_note;
    
    RECT_FIXED sdst = {
        dst->x,
        dst->y,
        dst->w,
        dst->h
    };
    
    fixed_t zoom = stage.camera.bzoom;
    
    if (hud)
        zoom = stage.bump;
    else
    {
        sdst.x -= stage.camera.x;
        sdst.y -= stage.camera.y;
    }
    
	if (hud)
		Stage_BlendTex(texture, src, &sdst, zoom, stage.camera.hudangle, 0);
	else
		Stage_BlendTex(texture, src, &sdst, zoom, stage.camera.hudangle, 0);
}

static void Stage_DrawHUDNotes(boolean back)
{
	RECT note_src = {0, 0, stage.note.size, stage.note.size};
	RECT_FIXED note_dst = {0, 0, FIXED_DEC(stage.note.size,1), FIXED_DEC(stage.note.size,1)};

	for (u8 i = 0; i < stage.keys; i++)
	{
		//BF
		note_dst.x = stage.note.x[i] - FIXED_DEC(stage.note.size / 2,1);
		note_dst.y = stage.note.y[i] - FIXED_DEC(stage.note.size / 2,1);
		if (stage.prefs.downscroll)
			note_dst.y = -note_dst.y - note_dst.h;
		
		Stage_DrawStrum(i, &note_src, &note_dst);
		
		if (back == stage.player_state[0].hud)
			Stage_DrawNote(&note_src, &note_dst, stage.player_state[0].hud, &stage.tex_note);
		
		//Opponent
		note_dst.x = stage.note.x[(i + stage.keys)] - FIXED_DEC(stage.note.size / 2,1);
		note_dst.y = stage.note.y[(i + stage.keys)] - FIXED_DEC(stage.note.size / 2,1);
		// Adjust note_dst up by 32 when back is true on downscroll during StageId_5_7
		if (stage.prefs.downscroll)
			note_dst.y = -note_dst.y - note_dst.h;
		
		Stage_DrawStrum(i + stage.keys, &note_src, &note_dst);
		
		if (back == stage.player_state[1].hud)
			Stage_DrawNote(&note_src, &note_dst, stage.player_state[1].hud, &stage.tex_note);
	}
}

static void Stage_DrawNotes(boolean back)
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
		fixed_t y;
		fixed_t size;
		fixed_t offset;
		
		if (!back)
		{
			y = FIXED_MUL(FIXED_DEC(5,3), time * 150);
			size = FIXED_MUL(FIXED_DEC(5,3), scroll.size) + FIXED_UNIT;
			offset = (FIXED_DEC(MUtil_Smooth(time), 256) * 32);
		}
		else if (back)
		{
			u8 note_index = Stage_GetNoteType(note) % stage.keys;
			if (note->is_opponent) {
				note_index += stage.keys;
			}
			y = stage.note.y[note_index] + FIXED_MUL(stage.speed, time * 150);
			size = FIXED_MUL(stage.speed, scroll.size) + FIXED_UNIT;
			offset = 0;
		}
		
		//Check if went above screen
		if (y < FIXED_DEC(-stage.note.size / 2 - SCREEN_HEIGHT2, 1))
		{
			//Wait for note to exit late time
			if (note_fp + stage.late_safe >= stage.note_scroll)
				continue;
			
				if ((note->type == NOTE_FLAG_DANGER) && !bot)
				{
				this->health -= 4750;
					Stage_CutVocal();
					Stage_MissNote(this);
				}

				if ((note->type == NOTE_FLAG_STATIC) && !bot)
				{
					Audio_PlaySound(Sounds[4], 0x3fff);
					this->health -= 2000;
					stage.hitstatic = 10;
					Stage_CutVocal();
					Stage_MissNote(this);
				}

				if (!(note->type & (NOTE_FLAG_HIT | NOTE_FLAG_MINE | NOTE_FLAG_PHANTOM | NOTE_FLAG_POLICE | NOTE_FLAG_MAGIC)) && !bot)
				{
					if (stage.prefs.mode < StageMode_Net1 || i == ((stage.prefs.mode == StageMode_Net1) ? 0 : 1))
					{
						// Missed note
						this->health -= 475;
						Stage_CutVocal();
						Stage_MissNote(this);
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
			if (!back && (y > (FIXED_DEC(480,2) + size) || note->pos == 0xFFFF))
			{
				break;
			}
			else if (back && (y > (FIXED_DEC(SCREEN_HEIGHT,2) + scroll.size) || note->pos == 0xFFFF))
			{
				break;
			}

			//Draw note
			if (note->type & NOTE_FLAG_SUSTAIN)
			{
				//Check for sustain clipping
				fixed_t clip;
				if (!back)
				{
					y -= size;
					if ((note->type & (NOTE_FLAG_HIT)) || (bot) || ((this->pad_held & note_key[Stage_GetNoteType(note) % stage.keys]) && (note_fp + stage.late_sus_safe >= stage.note_scroll)))
					{
						clip = 0 - y;
						if (clip < 0)
						{
							clip = 0;
						}
					}
					else 
					{
						clip = 0;
					}
				}
				else if (back)
				{
					y -= scroll.size;
					if ((note->type & (NOTE_FLAG_HIT)) || (bot) || ((this->pad_held & note_key[Stage_GetNoteType(note) % stage.keys]) && (note_fp + stage.late_sus_safe >= stage.note_scroll)))
					{
						clip = FIXED_DEC(32 - SCREEN_HEIGHT2, 1) - y;
						if (clip < 0)
						{
							clip = 0;
						}
					}
					else 
					{
						clip = 0;
					}
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
						
						if (!back)
						{
							note_dst.y = (FIXED_DEC(-88,1) + y) + clip;
						}
						else if (back)
						{
							note_dst.y = y + clip;
						}
						
						note_dst.y += offset;
						
						note_dst.w = (note_src.w << FIXED_SHIFT);
						note_dst.h = (note_src.h << FIXED_SHIFT);

						if (stage.prefs.downscroll)
						{
							note_dst.y = -note_dst.y;
							note_dst.h = -note_dst.h;
						}

						if (back == this->hud)
							Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_note);
					}
				}
				else
				{
					//Get note height
					fixed_t next_time = (scroll.start - stage.song_time) + (scroll.length * (note->pos + 12 - scroll.start_step) / scroll.length_step);
					fixed_t next_y = 0;
					
					if (!back)
					{
						next_y = FIXED_MUL(stage.speed, next_time * 150) - size;
					}
					else if (back)
					{
						u8 note_index = Stage_GetNoteType(note) % stage.keys;
						if (note->is_opponent) {
							note_index += stage.keys;
						}
						next_y = stage.note.y[note_index] + FIXED_MUL(stage.speed, next_time * 150) - scroll.size;
					}
					
					fixed_t next_size = next_y - y;
					
					if (clip < next_size)
					{
						note_src.x = (Stage_GetNoteType(note) % stage.keys) * stage.note.size;
						note_src.y = stage.note.size * 5;
						note_src.w = stage.note.size;
						note_src.h = stage.note.size / 2;
						
						note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
						
						if (!back)
						{
							note_dst.y = FIXED_DEC(-88,1) + y + clip;
						}
						else if (back)
						{
							note_dst.y = y + clip;
						}
						
						note_dst.y += offset;
						
						note_dst.w = note_src.w << FIXED_SHIFT;
						note_dst.h = (next_y - y) - clip;
						
						if (stage.prefs.downscroll)
							note_dst.y = -note_dst.y - note_dst.h;
						
						if (back == this->hud)
							Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_note);
					}
				}
			}
			else if (note->type & NOTE_FLAG_MINE)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;
				
				//Draw note body
				if (stage.keys == 4)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 64;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 5)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 64;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 6)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 48;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 7)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 48;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 9)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 32;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				
				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;
				
				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
					
				if (back == this->hud)
					Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_type);
			}
			else if (note->type & NOTE_FLAG_DANGER)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note body
				if (stage.keys == 4)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 96;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 5)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 96;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 6)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 72;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 7)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 72;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 9)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 48;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}

				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}			
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				
				if (back == this->hud)
					Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_type);
			}
			else if (note->type & NOTE_FLAG_STATIC)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note body
				if (stage.keys == 4)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 5)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 6)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 7)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 9)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}

				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}			
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;

				if (back == this->hud)
					Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_type);
			}
			else if (note->type & NOTE_FLAG_PHANTOM)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note body
				if (stage.keys == 4)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 32;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 5)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 32;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 6)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 24;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 7)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 24;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 9)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 16;
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}

				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}			
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				
				if (back == this->hud)
					Stage_DrawPhantomNote(&note_src, &note_dst, this->hud, &stage.tex_type);
			}
			else if (note->type & NOTE_FLAG_POLICE)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note body
				if (stage.keys == 4)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0 + ((animf_count & 0x6) << 5);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 5)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0 + ((animf_count & 0x6) << 5);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 6)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0 + ((animf_count & 0x6) << 5);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 7)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0 + ((animf_count & 0x6) << 5);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 9)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 0 + ((animf_count & 0x6) << 5);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}

				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}			
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				
				if (back == this->hud)
					Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_type2);
			}
			else if (note->type & NOTE_FLAG_MAGIC)
			{
				//Don't draw if already hit
				if (note->type & NOTE_FLAG_HIT)
					continue;

				//Draw note body
				if (stage.keys == 4)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 128 + ((animf_count & 0x2) * 16);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 5)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 128 + ((animf_count & 0x2) * 16);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 6)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 96 + ((animf_count & 0x2) * 16);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 7)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 96 + ((animf_count & 0x2) * 16);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}
				else if (stage.keys == 9)
				{
					note_src.x = ((Stage_GetNoteType(note) % stage.keys) * stage.note.size);
					note_src.y = 64 + ((animf_count & 0x2) * 16);
					note_src.w = stage.note.size;
					note_src.h = stage.note.size;
				}

				note_dst.x = stage.note.x[(Stage_GetNoteType(note) % stage.max_keys)] - FIXED_DEC(stage.note.size / 2,1);
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}			
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;

				if (back == this->hud)
					Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_type);
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
				if (back == false)
				{
					u8 note_index = Stage_GetNoteType(note) % stage.keys;
					if (note->is_opponent) {
						note_index += stage.keys;
					}
					note_dst.y = stage.note.y[note_index] + y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				else
				{
					note_dst.y = y - FIXED_DEC(stage.note.size / 2,1);
					note_dst.y += offset;
				}
				note_dst.w = note_src.w << FIXED_SHIFT;
				note_dst.h = note_src.h << FIXED_SHIFT;

				if (stage.prefs.downscroll)
					note_dst.y = -note_dst.y - note_dst.h;
				
				if (back == this->hud)
					Stage_DrawNote(&note_src, &note_dst, this->hud, &stage.tex_note);
			}
		}
	}
	
	//Draw HUD Notes
	Stage_DrawHUDNotes(back);
}

//Timer Code
#include "audio_def.h"

static void Stage_TimerGetLength(void)
{
    const XA_TrackDef1 *track_def1;
    const XA_TrackDef2 *track_def2;
	const XA_TrackDef3 *track_def3;

    // Select the correct track array based on the current disc
    if (currentDisc == 1) {
        track_def1 = &xa_tracks_disc1[stage.stage_def->music_track];
    } else if (currentDisc == 2) {
        track_def2 = &xa_tracks_disc2[stage.stage_def->music_track];
    }
	else if (currentDisc == 3) {
        track_def3 = &xa_tracks_disc3[stage.stage_def->music_track];
    }
	if (stage.stage_id >= StageId_1_1 && stage.stage_id <= StageId_3_3)
	{
		currentDisc = 1;
	}
	if (stage.stage_id >= StageId_4_1 && stage.stage_id <= StageId_4_7)
	{
		currentDisc = 2;
	}
	if (stage.stage_id >= StageId_5_1 && stage.stage_id <= StageId_5_7)
	{
		currentDisc = 3;
	}

    // Calculate the timer length from the track length
	if (currentDisc == 1) {
    	stage.timerlength = (track_def1->length / 75 / IO_SECT_SIZE) - 1; // Convert in seconds
	}
	if (currentDisc == 2) {
		stage.timerlength = (track_def2->length / 75 / IO_SECT_SIZE) - 1; // Convert in seconds
	}
	if (currentDisc == 3) {
		stage.timerlength = (track_def3->length / 75 / IO_SECT_SIZE) - 1; // Convert in seconds
	}

    // Calculate minutes and seconds
    stage.timermin = stage.timerlength / 60;
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

static void Stage_OrangeTimerTick(void)
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
	if (stage.prefs.songtimer)
	{
		char text[0x80];

		//format string
		sprintf(text, "%d : %s%d", stage.timermin, (stage.timersec > 9) ?"" :"0", stage.timersec); //making this for avoid cases like 1:4

		//Draw text
		if (hudEnabled == 1)
		{
				stage.font_cdr.draw_col(&stage.font_cdr,
				text,
				FIXED_DEC(-18,1),
				(stage.prefs.downscroll) ? FIXED_DEC(104,1) : FIXED_DEC(-112,1),
				FontAlign_Left,
				255 >> 1,
				255 >> 1,
				255 >> 1
			);
		}

		//draw square length
		RECT square_black = {0, 154, 111, 4};
		RECT square_fill = {0, 154, (110 * (stage.timepassed) / (stage.timerlength * 60)), 4};

		RECT_FIXED square_dst = {
		FIXED_DEC(-56,1),
		FIXED_DEC(-110,1),
		0,
		FIXED_DEC(4,1)
	};

		if (stage.prefs.downscroll)
			square_dst.y = -square_dst.y - square_dst.h + FIXED_DEC(1,1);

		square_dst.w = square_fill.w << FIXED_SHIFT;
		Stage_DrawTexCol(&stage.tex_hud1, &square_fill, &square_dst, stage.bump, stage.camera.hudangle, 255 >> 1, 128 >> 1, 0 >> 1);
		
		square_dst.w = square_black.w << FIXED_SHIFT;
		Stage_DrawTexCol(&stage.tex_hud1, &square_black, &square_dst, stage.bump, stage.camera.hudangle,  0,  0,  0);
	}
}

int soundcooldown;
int drawshit;

static void Stage_CountDown(void)
{
	switch(stage.song_step)
	{
		case -20:
			if (soundcooldown == 0)
				Audio_PlaySound(Sounds[0], 0x3fff);
			soundcooldown ++;
			break;
		case -16:
			soundcooldown = 0;
			break;
		case -15:
			drawshit = 3;
			if (soundcooldown == 0)
				Audio_PlaySound(Sounds[1], 0x3fff);
			soundcooldown ++;
			break;
		case -11:
			soundcooldown = 0;
			break;
		case -10:
			drawshit = 2;
			if (soundcooldown == 0)
				Audio_PlaySound(Sounds[2], 0x3fff);
			soundcooldown ++;
			break;
		case -6:
			soundcooldown = 0;
			break;
		case -5:
			drawshit = 1;
			if (soundcooldown == 0)
				Audio_PlaySound(Sounds[3], 0x3fff);
			soundcooldown ++;
			break;
	}

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
	if (stage.stage_def->pchar.new != NULL) {
		stage.player = stage.stage_def->pchar.new(stage.stage_def->pchar.x, stage.stage_def->pchar.y);
	}
	else
		stage.player = NULL;
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
	if (stage.stage_def->ochar.new != NULL) {
		stage.opponent = stage.stage_def->ochar.new(stage.stage_def->ochar.x, stage.stage_def->ochar.y);
	}
	else
		stage.opponent = NULL;
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
	//Use standard path convention
	sprintf(chart_path, "\\CHART\\%d.%d%c.CHT;1", stage.stage_def->week, stage.stage_def->week_song, "ENH"[stage.stage_diff]);
	
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
 			stage.num_notes = (notes - 1) * 2; // Double the number of notes
 		else
 			stage.num_notes = 0;
 		
 		//Realloc for separate structs
 		size_t sections_size = sections * sizeof(Section);
 		size_t notes_size = stage.num_notes * sizeof(Note); // Adjusted for double notes
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
 		
 		// Use reformatted chart
 		Mem_Free(stage.chart_data);
 		stage.chart_data = (IO_Data)nchart;
 	#else
 		// Directly use section and notes pointers
 		stage.sections = (Section*)(chart_byte + 4);
 		stage.notes = (Note*)(chart_byte + ((u16*)stage.chart_data)[1]);
 		
 		for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
 			stage.num_notes++;
 	#endif
 	
 	// Swap chart
 	if (stage.prefs.mode == StageMode_Swap)
 	{
 		for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
 			note->is_opponent = !note->is_opponent;
 	}
 	
 	// Count max scores
 	stage.player_state[0].max_score = 0;
 	stage.player_state[1].max_score = 0;
 	for (Note *note = stage.notes; note->pos != 0xFFFF; note++)
 	{
 		if (note->type & (NOTE_FLAG_SUSTAIN | NOTE_FLAG_MINE | NOTE_FLAG_DANGER | NOTE_FLAG_STATIC | NOTE_FLAG_PHANTOM | NOTE_FLAG_POLICE | NOTE_FLAG_MAGIC))
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
	u32* data;

	// Load INTRO0.VAG
	IO_FindFile(&file, "\\SOUNDS\\INTRO0.VAG;1");
	data = IO_ReadFile(&file);
	Sounds[0] = Audio_LoadVAGData(data, file.size);
	Mem_Free(data);

	// Load INTRO1.VAG
	IO_FindFile(&file, "\\SOUNDS\\INTRO1.VAG;1");
	data = IO_ReadFile(&file);
	Sounds[1] = Audio_LoadVAGData(data, file.size);
	Mem_Free(data);

	// Load INTRO2.VAG
	IO_FindFile(&file, "\\SOUNDS\\INTRO2.VAG;1");
	data = IO_ReadFile(&file);
	Sounds[2] = Audio_LoadVAGData(data, file.size);
	Mem_Free(data);

	// Load INTRO3.VAG
	IO_FindFile(&file, "\\SOUNDS\\INTRO3.VAG;1");
	data = IO_ReadFile(&file);
	Sounds[3] = Audio_LoadVAGData(data, file.size);
	Mem_Free(data);
	
	//Load SFX
	IO_FindFile(&file, "\\SOUNDS\\HITSTAT.VAG;1");
	data = IO_ReadFile(&file);
	Sounds[4] = Audio_LoadVAGData(data, file.size);
	Mem_Free(data);
}

static void Stage_LoadMusic(void)
{
	//Offset sing ends
	if (stage.player != NULL)
		stage.player->sing_end -= stage.note_scroll;
	if (stage.player2 != NULL)
		stage.player2->sing_end -= stage.note_scroll;
	if (stage.opponent != NULL)
		stage.opponent->sing_end -= stage.note_scroll;
	if (stage.opponent2 != NULL)
		stage.opponent2->sing_end -= stage.note_scroll;
	if (stage.gf != NULL)
		stage.gf->sing_end -= stage.note_scroll;
	
	//Find music file and begin seeking to it
	if (stage.stage_id >= StageId_1_1 && stage.stage_id <= StageId_3_3)
	{
		Audio_SeekXA_TrackDisc1(stage.stage_def->music_track, stage.audio_start_pos);
	}
	if (stage.stage_id >= StageId_4_1 && stage.stage_id <= StageId_4_7)
	{
		Audio_SeekXA_TrackDisc2(stage.stage_def->music_track, stage.audio_start_pos);
	}
	if (stage.stage_id >= StageId_5_1 && stage.stage_id <= StageId_5_7)
	{
		Audio_SeekXA_TrackDisc3(stage.stage_def->music_track, stage.audio_start_pos);
	}
	
	//Initialize music state
	stage.intro = true;
	stage.note_scroll = FIXED_DEC(-5 * 6 * 12,1);
	stage.song_time = FIXED_DIV(stage.note_scroll, stage.step_crochet);
	stage.interp_time = 0;
	stage.interp_ms = 0;
	stage.interp_speed = 0;
	
	//Offset sing ends again
	if (stage.player != NULL)
		stage.player->sing_end += stage.note_scroll;
	if (stage.player2 != NULL)
		stage.player2->sing_end += stage.note_scroll;
	if (stage.opponent != NULL)
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
	
	drain = 0;
	
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
		soundcooldown = 0;
		drawshit = 0;
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
		
		//Initialize boolean back logic fields
		stage.player_state[i].visible = true;
		stage.player_state[i].hud = true;
	}

	//for silly billy
	if(stage.stage_id == StageId_5_7)
    {
        stage.player_state[1].hud = false;
    }

	// Determine note offset type based on stage and conditions
	int note_offset_type = 0; // [0] = normal offsets
	if (stage.stage_id == StageId_5_7) {
		note_offset_type = 2; // [2] = silly billy opponent offsets
		// TODO: Add logic for [1] = flipped offsets and [3] = silly billy opponent + player middle scroll
	}
	
	//Update the keys
	// Silly Billy offsetted notes handling
	if (stage.stage_id == StageId_5_7)
	{
		switch (stage.keys)
		{
			//4 Key
			case 4:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x4k_silly_billy_middle; // Use silly billy + middle scroll
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x4k_normal; break;
						case 1: stage.note.x = note_x4k_flipped; break;
						case 2: stage.note.x = note_x4k_silly_billy; break;
						case 3: stage.note.x = note_x4k_silly_billy_middle; break;
						default: stage.note.x = note_x4k_normal; break;
					}
				}
				for (int i = 0; i < 4; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions higher for StageId_5_7
				for (int i = 0; i < 4; i++) {
					if (stage.prefs.downscroll)
					{
						stage.note.y[i + 4] = FIXED_DEC(12, 1);
					}
					else
					{
						stage.note.y[i + 4] = FIXED_DEC(-126, 1);
					}
				}
				note_anims = note_anims4k;
				note_key = note_key4k;
				stage.note.size = 32;
			break;
			//5 Key
			case 5:
				if (stage.stage_id == StageId_4_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
				{
					stage.note.x = note_x5k_flipped; // Use note_x5kflip when condition is true
				}
				else if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x5k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					stage.note.x = note_x5k_normal;
				}
				for (int i = 0; i < 5; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions to match player positions
				for (int i = 0; i < 5; i++) {
					if (stage.prefs.downscroll)
					{
						stage.note.y[i + 5] = FIXED_DEC(12, 1);
					}
					else
					{
						stage.note.y[i + 5] = FIXED_DEC(-126, 1);
					}
				}
				note_anims = note_anims5k;
				note_key = note_key5k;
				stage.note.size = 32;
			break;
			//6 Key
			case 6:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x6k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x6k_normal; break;
						case 1: stage.note.x = note_x6k_flipped; break;
						case 2: stage.note.x = note_x6k_silly_billy; break;
						case 3: stage.note.x = note_x6k_silly_billy_middle; break;
						default: stage.note.x = note_x6k_normal; break;
					}
				}
				for (int i = 0; i < 6; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions to match player positions
				for (int i = 0; i < 6; i++) {
					if (stage.prefs.downscroll)
					{
						stage.note.y[i + 6] = FIXED_DEC(12, 1);
					}
					else
					{
						stage.note.y[i + 6] = FIXED_DEC(-126, 1);
					}
				}
				note_anims = note_anims6k;
				note_key = note_key6k;
				stage.note.size = 24;
			break;
			//7 Key
			case 7:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x7k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x7k_normal; break;
						case 1: stage.note.x = note_x7k_flipped; break;
						case 2: stage.note.x = note_x7k_silly_billy; break;
						case 3: stage.note.x = note_x7k_silly_billy_middle; break;
						default: stage.note.x = note_x7k_normal; break;
					}
				}
				for (int i = 0; i < 7; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions to match player positions
				for (int i = 0; i < 7; i++) {
					if (stage.prefs.downscroll)
					{
						stage.note.y[i + 7] = FIXED_DEC(12, 1);
					}
					else
					{
						stage.note.y[i + 7] = FIXED_DEC(-126, 1);
					}
				}
				note_anims = note_anims7k;
				note_key = note_key7k;
				stage.note.size = 24;
			break;
			//9 Key
			case 9:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x9k_silly_billy_middle; // Use silly billy + middle scroll when condition is true
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x9k_normal; break;
						case 1: stage.note.x = note_x9k_flipped; break;
						case 2: stage.note.x = note_x9k_silly_billy; break;
						case 3: stage.note.x = note_x9k_silly_billy_middle; break;
						default: stage.note.x = note_x9k_normal; break;
					}
				}
				for (int i = 0; i < 9; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions to match player positions
				for (int i = 0; i < 9; i++) {
					if (stage.prefs.downscroll)
					{
						stage.note.y[i + 9] = FIXED_DEC(12, 1);
					}
					else
					{
						stage.note.y[i + 9] = FIXED_DEC(-126, 1);
					}
				}
				note_anims = note_anims9k;
				note_key = note_key9k;
				stage.note.size = 16;
			break;
			//4 Key
			default:    
				switch (note_offset_type) {
					case 0: stage.note.x = note_x4k_normal; break;
					case 1: stage.note.x = note_x4k_flipped; break;
					case 2: stage.note.x = note_x4k_silly_billy; break;
					case 3: stage.note.x = note_x4k_silly_billy_middle; break;
					default: stage.note.x = note_x4k_normal; break;
				}
				for (int i = 0; i < 4; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions to match player positions
				for (int i = 0; i < 4; i++) {
					if (stage.prefs.downscroll)
					{
						stage.note.y[i + 4] = FIXED_DEC(12, 1);
					}
					else
					{
						stage.note.y[i + 4] = FIXED_DEC(-126, 1);
					}
				}
				note_anims = note_anims4k;
				note_key = note_key4k;
				stage.note.size = 32;
			break;
		}
	}
	else
	{
		switch (stage.keys)
		{
			//4 Key
			case 4:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x4k_silly_billy_middle; // Use silly billy + middle scroll
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x4k_normal; break;
						case 1: stage.note.x = note_x4k_flipped; break;
						case 2: stage.note.x = note_x4k_silly_billy; break;
						case 3: stage.note.x = note_x4k_silly_billy_middle; break;
						default: stage.note.x = note_x4k_normal; break;
					}
				}
				for (int i = 0; i < 4; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				for (int i = 0; i < 4; i++) {
					stage.note.y[i + 4] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				note_anims = note_anims4k;
				note_key = note_key4k;
				stage.note.size = 32;
			break;
			//5 Key
			case 5:
				if (stage.stage_id == StageId_4_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
				{
					stage.note.x = note_x5k_flipped; // Use note_x5kflip when condition is true
				}
				else if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x5k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					stage.note.x = note_x5k_normal;
				}
				for (int i = 0; i < 5; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				for (int i = 0; i < 5; i++) {
					stage.note.y[i + 5] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				note_anims = note_anims5k;
				note_key = note_key5k;
				stage.note.size = 32;
			break;
			//6 Key
			case 6:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x6k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x6k_normal; break;
						case 1: stage.note.x = note_x6k_flipped; break;
						case 2: stage.note.x = note_x6k_silly_billy; break;
						case 3: stage.note.x = note_x6k_silly_billy_middle; break;
						default: stage.note.x = note_x6k_normal; break;
					}
				}
				for (int i = 0; i < 6; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				for (int i = 0; i < 6; i++) {
					stage.note.y[i + 6] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				note_anims = note_anims6k;
				note_key = note_key6k;
				stage.note.size = 24;
			break;
			//7 Key
			case 7:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x7k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x7k_normal; break;
						case 1: stage.note.x = note_x7k_flipped; break;
						case 2: stage.note.x = note_x7k_silly_billy; break;
						case 3: stage.note.x = note_x7k_silly_billy_middle; break;
						default: stage.note.x = note_x7k_normal; break;
					}
				}
				for (int i = 0; i < 7; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				for (int i = 0; i < 7; i++) {
					stage.note.y[i + 7] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				note_anims = note_anims7k;
				note_key = note_key7k;
				stage.note.size = 24;
			break;
			//9 Key
			case 9:
				if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					stage.note.x = note_x9k_silly_billy_middle; // Use note_x5kflip when condition is true
				}
				else
				{
					switch (note_offset_type) {
						case 0: stage.note.x = note_x9k_normal; break;
						case 1: stage.note.x = note_x9k_flipped; break;
						case 2: stage.note.x = note_x9k_silly_billy; break;
						case 3: stage.note.x = note_x9k_silly_billy_middle; break;
						default: stage.note.x = note_x9k_normal; break;
					}
				}
				for (int i = 0; i < 9; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				for (int i = 0; i < 9; i++) {
					stage.note.y[i + 9] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				note_anims = note_anims9k;
				note_key = note_key9k;
				stage.note.size = 16;
			break;
			//4 Key
			default:    
				switch (note_offset_type) {
					case 0: stage.note.x = note_x4k_normal; break;
					case 1: stage.note.x = note_x4k_flipped; break;
					case 2: stage.note.x = note_x4k_silly_billy; break;
					case 3: stage.note.x = note_x4k_silly_billy_middle; break;
					default: stage.note.x = note_x4k_normal; break;
				}
				for (int i = 0; i < 4; i++) {
					stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				// Set opponent Y positions to match player positions
				for (int i = 0; i < 4; i++) {
					stage.note.y[i + 4] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
				}
				note_anims = note_anims4k;
				note_key = note_key4k;
				stage.note.size = 32;
			break;
		}
	}
	
	ObjectList_Free(&stage.objlist_splash);
	ObjectList_Free(&stage.objlist_fg);
	ObjectList_Free(&stage.objlist_bg);
}

// --- Mid-game swap support ---
static StageId g_pendingSwapTarget = StageId_Max; // invalid when no swap pending
static u8 g_pendingSwapFlags = 0;                 // STAGE_LOAD_* bitfield

static void Stage_PerformMidSwap(StageId target, u8 load_flags)
{
	// Update definition pointer and stage id
	stage.stage_def = &stage_defs[stage.stage_id = target];

	// Swap background if requested
	if (load_flags & STAGE_LOAD_STAGE)
		Stage_LoadStage();

	// Handle player character
	if (load_flags & STAGE_LOAD_PLAYER)
		Stage_LoadPlayer();
	else if (stage.stage_def->pchar.new != NULL) {
		if (stage.player != NULL) {
			stage.player->x = stage.stage_def->pchar.x;
			stage.player->y = stage.stage_def->pchar.y;
		}
	} else if (stage.player != NULL) {
		// Unload player if target stage doesn't have one
		Character_Free(stage.player);
		stage.player = NULL;
	}

	// Handle player2 character
	if (load_flags & STAGE_LOAD_PLAYER2)
		Stage_LoadPlayer2();
	else if (stage.stage_def->pchar2.new != NULL) {
		if (stage.player2 != NULL) {
			stage.player2->x = stage.stage_def->pchar2.x;
			stage.player2->y = stage.stage_def->pchar2.y;
		}
	} else if (stage.player2 != NULL) {
		// Unload player2 if target stage doesn't have one
		Character_Free(stage.player2);
		stage.player2 = NULL;
	}

	// Handle opponent character
	if (load_flags & STAGE_LOAD_OPPONENT)
		Stage_LoadOpponent();
	else if (stage.stage_def->ochar.new != NULL) {
		if (stage.opponent != NULL) {
			stage.opponent->x = stage.stage_def->ochar.x;
			stage.opponent->y = stage.stage_def->ochar.y;
		}
	} else if (stage.opponent != NULL) {
		// Unload opponent if target stage doesn't have one
		Character_Free(stage.opponent);
		stage.opponent = NULL;
	}

	// Handle opponent2 character
	if (load_flags & STAGE_LOAD_OPPONENT2)
		Stage_LoadOpponent2();
	else if (stage.stage_def->ochar2.new != NULL) {
		if (stage.opponent2 != NULL) {
			stage.opponent2->x = stage.stage_def->ochar2.x;
			stage.opponent2->y = stage.stage_def->ochar2.y;
		}
	} else if (stage.opponent2 != NULL) {
		// Unload opponent2 if target stage doesn't have one
		Character_Free(stage.opponent2);
		stage.opponent2 = NULL;
	}

	// Handle girlfriend character
	if (load_flags & STAGE_LOAD_GIRLFRIEND)
		Stage_LoadGirlfriend();
	else if (stage.stage_def->gchar.new != NULL) {
		if (stage.gf != NULL) {
			stage.gf->x = stage.stage_def->gchar.x;
			stage.gf->y = stage.stage_def->gchar.y;
		}
	} else if (stage.gf != NULL) {
		// Unload girlfriend if target stage doesn't have one
		Character_Free(stage.gf);
		stage.gf = NULL;
	}

	// Keep current chart/music/timers; update player_state character pointers only
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
	
	// Ensure player_state character pointers are valid
	if (stage.player_state[0].character == NULL)
		stage.player_state[0].character = stage.player; // Fallback to player if opponent is NULL
	if (stage.player_state[1].character == NULL)
		stage.player_state[1].character = stage.opponent; // Fallback to opponent if player is NULL

	// Recalculate camera target to avoid a sudden snap
	if (stage.cur_section != NULL)
	{
		if (stage.cur_section->flag & SECTION_FLAG_OPPFOCUS)
		{
			if (stage.opponent != NULL)
				Stage_FocusCharacter(stage.opponent);
			else if (stage.player != NULL)
				Stage_FocusCharacter(stage.player); // Fallback to player if opponent is NULL
		}
		else
		{
			if (stage.player != NULL)
				Stage_FocusCharacter(stage.player);
			else if (stage.opponent != NULL)
				Stage_FocusCharacter(stage.opponent); // Fallback to opponent if player is NULL
		}
	}

	// Do not reload music or chart here to keep timing continuous

	// Re-apply XA track/channel and resume playback at current time
	s32 resume_seconds = (s32)(stage.song_time >> FIXED_SHIFT);
	if (resume_seconds < 0) resume_seconds = 0;
	if (stage.music_disc_active == 1) {
		Audio_PlayXA_TrackDisc1((XA_Track1)stage.music_track_active, 0x40, stage.music_channel_active, false, resume_seconds);
	} else if (stage.music_disc_active == 2) {
		Audio_PlayXA_TrackDisc2((XA_Track2)stage.music_track_active, 0x40, stage.music_channel_active, false, resume_seconds);
	} else if (stage.music_disc_active == 3) {
		Audio_PlayXA_TrackDisc3((XA_Track3)stage.music_track_active, 0x40, stage.music_channel_active, false, resume_seconds);
	}
	Audio_ChannelXA(stage.music_channel_active);

	// Brief grace period to avoid false misses due to asset latency
	stage.swap_grace_frames = 6; // ~1/10 sec at 60fps
}

void Stage_RequestSwapTo(StageId target, u8 load_flags)
{
	g_pendingSwapTarget = target;
	g_pendingSwapFlags = load_flags;
}

void Stage_RequestNextLoadSwap(void)
{
	// Use the stage's defined next hop
	Stage_RequestSwapTo(stage.stage_def->next_stage, stage.stage_def->next_load);
}

//Stage functions
void Stage_Load(StageId id, StageDiff difficulty, boolean story)
{
	//Get stage definition
	stage.stage_def = &stage_defs[stage.stage_id = id];
	stage.stage_diff = difficulty;
	stage.story = story;

	// Track the original requested song for restarts
	stage.original_stage_id = id;
	stage.original_stage_diff = difficulty;
	stage.original_story = story;

	// Persist music/flow state for this song
	if (id >= StageId_1_1 && id <= StageId_3_3) stage.music_disc_active = 1; else
	if (id >= StageId_4_1 && id <= StageId_4_7) stage.music_disc_active = 2; else
	if (id >= StageId_5_1 && id <= StageId_5_7) stage.music_disc_active = 3;
	stage.music_track_active = stage.stage_def->music_track;
	stage.music_channel_active = stage.stage_def->music_channel;
	stage.next_stage_active = stage.stage_def->next_stage;
	stage.next_load_active = stage.stage_def->next_load;
	
	if (stage.stage_id == StageId_5_2)
	{
		IO_FindFile(&stage.str_grace_lba, "\\STR\\GRACE.STR;1");
	}
	else if (stage.stage_id == StageId_5_7)
	{
		IO_FindFile(&stage.str_sillybilly_intro_lba, "\\STR\\INTRO.STR;1");
		IO_FindFile(&stage.str_sillybilly_final_lba, "\\STR\\FINAL.STR;1");
	}

    // Check movies
    // Don't play movie if you are retrying the song
    if (stage.trans != StageTrans_Reload)
        Str_CanPlayDef();
	
	//Load HUD textures
	if (stage.stage_def->tim)
	{
		stage.startscreen = 512;
		Gfx_LoadTex(&stage.tex_strscr, IO_Read("\\STAGE\\PHASE3.TIM;1"), GFX_LOADTEX_FREE);
	}
	if (stage.stage_id >= StageId_1_1 && stage.stage_id <= StageId_4_7)
	{
		Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
	}
	else if (stage.stage_id == StageId_5_1)
	{
		Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
	}
	else if (stage.stage_id == StageId_5_2)
	{
		Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID1.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
	}
	else if (stage.stage_id >= StageId_5_3 && stage.stage_id <= StageId_5_7)
	{
		Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID0.TIM;1"), GFX_LOADTEX_FREE);
		Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
	}
	
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
	char type_text[32];
	char type2_text[32];

	sprintf(note_text, "\\STAGE\\NOTE%dK.TIM;1", stage.keys);
	Gfx_LoadTex(&stage.tex_note, IO_Read(note_text), GFX_LOADTEX_FREE);
	if (stage.stage_def->has_note_types)
	{
		if (stage.stage_id == StageId_4_4)
		{
			sprintf(type_text, "\\STAGE\\TYPE%dK1.TIM;1", stage.keys);
			Gfx_LoadTex(&stage.tex_type, IO_Read(type_text), GFX_LOADTEX_FREE);
			sprintf(type2_text, "\\STAGE\\TYPE%dK0.TIM;1", stage.keys);
			Gfx_LoadTex(&stage.tex_type2, IO_Read(type2_text), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_static, IO_Read("\\STAGE\\STATIC.TIM;1"), GFX_LOADTEX_FREE);
		}
		else
		{
			sprintf(type_text, "\\STAGE\\TYPE%dK1.TIM;1", stage.keys);
			Gfx_LoadTex(&stage.tex_type, IO_Read(type_text), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_static, IO_Read("\\STAGE\\STATIC.TIM;1"), GFX_LOADTEX_FREE);
		}
	}
	
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

		//Check movies
		Str_CanPlayDef();
		
		//Load stage background
		if (load & STAGE_LOAD_STAGE)
			Stage_LoadStage();
		
		//Load characters
		if (load & STAGE_LOAD_PLAYER)
		{
			Stage_LoadPlayer();
		}
		else if (stage.stage_def->pchar.new != NULL)
		{
			if (stage.player != NULL) {
				stage.player->x = stage.stage_def->pchar.x;
				stage.player->y = stage.stage_def->pchar.y;
			}
		} else if (stage.player != NULL) {
			// Unload player if target stage doesn't have one
			Character_Free(stage.player);
			stage.player = NULL;
		}
		
		if (load & STAGE_LOAD_PLAYER2)
		{
			Stage_LoadPlayer2();
		}
		else if (stage.stage_def->pchar2.new != NULL)
		{
			if (stage.player2 != NULL) {
				stage.player2->x = stage.stage_def->pchar2.x;
				stage.player2->y = stage.stage_def->pchar2.y;
			}
		} else if (stage.player2 != NULL) {
			// Unload player2 if target stage doesn't have one
			Character_Free(stage.player2);
			stage.player2 = NULL;
		}
		
		if (load & STAGE_LOAD_OPPONENT)
		{
			Stage_LoadOpponent();
		}
		else if (stage.stage_def->ochar.new != NULL)
		{
			if (stage.opponent != NULL) {
				stage.opponent->x = stage.stage_def->ochar.x;
				stage.opponent->y = stage.stage_def->ochar.y;
			}
		} else if (stage.opponent != NULL) {
			// Unload opponent if target stage doesn't have one
			Character_Free(stage.opponent);
			stage.opponent = NULL;
		}
		
		if (load & STAGE_LOAD_OPPONENT2)
		{
			Stage_LoadOpponent2();
		}
		else if (stage.stage_def->ochar2.new != NULL)
		{
			if (stage.opponent2 != NULL) {
				stage.opponent2->x = stage.stage_def->ochar2.x;
				stage.opponent2->y = stage.stage_def->ochar2.y;
			}
		} else if (stage.opponent2 != NULL) {
			// Unload opponent2 if target stage doesn't have one
			Character_Free(stage.opponent2);
			stage.opponent2 = NULL;
		}
		
		if (load & STAGE_LOAD_GIRLFRIEND)
		{
			Stage_LoadGirlfriend();
		}
		else if (stage.stage_def->gchar.new != NULL)
		{
			if (stage.gf != NULL) {
				stage.gf->x = stage.stage_def->gchar.x;
				stage.gf->y = stage.stage_def->gchar.y;
			}
		} else if (stage.gf != NULL) {
			// Unload girlfriend if target stage doesn't have one
			Character_Free(stage.gf);
			stage.gf = NULL;
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
	
	// Determine note offset type based on stage and conditions
	int note_offset_type = 0; // [0] = normal offsets
	if (stage.stage_id == StageId_5_7) {
		note_offset_type = 2; // [2] = silly billy opponent offsets
		// TODO: Add logic for [1] = flipped offsets and [3] = silly billy opponent + player middle scroll
	}
	
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
		// Allow immediate transition to menu only on death screen (player restart or exit), keep during play
		stage.trans = StageTrans_Menu;
		Trans_Start();
	}
	
	if (stage.stage_id == StageId_5_2 && stage.song_step == 1312 && stage.flag & STAGE_FLAG_JUST_STEP && !stage.movie_is_playing)
	{
		Str_PlayFile(&stage.str_grace_lba);
		if (currentDisc == 1) {
			Audio_PlayXA_TrackDisc1(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 168);
		}
		if (currentDisc == 2) {
			Audio_PlayXA_TrackDisc2(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 168);
		}
		if (currentDisc == 3) {
			Audio_PlayXA_TrackDisc3(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 168);
		}
		Audio_WaitPlayXA();
	}
	
	if (stage.stage_id == StageId_5_7 && stage.song_step == 0 && stage.flag & STAGE_FLAG_JUST_STEP && !stage.movie_is_playing)
	{
		stage.audio_start_pos = 0;
		Str_PlayFile(&stage.str_sillybilly_intro_lba);
		if (currentDisc == 1) {
			Audio_PlayXA_TrackDisc1(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 12);
		}
		if (currentDisc == 2) {
			Audio_PlayXA_TrackDisc2(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 12);
		}
		if (currentDisc == 3) {
			Audio_PlayXA_TrackDisc3(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 12);
		}
		Audio_WaitPlayXA();
	}
	
	if (stage.stage_id == StageId_5_7 && stage.song_step == 3492 && stage.flag & STAGE_FLAG_JUST_STEP && !stage.movie_is_playing)
	{
		stage.audio_start_pos = 304;
		Str_PlayFile(&stage.str_sillybilly_final_lba);
		if (currentDisc == 1) {
			Audio_PlayXA_TrackDisc1(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 338);
		}
		if (currentDisc == 2) {
			Audio_PlayXA_TrackDisc2(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 338);
		}
		if (currentDisc == 3) {
			Audio_PlayXA_TrackDisc3(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 338);
		}
		Audio_WaitPlayXA();
	}
	
	// At step 1824 on stage 5_2, reload HUD/grid textures so they draw again
	if (stage.stage_id == StageId_5_2)
	{
		if (stage.song_step == 1824 && (stage.flag & STAGE_FLAG_JUST_STEP))
		{
			// Perform stage-only mid swap (reload background assets without touching characters/music)
			Stage_RequestSwapTo(StageId_5_2, STAGE_LOAD_STAGE);

			Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID1.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
		}	
	}
	else if (stage.stage_id == StageId_5_7)
	{
		if (stage.song_step == 128 && (stage.flag & STAGE_FLAG_JUST_STEP))
		{
			// Perform stage-only mid swap (reload background assets without touching characters/music)
			Stage_RequestSwapTo(StageId_5_7, STAGE_LOAD_STAGE);

			Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID0.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
		}
		else if (stage.song_step == 3888 && (stage.flag & STAGE_FLAG_JUST_STEP))
		{
			// Perform stage-only mid swap (reload background assets without touching characters/music)
			Stage_RequestSwapTo(StageId_5_7, STAGE_LOAD_STAGE);

			Gfx_LoadTex(&stage.tex_hud0, IO_Read("\\STAGE\\HUD0.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hud1, IO_Read("\\STAGE\\GRID0.TIM;1"), GFX_LOADTEX_FREE);
			Gfx_LoadTex(&stage.tex_hude, IO_Read("\\STAGE\\HUDE.TIM;1"), GFX_LOADTEX_FREE);
		}
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
				// Restart should load the original song requested at menu time, not any swapped state
				Stage_Load(stage.original_stage_id, stage.original_stage_diff, stage.original_story);
				LoadScr_End();
				break;
		}
	}
	
    // Apply pending mid-game swap at a safe point each frame
    if (g_pendingSwapTarget != StageId_Max)
    {
        Stage_PerformMidSwap(g_pendingSwapTarget, g_pendingSwapFlags);
        g_pendingSwapTarget = StageId_Max;
        g_pendingSwapFlags = 0;
    }

    switch (stage.state)
	{
		case StageState_Play:
		{
			if (stage.stage_id >= StageId_1_1 && stage.stage_id <= StageId_3_3)
			{
				currentDisc = 1;
			}
			if (stage.stage_id >= StageId_4_1 && stage.stage_id <= StageId_4_7)
			{
				currentDisc = 2;
			}
			if (stage.stage_id >= StageId_5_1 && stage.stage_id <= StageId_5_7)
			{
				currentDisc = 3;
			}
			
			if ((stage.stage_id == StageId_Temp)) //PLACEHOLDER
			{
				stage.flash = FIXED_DEC(255,1);
				stage.flashspd = FIXED_DEC(1000,1);
			}
			if (stage.prefs.flash != 0)
			{
				if (stage.flash > 0)
				{
					RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
					u8 flash_col = stage.flash >> FIXED_SHIFT;
					Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 1);
					if (stage.paused == false)
						stage.flash -= FIXED_MUL(stage.flashspd, timer_dt);
				}
			}
			
			//Draw stage hud elements
			if (stage.back->draw_hud != NULL)
				stage.back->draw_hud(stage.back);
			
			if (stage.startscreen > 0)
				Stage_DrawStartScreen();
			
			Static_tick();
			
			if (stage.song_step >= 0)
			{
				if (stage.paused == false && pad_state.press & PAD_START && !stage.movie_is_playing)
				{
					stage.pause_scroll = -1;
					Audio_PauseXA();
					stage.paused = true;
					pad_state.press = 0;
				}
			}
			
			if (stage.stage_id == StageId_5_2)
			{
				if (stage.song_step >= -48 && stage.song_step <= 16)
				{
					hudEnabled = 0;
					noteEnabled = 0;
				}
				else if (stage.song_step >= 512 && stage.song_step <= 528)
				{
					hudEnabled = 0;
					noteEnabled = 1;
				}
				else if (stage.song_step >= 1304 && stage.song_step <= 1559)
				{
					hudEnabled = 0;
					noteEnabled = 0;
				}
				else if (stage.song_step >= 1560 && stage.song_step <= 1824)
				{
					hudEnabled = 0;
					noteEnabled = 1;
				}
				else if (stage.song_step >= 2080 && stage.song_step <= 2084)
				{
					hudEnabled = 0;
					noteEnabled = 0;
				}
				else if (stage.song_step >= 2208 && stage.song_step <= 2224)
				{
					hudEnabled = 0;
					noteEnabled = 0;
				}
				else if (stage.song_step >= 2364 && stage.song_step <= 5500)
				{
					hudEnabled = 0;
					noteEnabled = 0;
				}
				else
				{
					hudEnabled = 1;
					noteEnabled = 1;
				}
			}
			else if (stage.stage_id == StageId_5_7)
			{
				if (stage.song_step >= 3356 && stage.song_step <= 3620)
				{
					hudEnabled = 0;
					noteEnabled = 0;
				}
				else if (stage.song_step >= 3621 && stage.song_step <= 3888)
				{
					hudEnabled = 0;
					noteEnabled = 1;
				}
				else
				{
					hudEnabled = 1;
					noteEnabled = 1;
				}
			}
			else
			{
				hudEnabled = 1;
				noteEnabled = 1;
			}
			
			//Update the keys
			// Determine note offset type based on stage and conditions
			int note_offset_type = 0; // [0] = normal offsets
			if (stage.stage_id == StageId_5_7) {
				note_offset_type = 2; // [2] = silly billy opponent offsets
				// TODO: Add logic for [1] = flipped offsets and [3] = silly billy opponent + player middle scroll
			}
			
			//Update the keys
			// Silly Billy offsetted notes handling
			if (stage.stage_id == StageId_5_7)
			{
				switch (stage.keys)
				{
					//4 Key
					case 4:
						if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
						{
							stage.note.x = note_x4k_silly_billy_middle; // Use silly billy + middle scroll
						}
						else
						{
							switch (note_offset_type) {
								case 0: stage.note.x = note_x4k_normal; break;
								case 1: stage.note.x = note_x4k_flipped; break;
								case 2: stage.note.x = note_x4k_silly_billy; break;
								case 3: stage.note.x = note_x4k_silly_billy_middle; break;
								default: stage.note.x = note_x4k_normal; break;
							}
						}
						for (int i = 0; i < 4; i++) {
							stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
						}
						// Set opponent Y positions to match player positions
						for (int i = 0; i < 4; i++) {
							if (stage.prefs.downscroll)
							{
								stage.note.y[i + 4] = FIXED_DEC(12, 1);
							}
							else
							{
								stage.note.y[i + 4] = FIXED_DEC(-126, 1);
							}
						}
						note_anims = note_anims4k;
						note_key = note_key4k;
						stage.note.size = 32;
					break;
					//5 Key
					case 5:
						if (stage.stage_id == StageId_4_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
						{
							stage.note.x = note_x5k_flipped; // Use note_x5kflip when condition is true
						}
						else if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
						{
							stage.note.x = note_x5k_silly_billy_middle; // Use note_x5kflip when condition is true
						}
						else
						{
							stage.note.x = note_x5k_normal;
						}
						for (int i = 0; i < 5; i++) {
							stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
						}
						// Set opponent Y positions to match player positions
						for (int i = 0; i < 5; i++) {
							if (stage.prefs.downscroll)
							{
								stage.note.y[i + 5] = FIXED_DEC(12, 1);
							}
							else
							{
								stage.note.y[i + 5] = FIXED_DEC(-126, 1);
							}
						}
						note_anims = note_anims5k;
						note_key = note_key5k;
						stage.note.size = 32;
					break;
					//6 Key
					case 6:
						if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
						{
							stage.note.x = note_x6k_silly_billy_middle; // Use note_x5kflip when condition is true
						}
						else
						{
							switch (note_offset_type) {
								case 0: stage.note.x = note_x6k_normal; break;
								case 1: stage.note.x = note_x6k_flipped; break;
								case 2: stage.note.x = note_x6k_silly_billy; break;
								case 3: stage.note.x = note_x6k_silly_billy_middle; break;
								default: stage.note.x = note_x6k_normal; break;
							}
						}
						for (int i = 0; i < 6; i++) {
							stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
						}
						// Set opponent Y positions to match player positions
						for (int i = 0; i < 6; i++) {
							if (stage.prefs.downscroll)
							{
								stage.note.y[i + 6] = FIXED_DEC(12, 1);
							}
							else
							{
								stage.note.y[i + 6] = FIXED_DEC(-126, 1);
							}
						}
						note_anims = note_anims6k;
						note_key = note_key6k;
						stage.note.size = 24;
					break;
					//7 Key
					case 7:
						if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
						{
							stage.note.x = note_x7k_silly_billy_middle; // Use note_x5kflip when condition is true
						}
						else
						{
							switch (note_offset_type) {
								case 0: stage.note.x = note_x7k_normal; break;
								case 1: stage.note.x = note_x7k_flipped; break;
								case 2: stage.note.x = note_x7k_silly_billy; break;
								case 3: stage.note.x = note_x7k_silly_billy_middle; break;
								default: stage.note.x = note_x7k_normal; break;
							}
						}
						for (int i = 0; i < 7; i++) {
							stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
						}
						// Set opponent Y positions to match player positions
						for (int i = 0; i < 7; i++) {
							if (stage.prefs.downscroll)
							{
								stage.note.y[i + 7] = FIXED_DEC(12, 1);
							}
							else
							{
								stage.note.y[i + 7] = FIXED_DEC(-126, 1);
							}
						}
						note_anims = note_anims7k;
						note_key = note_key7k;
						stage.note.size = 24;
					break;
					//9 Key
					case 9:
						if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
						{
							stage.note.x = note_x9k_silly_billy_middle; // Use note_x5kflip when condition is true
						}
						else
						{
							switch (note_offset_type) {
								case 0: stage.note.x = note_x9k_normal; break;
								case 1: stage.note.x = note_x9k_flipped; break;
								case 2: stage.note.x = note_x9k_silly_billy; break;
								case 3: stage.note.x = note_x9k_silly_billy_middle; break;
								default: stage.note.x = note_x9k_normal; break;
							}
						}
						for (int i = 0; i < 9; i++) {
							stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
						}
						// Set opponent Y positions to match player positions
						for (int i = 0; i < 9; i++) {
							if (stage.prefs.downscroll)
							{
								stage.note.y[i + 9] = FIXED_DEC(12, 1);
							}
							else
							{
								stage.note.y[i + 9] = FIXED_DEC(-126, 1);
							}
						}
						note_anims = note_anims9k;
						note_key = note_key9k;
						stage.note.size = 16;
					break;
					//4 Key
					default:    
						switch (note_offset_type) {
							case 0: stage.note.x = note_x4k_normal; break;
							case 1: stage.note.x = note_x4k_flipped; break;
							case 2: stage.note.x = note_x4k_silly_billy; break;
							case 3: stage.note.x = note_x4k_silly_billy_middle; break;
							default: stage.note.x = note_x4k_normal; break;
						}
						for (int i = 0; i < 4; i++) {
							stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
						}
						// Set opponent Y positions to match player positions
						for (int i = 0; i < 4; i++) {
							if (stage.prefs.downscroll)
							{
								stage.note.y[i + 4] = FIXED_DEC(12, 1);
							}
							else
							{
								stage.note.y[i + 4] = FIXED_DEC(-126, 1);
							}
						}
						note_anims = note_anims4k;
						note_key = note_key4k;
						stage.note.size = 32;
					break;
				}
			}
			else
			{
				switch (stage.keys)
				{
					//4 Key
					case 4:
					if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
					{
						stage.note.x = note_x4k_silly_billy_middle; // Use silly billy + middle scroll
					}
					else
					{
						switch (note_offset_type) {
							case 0: stage.note.x = note_x4k_normal; break;
							case 1: stage.note.x = note_x4k_flipped; break;
							case 2: stage.note.x = note_x4k_silly_billy; break;
							case 3: stage.note.x = note_x4k_silly_billy_middle; break;
							default: stage.note.x = note_x4k_normal; break;
						}
					}
					for (int i = 0; i < 4; i++) {
						stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					for (int i = 0; i < 4; i++) {
						stage.note.y[i + 4] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					note_anims = note_anims4k;
					note_key = note_key4k;
					stage.note.size = 32;
				break;
				//5 Key
				case 5:
					if (stage.stage_id == StageId_4_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
					{
						stage.note.x = note_x5k_flipped; // Use note_x5kflip when condition is true
					}
					else if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
					{
						stage.note.x = note_x5k_silly_billy_middle; // Use note_x5kflip when condition is true
					}
					else
					{
						stage.note.x = note_x5k_normal;
					}
					for (int i = 0; i < 5; i++) {
						stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					for (int i = 0; i < 5; i++) {
						stage.note.y[i + 5] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					note_anims = note_anims5k;
					note_key = note_key5k;
					stage.note.size = 32;
				break;
				//6 Key
				case 6:
					if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
					{
						stage.note.x = note_x6k_silly_billy_middle; // Use note_x5kflip when condition is true
					}
					else
					{
						switch (note_offset_type) {
							case 0: stage.note.x = note_x6k_normal; break;
							case 1: stage.note.x = note_x6k_flipped; break;
							case 2: stage.note.x = note_x6k_silly_billy; break;
							case 3: stage.note.x = note_x6k_silly_billy_middle; break;
							default: stage.note.x = note_x6k_normal; break;
						}
					}
					for (int i = 0; i < 6; i++) {
						stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					for (int i = 0; i < 6; i++) {
						stage.note.y[i + 6] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					note_anims = note_anims6k;
					note_key = note_key6k;
					stage.note.size = 24;
				break;
				//7 Key
				case 7:
					if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
					{
						stage.note.x = note_x7k_silly_billy_middle; // Use note_x5kflip when condition is true
					}
					else
					{
						switch (note_offset_type) {
							case 0: stage.note.x = note_x7k_normal; break;
							case 1: stage.note.x = note_x7k_flipped; break;
							case 2: stage.note.x = note_x7k_silly_billy; break;
							case 3: stage.note.x = note_x7k_silly_billy_middle; break;
							default: stage.note.x = note_x7k_normal; break;
						}
					}
					for (int i = 0; i < 7; i++) {
						stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					for (int i = 0; i < 7; i++) {
						stage.note.y[i + 7] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					note_anims = note_anims7k;
					note_key = note_key7k;
					stage.note.size = 24;
				break;
				//9 Key
				case 9:
					if (stage.stage_id == StageId_5_7 && stage.song_step >= 3621 && stage.song_step <= 3888)
					{
						stage.note.x = note_x9k_silly_billy_middle; // Use note_x5kflip when condition is true
					}
					else
					{
						switch (note_offset_type) {
							case 0: stage.note.x = note_x9k_normal; break;
							case 1: stage.note.x = note_x9k_flipped; break;
							case 2: stage.note.x = note_x9k_silly_billy; break;
							case 3: stage.note.x = note_x9k_silly_billy_middle; break;
							default: stage.note.x = note_x9k_normal; break;
						}
					}
					for (int i = 0; i < 9; i++) {
						stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					for (int i = 0; i < 9; i++) {
						stage.note.y[i + 9] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					note_anims = note_anims9k;
					note_key = note_key9k;
					stage.note.size = 16;
				break;
				//4 Key
				default:    
					switch (note_offset_type) {
						case 0: stage.note.x = note_x4k_normal; break;
						case 1: stage.note.x = note_x4k_flipped; break;
						case 2: stage.note.x = note_x4k_silly_billy; break;
						case 3: stage.note.x = note_x4k_silly_billy_middle; break;
						default: stage.note.x = note_x4k_normal; break;
					}
					for (int i = 0; i < 4; i++) {
						stage.note.y[i] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					// Set opponent Y positions to match player positions
					for (int i = 0; i < 4; i++) {
						stage.note.y[i + 4] = FIXED_DEC(32 - SCREEN_HEIGHT2, 1);
					}
					note_anims = note_anims4k;
					note_key = note_key4k;
					stage.note.size = 32;
				break;
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
			
			//drain effect
			if (drain > 0)
			{
				drain -= 1;
				if (stage.player_state[0].health >= 1)
					stage.player_state[0].health -= 45;
			}

			//rotate effect
			stage.camera.ta -= 1;
			if (stage.camera.ta <= 0)
				stage.camera.ta = 0;

			stage.camera.hudta -= 1;
			if (stage.camera.hudta <= 0)
				stage.camera.hudta = 0;

			//Clear per-frame flags
			stage.flag &= ~(STAGE_FLAG_JUST_STEP | STAGE_FLAG_SCORE_REFRESH);

			// Consume swap grace period frames
			if (stage.swap_grace_frames > 0)
				stage.swap_grace_frames--;

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

						if (currentDisc == 1) {
							Audio_PlayXA_TrackDisc1(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 0);
						}
						if (currentDisc == 2) {
							Audio_PlayXA_TrackDisc2(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 0);
						}
						if (currentDisc == 3) {
							Audio_PlayXA_TrackDisc3(stage.stage_def->music_track, 0x40, stage.stage_def->music_channel, false, 0);
						}
							
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
					fixed_t audio_time_pof;

					if (stage.movie_is_playing)
						audio_time_pof = stage.audio_last_pos_before_movie + stage.movie_pos;
					else
						audio_time_pof = (fixed_t)Audio_TellXA_Milli();
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
                    // XA not playing; continue local timing for botplay/scroll until chart ends
                    u16 last_note_pos = 0;
                    for (Note *n = stage.notes; n->pos != 0xFFFF; n++)
                        last_note_pos = n->pos;

                    // Keep playing true while chart hasn't ended yet
                    playing = ((stage.note_scroll >> FIXED_SHIFT) < last_note_pos);

                    // Advance local song_time using timer to keep scroll going
                    stage.song_time += timer_dt;
                    next_scroll = ((fixed_t)stage.step_base << FIXED_SHIFT) + FIXED_MUL(stage.song_time - stage.time_base, stage.step_crochet);

                    // If chart has ended now, perform transition
                    if (!playing)
                    {
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

				// Example: mid-song swap from Bopeebo (1_1) to Pico (3_1) at step 64
				/*if ((stage.flag & STAGE_FLAG_JUST_STEP) &&
					stage.stage_id == StageId_1_1 &&
					stage.song_step == 64)
				{
					Stage_RequestSwapTo(StageId_3_1, STAGE_SWAP_ALL);
				}*/
				
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
			{
				if (stage.intro)
				{
					Stage_CountDown();
				}
			}
			
			switch (stage.stage_id)
			{
				case StageId_1_1:
					stage.intro = true;
				break;
				case StageId_1_2:
					stage.intro = true;
				break;
				case StageId_1_3:
					stage.intro = true;
				break;
				case StageId_1_4:
					stage.intro = true;
				break;
				case StageId_2_1:
					stage.intro = true;
				break;
				case StageId_2_2:
					stage.intro = true;
				break;
				case StageId_2_3:
					stage.intro = true;
				break;
				case StageId_3_1:
					stage.intro = true;
				break;
				case StageId_3_2:
					stage.intro = true;
				break;
				case StageId_3_3:
					stage.intro = true;
				break;
				case StageId_4_1:
					stage.intro = true;
				break;
				case StageId_4_2:
					stage.intro = true;
				break;
				case StageId_4_3:
					stage.intro = true;
				break;
				case StageId_4_4:
					stage.intro = true;					
				break;
				case StageId_4_5:
					stage.intro = false;
				break;
				case StageId_4_6:
					stage.intro = false;	
				break;
				case StageId_4_7:
					stage.intro = true;						
				break;
				case StageId_5_1:
					stage.intro = false;
				break;
				case StageId_5_2:
					stage.intro = false;
				break;
				case StageId_5_3:
					stage.intro = false;
				break;
				case StageId_5_4:
					stage.intro = false;
				break;
				case StageId_5_5:
					stage.intro = false;
				break;
				case StageId_5_6:
					stage.intro = false;
				break;
				case StageId_5_7:
					stage.intro = false;
				break;
				default:
					stage.intro = true;
				break;
			}
			
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
			{
				if (stage.stage_id == StageId_5_2)
					Stage_OrangeTimerTick();
				else
					Stage_TimerTick();
			}

			if (stage.prefs.botplay)
			{
				//Draw skill issue mode (botplay)
				RECT skill_issue_src = {188, 111, 67, 16};
				RECT_FIXED skill_issue_dst = {
				FIXED_DEC(-33,1), 
				FIXED_DEC(-60,1), 
				FIXED_DEC(67,1), 
				FIXED_DEC(16,1)
			};

				skill_issue_dst.y += stage.noteshakey;
				skill_issue_dst.x += stage.noteshakex;
				
				if (!stage.prefs.debug)
					Stage_DrawTex(&stage.tex_hud0, &skill_issue_src, &skill_issue_dst, stage.bump, stage.camera.hudangle);
			}
			
			//Tick note splashes
			ObjectList_Tick(&stage.objlist_splash);
				
			//Draw stage notes
			Stage_DrawNotes(true);
			
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
							stage.player_state[1].arrow_hitan[Stage_GetNoteType(note) % stage.keys] = stage.step_time;
							Stage_StartVocal();
							if (note->type & NOTE_FLAG_SUSTAIN)
								opponent_snote = note_anims[Stage_GetNoteType(note) % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0];
							else
								opponent_anote = note_anims[Stage_GetNoteType(note) % stage.keys][(note->type & NOTE_FLAG_ALT_ANIM) != 0];
							note->type |= NOTE_FLAG_HIT;
						}
					}
					
					if (opponent_anote != CharAnim_Idle && stage.player_state[1].character != NULL)
						stage.player_state[1].character->set_anim(stage.player_state[1].character, opponent_anote);
					else if (opponent_snote != CharAnim_Idle && stage.player_state[1].character != NULL)
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
				if (stage.movie_is_playing == true || stage.prefs.mode < StageMode_2P)
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
					
					//Draw health bar
					if (stage.stage_id >= StageId_1_1 && stage.stage_id <= StageId_4_7)
					{
						if (stage.player != NULL)
							Stage_DrawHealth(stage.player_state[0].health, stage.player->health_i, 1);
						if (stage.opponent != NULL)
							Stage_DrawHealth(stage.player_state[0].health, stage.opponent->health_i, -1);
					
						if (stage.player_state[1].character != NULL)
							Stage_DrawHealthBar(251 - (251 * stage.player_state[0].health / 20000), stage.player_state[1].character->health_bar);
						if (stage.player_state[0].character != NULL)
							Stage_DrawHealthBar(251, stage.player_state[0].character->health_bar);
					}
					else if (stage.stage_id == StageId_5_1)
					{
						if (stage.player != NULL)
							Stage_DrawHealth(stage.player_state[0].health, stage.player->health_i, 1);
						if (stage.opponent != NULL)
							Stage_DrawHealth(stage.player_state[0].health, stage.opponent->health_i, -1);
					
						if (stage.player_state[1].character != NULL)
							Stage_DrawHealthBar(251 - (251 * stage.player_state[0].health / 20000), stage.player_state[1].character->health_bar);
						if (stage.player_state[0].character != NULL)
							Stage_DrawHealthBar(251, stage.player_state[0].character->health_bar);
					}
					else if (stage.stage_id == StageId_5_2)
					{
						if (stage.player != NULL)
							Stage_DrawOrangeHealth(stage.player_state[0].health, stage.player->health_i, 1);
						if (stage.opponent != NULL)
							Stage_DrawOrangeHealth(stage.player_state[0].health, stage.opponent->health_i, -1);
					
						//Draw health bar
						RECT health_fill = {0, 208, 256 - (256 * stage.player_state[0].health / 20000), 48};
						RECT health_back = {0, 160, 256, 48};
						RECT_FIXED health_dst = {FIXED_DEC(-128,1), (SCREEN_HEIGHT2 - 64) << FIXED_SHIFT, 0, FIXED_DEC(48,1)};
						if (stage.prefs.downscroll)
							health_dst.y = -health_dst.y - health_dst.h;
						
						health_dst.w = health_fill.w << FIXED_SHIFT;
						Stage_DrawTex(&stage.tex_hud1, &health_fill, &health_dst, stage.bump, stage.camera.hudangle);
						health_dst.w = health_back.w << FIXED_SHIFT;
						Stage_DrawTex(&stage.tex_hud1, &health_back, &health_dst, stage.bump, stage.camera.hudangle);
					}
					else if (stage.stage_id >= StageId_5_3 && stage.stage_id <= StageId_5_7)
					{
						if (stage.player != NULL)
							Stage_DrawHealth(stage.player_state[0].health, stage.player->health_i, 1);
						if (stage.opponent != NULL)
							Stage_DrawHealth(stage.player_state[0].health, stage.opponent->health_i, -1);
					
						if (stage.player_state[1].character != NULL)
							Stage_DrawHealthBar(251 - (251 * stage.player_state[0].health / 20000), stage.player_state[1].character->health_bar);
						if (stage.player_state[0].character != NULL)
							Stage_DrawHealthBar(251, stage.player_state[0].character->health_bar);
					}
				}
				
				if (stage.movie_is_playing)
					return;
				
				for (int i = 0; i < ((stage.prefs.mode >= StageMode_2P) ? 2 : 1); i++)
				{
					if (stage.movie_is_playing)
						return;
					
					PlayerState *this = &stage.player_state[i];
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
				if (stage.opponent != NULL)
					stage.opponent->tick(stage.opponent);
				if (stage.player != NULL)
					stage.player->tick(stage.player);
			}
			else
			{
				if (stage.player != NULL)
					stage.player->tick(stage.player);
				if (stage.opponent != NULL)
					stage.opponent->tick(stage.opponent);
			}
      		if (stage.player2 != NULL)
				stage.player2->tick(stage.player2);
			if (stage.opponent2 != NULL)
				stage.opponent2->tick(stage.opponent2);
			
			//Draw stage middle
			if (stage.back->draw_md != NULL)
				stage.back->draw_md(stage.back);
			
			//Draw stage notes (background)
			Stage_DrawNotes(false);

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
				if (stage.stage_id >= StageId_1_1 && stage.stage_id <= StageId_3_3)
				{
					currentDisc = 1;
				}
				if (stage.stage_id >= StageId_4_1 && stage.stage_id <= StageId_4_7)
				{
					currentDisc = 2;
				}
				if (stage.stage_id >= StageId_5_1 && stage.stage_id <= StageId_5_7)
				{
					currentDisc = 3;
				}
				if (currentDisc == 1) {
					stage.state = StageState_DeadRetry;
					Audio_PlayXA_TrackDisc1(XA_GameOver_Disc1, 0x40, 1, true, 0);
				}
				if (currentDisc == 2) {
					stage.state = StageState_DeadRetry;
					Audio_PlayXA_TrackDisc2(XA_GameOver_Disc2, 0x40, 1, true, 0);
				}
				if (currentDisc == 3) {
					stage.state = StageState_DeadRetry;
					Audio_PlayXA_TrackDisc3(XA_GameOver_Disc3, 0x40, 1, true, 0);
				}
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