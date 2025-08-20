/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "apple.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../random.h"
#include "../main.h"

//Boyfriend skull fragments
static SkullFragment char_apple_skull[15] = {
	{ 1 * 8, -87 * 8, -13, -13},
	{ 9 * 8, -88 * 8,   5, -22},
	{18 * 8, -87 * 8,   9, -22},
	{26 * 8, -85 * 8,  13, -13},
	
	{-3 * 8, -82 * 8, -13, -11},
	{ 8 * 8, -85 * 8,  -9, -15},
	{20 * 8, -82 * 8,   9, -15},
	{30 * 8, -79 * 8,  13, -11},
	
	{-1 * 8, -74 * 8, -13, -5},
	{ 8 * 8, -77 * 8,  -9, -9},
	{19 * 8, -75 * 8,   9, -9},
	{26 * 8, -74 * 8,  13, -5},
	
	{ 5 * 8, -73 * 8, -5, -3},
	{14 * 8, -76 * 8,  9, -6},
	{26 * 8, -67 * 8, 15, -3},
};

//Boyfriend player types
enum
{
	Apple_ArcMain_Apple0,
	Apple_ArcMain_Apple1,
	Apple_ArcMain_Apple2,
	Apple_ArcMain_Apple3,
	Apple_ArcMain_Apple4,
	Apple_ArcMain_Apple5,
	Apple_ArcMain_Real0,
	Apple_ArcMain_Real1,
	Apple_ArcMain_Real2,
	Apple_ArcMain_Real3,
	Apple_ArcMain_Real4,
	Apple_ArcMain_Real5,
	Apple_ArcMain_Real6,
	Apple_ArcMain_Real7,
	Apple_ArcMain_Real8,
	Apple_ArcMain_Dead0, //BREAK
	
	Apple_ArcMain_Max,
};

enum
{
	Apple_ArcDead_Dead1, //Mic Drop
	Apple_ArcDead_Dead2, //Twitch
	Apple_ArcDead_Retry, //Retry prompt
	
	Apple_ArcDead_Max,
};

#define Apple_Arc_Max Apple_ArcMain_Max

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main, arc_dead;
	CdlFILE file_dead_arc; //dead.arc file position
	IO_Data arc_ptr[Apple_Arc_Max];
	
	Gfx_Tex tex, tex_retry;
	u8 frame, tex_id;
	
	u8 retry_bump;
	
	SkullFragment skull[COUNT_OF(char_apple_skull)];
	u8 skull_scale;
} Char_Apple;

static const u16 char_apple_icons[2][4] = {
	{128,0,64,64},
	{192,0,64,64}
};

static const u16 char_real_icons[2][4] = {
	{128,64,64,64},
	{192,64,64,64}
};

static const CharFrame char_apple_frame[] = {
	{Apple_ArcMain_Apple0, {  0,  0,102, 95}, {155,154}}, //0 idle 1
	{Apple_ArcMain_Apple0, {102,  0,101, 97}, {155,156}}, //1 idle 2
	{Apple_ArcMain_Apple0, {  0, 97,100, 99}, {155,158}}, //2 idle 3
	{Apple_ArcMain_Apple0, {100, 97, 99, 99}, {154,158}}, //3 idle 4
	{Apple_ArcMain_Apple1, {  0,  0, 99, 99}, {154,158}}, //4 idle 5
	{Apple_ArcMain_Apple1, { 99,  0, 99, 98}, {154,158}}, //5 idle 6
	{Apple_ArcMain_Apple1, {  0, 99, 99, 98}, {155,158}}, //6 idle 7

	{Apple_ArcMain_Apple1, { 99, 99, 96,102}, {159,164}}, //7 left 1
	{Apple_ArcMain_Apple2, {  0,  0, 99, 98}, {155,157}}, //8 left 2
	{Apple_ArcMain_Apple2, { 99,  0, 99, 98}, {154,157}}, //9 left 3
	{Apple_ArcMain_Apple2, {  0, 98,100, 98}, {155,157}}, //10 left 4

	{Apple_ArcMain_Apple2, {100, 98,104, 94}, {157,154}}, //11 down 1
	{Apple_ArcMain_Apple3, {  0,  0,100, 98}, {155,158}}, //12 down 2
	{Apple_ArcMain_Apple3, {100,  0,100, 97}, {155,157}}, //13 down 3
	{Apple_ArcMain_Apple3, {  0, 98,100, 97}, {155,157}}, //14 down 4

	{Apple_ArcMain_Apple3, {100, 98, 93,103}, {151,162}}, //15 up 1
	{Apple_ArcMain_Apple4, {  0,  0,101, 97}, {157,170}}, //16 up 2
	{Apple_ArcMain_Apple4, {101,  0,100, 99}, {156,169}}, //17 up 3
	{Apple_ArcMain_Apple4, {  0, 99,100, 99}, {155,166}}, //18 up 4

	{Apple_ArcMain_Apple4, {100, 99,104, 97}, {150,159}}, //19 right 1
	{Apple_ArcMain_Apple5, {  0,  0,102, 98}, {153,157}}, //20 right 2
	{Apple_ArcMain_Apple5, {102,  0,102, 97}, {154,156}}, //21 right 3
	
	{Apple_ArcMain_Real0, {  0,  0,102, 94}, {159,156}}, //22 idle 1
	{Apple_ArcMain_Real0, {102,  0,102, 93}, {159,156}}, //23 idle 2
	{Apple_ArcMain_Real0, {  0, 94,101, 93}, {158,156}}, //24 idle 3
	{Apple_ArcMain_Real0, {101, 94,100, 94}, {157,157}}, //25 idle 4
	{Apple_ArcMain_Real1, {  0,  0, 99, 95}, {157,158}}, //26 idle 5
	{Apple_ArcMain_Real1, { 99,  0,100, 96}, {157,158}}, //27 idle 6
	{Apple_ArcMain_Real1, {  0, 96, 99, 96}, {157,159}}, //28 idle 7
	{Apple_ArcMain_Real1, { 99, 96, 99, 97}, {157,160}}, //29 idle 8
	{Apple_ArcMain_Real2, {  0,  0, 99, 97}, {157,160}}, //30 idle 9
	{Apple_ArcMain_Real2, { 99,  0, 99, 97}, {156,160}}, //31 idle 10
	{Apple_ArcMain_Real2, {  0, 97, 99, 97}, {157,160}}, //32 idle 11

	{Apple_ArcMain_Real2, { 99, 97, 99, 97}, {164,160}}, //33 left 1
	{Apple_ArcMain_Real3, {  0,  0, 99, 98}, {161,160}}, //34 left 2
	{Apple_ArcMain_Real3, { 99,  0,100, 98}, {159,160}}, //35 left 3
	{Apple_ArcMain_Real3, {  0, 98,100, 98}, {159,160}}, //36 left 4
	{Apple_ArcMain_Real3, {100, 98, 99, 98}, {158,160}}, //37 left 5
	{Apple_ArcMain_Real4, {  0,  0, 99, 98}, {158,159}}, //38 left 6

	{Apple_ArcMain_Real4, { 99,  0,100, 97}, {159,156}}, //39 down 1
	{Apple_ArcMain_Real4, {  0, 98, 99, 98}, {159,157}}, //40 down 2
	{Apple_ArcMain_Real4, { 99, 98, 99, 98}, {159,159}}, //41 down 3
	{Apple_ArcMain_Real5, {  0,  0, 99, 98}, {159,159}}, //42 down 4
	{Apple_ArcMain_Real5, { 99,  0, 99, 98}, {158,159}}, //43 down 5
	{Apple_ArcMain_Real5, {  0, 98, 99, 97}, {159,159}}, //44 down 6

	{Apple_ArcMain_Real5, { 99, 98, 99, 96}, {157,161}}, //45 up 1
	{Apple_ArcMain_Real6, {  0,  0,100, 96}, {158,159}}, //46 up 2
	{Apple_ArcMain_Real6, {100,  0,100, 96}, {158,157}}, //47 up 3
	{Apple_ArcMain_Real6, {  0, 96, 99, 96}, {158,157}}, //48 up 4
	{Apple_ArcMain_Real6, { 99, 96, 99, 96}, {157,157}}, //49 up 5
	{Apple_ArcMain_Real7, {  0,  0,100, 96}, {157,157}}, //50 up 6

	{Apple_ArcMain_Real7, {100,  0, 99, 97}, {151,160}}, //51 right 1
	{Apple_ArcMain_Real7, {  0, 97, 99, 96}, {153,159}}, //52 right 2
	{Apple_ArcMain_Real7, { 99, 97, 99, 96}, {156,159}}, //53 right 3
	{Apple_ArcMain_Real8, {  0,  0, 99, 96}, {156,159}}, //54 right 4
	{Apple_ArcMain_Real8, { 99,  0, 99, 96}, {157,159}}, //55 right 5
	{Apple_ArcMain_Real8, {  0, 96, 99, 96}, {157,158}}, //56 right 6

	{Apple_ArcMain_Dead0, {0, 0, 104, 104}, {0,50}}, //57 dead0 0
	
	{Apple_ArcDead_Dead1, {0, 0, 104, 104}, {0,50}}, //58 dead1 0
	
	{Apple_ArcDead_Dead2, {0, 0, 104, 104}, {0,50}}, //59 dead2 body twitch 0
};

static const Animation char_apple_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5,  6, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 7,  8,  9, 10, ASCR_BACK, 0}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{2, (const u8[]){11, 12, 13, 14, ASCR_BACK, 0}},             //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_DownAlt
	{2, (const u8[]){15, 16, 17, 18, ASCR_BACK, 0}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{2, (const u8[]){19, 20, 21, ASCR_BACK, 0}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing

	{2, (const u8[]){ 7,  8,  9, 10, ASCR_BACK, 0}},     //PlayerAnim_LeftMiss
	{2, (const u8[]){11, 12, 13, 14, ASCR_BACK, 0}},     //PlayerAnim_DownMiss
	{2, (const u8[]){15, 16, 17, 18, ASCR_BACK, 0}},     //PlayerAnim_UpMiss
	{2, (const u8[]){19, 20, 21, ASCR_BACK, 0}},     //PlayerAnim_RightMiss
	
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //PlayerAnim_Peace
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},      //PlayerAnim_Sweat
	
	{5, (const u8[]){57, 57, 57, 57, 57, 57, 57, 57, 57, 57, ASCR_CHGANI, PlayerAnim_Dead1}}, //PlayerAnim_Dead0
	{5, (const u8[]){57, ASCR_REPEAT}},                                                       //PlayerAnim_Dead1
	{3, (const u8[]){58, 58, 58, 58, 58, 58, 58, 58, 58, 58, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2
	{3, (const u8[]){58, ASCR_REPEAT}},                                                       //PlayerAnim_Dead3
	{3, (const u8[]){59, 59, 59, 59, 59, 59, 59, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead4
	{3, (const u8[]){59, 59, 59, 59, 59, 59, 59, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead5
	
	{10, (const u8[]){58, 58, 58, ASCR_BACK, 1}}, //PlayerAnim_Dead4
	{ 3, (const u8[]){59, 59, 59, ASCR_REPEAT}},  //PlayerAnim_Dead5
};

static const Animation char_real_anim[PlayerAnim_Max] = {
	{2, (const u8[]){22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){33, 34, 35, 36, 37, 38, ASCR_BACK, 0}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{2, (const u8[]){39, 40, 41, 42, 43, 44, ASCR_BACK, 0}},             //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_DownAlt
	{2, (const u8[]){45, 46, 47, 48, 49, 50, ASCR_BACK, 0}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{2, (const u8[]){51, 52, 53, 54, 55, 56, ASCR_BACK, 0}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing
	
	{2, (const u8[]){33, 34, 35, 36, 37, 38, ASCR_BACK, 0}},     //PlayerAnim_LeftMiss
	{2, (const u8[]){39, 40, 41, 42, 43, 44, ASCR_BACK, 0}},     //PlayerAnim_DownMiss
	{2, (const u8[]){45, 46, 47, 48, 49, 50, ASCR_BACK, 0}},     //PlayerAnim_UpMiss
	{2, (const u8[]){51, 52, 53, 54, 55, 56, ASCR_BACK, 0}},     //PlayerAnim_RightMiss
	
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //PlayerAnim_Peace
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},      //PlayerAnim_Sweat
	
	{5, (const u8[]){57, 57, 57, 57, 57, 57, 57, 57, 57, 57, ASCR_CHGANI, PlayerAnim_Dead1}}, //PlayerAnim_Dead0
	{5, (const u8[]){57, ASCR_REPEAT}},                                                       //PlayerAnim_Dead1
	{3, (const u8[]){58, 58, 58, 58, 58, 58, 58, 58, 58, 58, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2
	{3, (const u8[]){58, ASCR_REPEAT}},                                                       //PlayerAnim_Dead3
	{3, (const u8[]){59, 59, 59, 59, 59, 59, 59, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead4
	{3, (const u8[]){59, 59, 59, 59, 59, 59, 59, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead5
	
	{10, (const u8[]){58, 58, 58, ASCR_BACK, 1}}, //PlayerAnim_Dead4
	{3, (const u8[]){59, 59, 59, ASCR_REPEAT}},  //PlayerAnim_Dead5
};

//Boyfriend player functions
void Char_Apple_SetFrame(void *user, u8 frame)
{
	Char_Apple *this = (Char_Apple*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_apple_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Apple_Tick(Character *character)
{
	Char_Apple *this = (Char_Apple*)character;
	
	if (stage.stage_id == StageId_5_2)
	{
		//Camera stuff
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 0)
		{
			this->character.focus_x = FIXED_DEC(-125, 1);
			this->character.focus_y = FIXED_DEC(-100, 1);
			this->character.focus_zoom = FIXED_DEC(125,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 1040)
		{
			this->character.focus_x = FIXED_DEC(-180, 1);
			this->character.focus_y = FIXED_DEC(-60, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 1816)
		{
			this->character.focus_x = FIXED_DEC(-150, 1);
			this->character.focus_y = FIXED_DEC(-100, 1);
			this->character.focus_zoom = FIXED_DEC(96,100);
		}
	}
	
	//Secret icon
	memcpy(this->character.health_i, char_apple_icons, sizeof(char_apple_icons));

	if (stage.stage_id == StageId_5_2)
	{
		switch(stage.song_step)
		{
			case 0:
				Animatable_Init(&this->character.animatable, char_apple_anim);
				break;
			case 784:
				Animatable_Init(&this->character.animatable, char_real_anim);
				break;
		}
	}
	
	//Handle animation updates
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0 ||
	    (character->animatable.anim != CharAnim_Left &&
	     character->animatable.anim != CharAnim_LeftAlt &&
	     character->animatable.anim != CharAnim_Down &&
	     character->animatable.anim != CharAnim_DownAlt &&
	     character->animatable.anim != CharAnim_Up &&
	     character->animatable.anim != CharAnim_UpAlt &&
	     character->animatable.anim != CharAnim_Right &&
	     character->animatable.anim != CharAnim_RightAlt))
		Character_CheckEndSing(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		//Perform idle dance
		if (Animatable_Ended(&character->animatable) &&
			(character->animatable.anim != CharAnim_Left &&
		     character->animatable.anim != CharAnim_LeftAlt &&
		     character->animatable.anim != PlayerAnim_LeftMiss &&
		     character->animatable.anim != CharAnim_Down &&
		     character->animatable.anim != CharAnim_DownAlt &&
		     character->animatable.anim != PlayerAnim_DownMiss &&
		     character->animatable.anim != CharAnim_Up &&
		     character->animatable.anim != CharAnim_UpAlt &&
		     character->animatable.anim != PlayerAnim_UpMiss &&
		     character->animatable.anim != CharAnim_Right &&
		     character->animatable.anim != CharAnim_RightAlt &&
		     character->animatable.anim != PlayerAnim_RightMiss) &&
			(stage.song_step & 0x7) == 0)
			character->set_anim(character, CharAnim_Idle);
	}
	
	if (stage.stage_id == StageId_5_2 && stage.song_step >= 784)
	memcpy(this->character.health_i, char_real_icons, sizeof(char_real_icons));

	//Retry screen
	if (character->animatable.anim >= PlayerAnim_Dead3)
	{
		//Tick skull fragments
		if (this->skull_scale)
		{
			SkullFragment *frag = this->skull;
			for (size_t i = 0; i < COUNT_OF_MEMBER(Char_Apple, skull); i++, frag++)
			{
				//Draw fragment
				RECT frag_src = {
					(i & 1) ? 112 : 96,
					(i >> 1) << 4,
					16,
					16
				};
				fixed_t skull_dim = (FIXED_DEC(16,1) * this->skull_scale) >> 6;
				fixed_t skull_rad = skull_dim >> 1;
				RECT_FIXED frag_dst = {
					character->x + (((fixed_t)frag->x << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.x,
					character->y + (((fixed_t)frag->y << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.y,
					skull_dim,
					skull_dim,
				};
				Stage_DrawTex(&this->tex_retry, &frag_src, &frag_dst, FIXED_MUL(stage.camera.zoom, stage.bump), stage.camera.angle);
				
				//Move fragment
				frag->x += frag->xsp;
				frag->y += ++frag->ysp;
			}
			
			//Decrease scale
			this->skull_scale--;
		}
		
		//Draw input options
		u8 input_scale = 16 - this->skull_scale;
		if (input_scale > 16)
			input_scale = 0;
		
		RECT button_src = {
			 0, 96,
			16, 16
		};
		RECT_FIXED button_dst = {
			character->x - FIXED_DEC(32,1) - stage.camera.x,
			character->y - FIXED_DEC(88,1) - stage.camera.y,
			(FIXED_DEC(16,1) * input_scale) >> 4,
			FIXED_DEC(16,1),
		};
		
		//Cross - Retry
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump), stage.camera.angle);
		
		//Circle - Blueball
		button_src.x = 16;
		button_dst.y += FIXED_DEC(56,1);
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump), stage.camera.angle);
		
		//Draw 'RETRY'
		u8 retry_frame;
		
		if (character->animatable.anim == PlayerAnim_Dead6)
		{
			//Selected retry
			retry_frame = 2 - (this->retry_bump >> 3);
			if (retry_frame >= 3)
				retry_frame = 0;
			if (this->retry_bump & 2)
				retry_frame += 3;
			
			if (++this->retry_bump == 0xFF)
				this->retry_bump = 0xFD;
		}
		else
		{
			//Idle
			retry_frame = 1 +  (this->retry_bump >> 2);
			if (retry_frame >= 3)
				retry_frame = 0;
			
			if (++this->retry_bump >= 55)
				this->retry_bump = 0;
		}
		
		RECT retry_src = {
			(retry_frame & 1) ? 48 : 0,
			(retry_frame >> 1) << 5,
			48,
			32
		};
		RECT_FIXED retry_dst = {
			character->x -  FIXED_DEC(7,1) - stage.camera.x,
			character->y - FIXED_DEC(92,1) - stage.camera.y,
			FIXED_DEC(48,1),
			FIXED_DEC(32,1),
		};
		Stage_DrawTex(&this->tex_retry, &retry_src, &retry_dst, FIXED_MUL(stage.camera.zoom, stage.bump), stage.camera.angle);
	}
	
	//Animate and draw character
	Animatable_Animate(&character->animatable, (void*)this, Char_Apple_SetFrame);
	Character_Draw(character, &this->tex, &char_apple_frame[this->frame]);
}

void Char_Apple_SetAnim(Character *character, u8 anim)
{
	Char_Apple *this = (Char_Apple*)character;
	
	//Perform animation checks
	switch (anim)
	{
		case PlayerAnim_Dead0:
			//Begin reading dead.arc and adjust focus
			this->arc_dead = IO_AsyncReadFile(&this->file_dead_arc);
			character->focus_x = FIXED_DEC(0,1);
			character->focus_y = FIXED_DEC(-40,1);
			character->focus_zoom = FIXED_DEC(125,100);
			break;
		case PlayerAnim_Dead2:
			//Unload main.arc
			Mem_Free(this->arc_main);
			this->arc_main = this->arc_dead;
			this->arc_dead = NULL;
			
			//Find dead.arc files
			const char **pathp = (const char *[]){
				"dead1.tim", //Apple_ArcDead_Dead1
				"dead2.tim", //Apple_ArcDead_Dead2
				"retry.tim", //Apple_ArcDead_Retry
				NULL
			};
			IO_Data *arc_ptr = this->arc_ptr;
			for (; *pathp != NULL; pathp++)
				*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
			
			//Load retry art
			Gfx_LoadTex(&this->tex_retry, this->arc_ptr[Apple_ArcDead_Retry], 0);
			break;
	}
	
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Apple_Free(Character *character)
{
	Char_Apple *this = (Char_Apple*)character;
	
	//Free art
	Mem_Free(this->arc_main);
	Mem_Free(this->arc_dead);
}

Character *Char_Apple_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_Apple *this = Mem_Alloc(sizeof(Char_Apple));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Apple_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Apple_Tick;
	this->character.set_anim = Char_Apple_SetAnim;
	this->character.free = Char_Apple_Free;
	
	Animatable_Init(&this->character.animatable, char_apple_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = CHAR_SPEC_MISSANIM;
	
	memcpy(this->character.health_i, char_apple_icons, sizeof(char_apple_icons));
	
	this->character.health_bar = 0xFF29B5D6;
	
	this->character.focus_x = FIXED_DEC(-125,1);
	this->character.focus_y = FIXED_DEC(-100,1);
	this->character.focus_zoom = FIXED_DEC(125,100);
	
	this->character.size = FIXED_DEC(50,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\APPLE.ARC;1");
	this->arc_dead = NULL;
	IO_FindFile(&this->file_dead_arc, "\\CHAR\\APPDED.ARC;1");
	
	const char **pathp = (const char *[]){
		"apple0.tim",   //Apple_ArcMain_Apple0
		"apple1.tim",   //Apple_ArcMain_Apple1
		"apple2.tim",   //Apple_ArcMain_Apple2
		"apple3.tim",   //Apple_ArcMain_Apple3
		"apple4.tim",   //Apple_ArcMain_Apple4
		"apple5.tim",   //Apple_ArcMain_Apple5
		"real0.tim",
		"real1.tim",
		"real2.tim",
		"real3.tim",
		"real4.tim",
		"real5.tim",
		"real6.tim",
		"real7.tim",
		"real8.tim",
		"dead0.tim", //Apple_ArcMain_Dead0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	//Initialize player state
	this->retry_bump = 0;
	
	//Copy skull fragments
	memcpy(this->skull, char_apple_skull, sizeof(char_apple_skull));
	this->skull_scale = 64;
	
	SkullFragment *frag = this->skull;
	for (size_t i = 0; i < COUNT_OF_MEMBER(Char_Apple, skull); i++, frag++)
	{
		//Randomize trajectory
		frag->xsp += RandomRange(-4, 4);
		frag->ysp += RandomRange(-2, 2);
	}
	
	return (Character*)this;
}
