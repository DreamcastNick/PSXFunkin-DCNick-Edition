/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "orange.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Orange character structure
enum
{
	Orange_ArcMain_Orange0,
	Orange_ArcMain_Orange1,
	Orange_ArcMain_Orange2,
	Orange_ArcMain_Orange3,
	Orange_ArcMain_Orange4,
	Orange_ArcMain_Orange5,
	Orange_ArcMain_Scary0,
	Orange_ArcMain_Scary1,
	Orange_ArcMain_Scary2,
	Orange_ArcMain_Scary3,
	Orange_ArcMain_Scary4,
	Orange_ArcMain_Scary5,
	Orange_ArcMain_Scary6,
	Orange_ArcMain_Scary7,
	Orange_ArcMain_Scary8,
	Orange_ArcMain_Scary9,
	Orange_ArcMain_Scary10,
	Orange_ArcMain_Scary11,
	Orange_ArcMain_Scary12,

	
	Orange_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Orange_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Orange;

static const u16 char_orange_icons[2][4] = {
	{0,0,64,64},
	{64,0,64,64}
};

static const u16 char_scary_icons[2][4] = {
	{0,64,64,64},
	{64,64,64,64}
};

//Orange character definitions
static const CharFrame char_orange_frame[] = {
	{Orange_ArcMain_Orange0, {  0,  0,100, 96}, {155,150}}, //0 idle 1
	{Orange_ArcMain_Orange0, {100,  0, 98, 98}, {153,152}}, //1 idle 2
	{Orange_ArcMain_Orange0, {  0, 98, 98, 99}, {153,153}}, //2 idle 3
	{Orange_ArcMain_Orange0, { 98, 98, 99,100}, {153,154}}, //3 idle 4
	{Orange_ArcMain_Orange1, {  0,  0, 98,100}, {153,154}}, //4 idle 5
	{Orange_ArcMain_Orange1, { 98,  0, 98, 99}, {153,154}}, //5 idle 6

	{Orange_ArcMain_Orange1, {  0,100, 98, 98}, {162,153}}, //6 left 1
	{Orange_ArcMain_Orange1, { 98,100, 98,100}, {155,154}}, //7 left 2
	{Orange_ArcMain_Orange2, {  0,  0, 99, 99}, {155,154}}, //8 left 3
	{Orange_ArcMain_Orange2, { 99,  0, 98, 99}, {154,154}}, //9 left 4

	{Orange_ArcMain_Orange2, {  0, 99,102, 96}, {156,148}}, //10 down 1
	{Orange_ArcMain_Orange2, {102, 99, 99, 99}, {156,153}}, //11 down 2
	{Orange_ArcMain_Orange3, {  0,  0, 99,100}, {155,154}}, //12 down 3
	{Orange_ArcMain_Orange3, { 99,  0, 98,100}, {155,154}}, //13 down 4

	{Orange_ArcMain_Orange3, {  0,100, 99,100}, {154,156}}, //14 up 1
	{Orange_ArcMain_Orange3, { 99,100, 99, 99}, {154,154}}, //15 up 2
	{Orange_ArcMain_Orange4, {  0,  0, 98,100}, {153,154}}, //16 up 3
	{Orange_ArcMain_Orange4, { 98,  0, 98, 99}, {154,153}}, //17 up 4

	{Orange_ArcMain_Orange4, {  0,100, 98, 99}, {150,155}}, //18 right 1
	{Orange_ArcMain_Orange4, { 98,100, 99, 99}, {153,154}}, //19 right 2
	{Orange_ArcMain_Orange5, {  0,  0, 99, 99}, {153,153}}, //20 right 3
	{Orange_ArcMain_Orange5, { 99,  0, 98, 99}, {153,153}}, //21 right 4

	{Orange_ArcMain_Scary0, {  0,  0,103,101}, {158,154}}, //22 idle 1
	{Orange_ArcMain_Scary0, {103,  0,103,101}, {158,154}}, //23 idle 2
	{Orange_ArcMain_Scary0, {  0,101,103,101}, {158,154}}, //24 idle 3
	{Orange_ArcMain_Scary0, {103,101,103,101}, {158,154}}, //25 idle 4

	{Orange_ArcMain_Scary1, {  0,  0,102,100}, {172,150}}, //26 left 1
	{Orange_ArcMain_Scary1, {102,  0, 99,104}, {157,155}}, //27 left 2
	{Orange_ArcMain_Scary1, {  0,104,104,100}, {158,153}}, //28 left 3
	{Orange_ArcMain_Scary1, {104,104,103,102}, {158,154}}, //29 left 4
	{Orange_ArcMain_Scary2, {  0,  0,103,101}, {157,153}}, //30 left 5
	{Orange_ArcMain_Scary2, {103,  0,104,101}, {158,153}}, //31 left 6

	{Orange_ArcMain_Scary2, {  0,101,100,122}, {156,141}}, //32 down 1
	{Orange_ArcMain_Scary2, {100,101,118, 83}, {166,139}}, //33 down 2
	{Orange_ArcMain_Scary3, {  0,  0,118, 83}, {166,139}}, //34 down 3
	{Orange_ArcMain_Scary3, {118,  0,106,103}, {159,152}}, //35 down 4
	{Orange_ArcMain_Scary3, {  0,103,106,102}, {158,151}}, //36 down 5
	{Orange_ArcMain_Scary3, {106,103,104,101}, {158,152}}, //37 down 6
	{Orange_ArcMain_Scary4, {  0,  0,104,101}, {158,152}}, //38 down 7
	{Orange_ArcMain_Scary4, {104,  0,103,102}, {158,152}}, //39 down 8
	{Orange_ArcMain_Scary4, {  0,102,103,102}, {158,152}}, //40 down 9
	{Orange_ArcMain_Scary4, {103,102,103,103}, {157,154}}, //41 down 10
	{Orange_ArcMain_Scary5, {  0,  0,103,102}, {157,153}}, //42 down 11
	{Orange_ArcMain_Scary5, {103,  0,103,103}, {158,154}}, //43 down 12
	{Orange_ArcMain_Scary5, {  0,103,103,102}, {158,153}}, //44 down 13

	{Orange_ArcMain_Scary5, {103,103,100,118}, {153,181}}, //45 up 1
	{Orange_ArcMain_Scary6, {  0,  0,107, 88}, {157,148}}, //46 up 2
	{Orange_ArcMain_Scary6, {107,  0,107, 87}, {158,147}}, //47 up 3
	{Orange_ArcMain_Scary6, {  0, 88, 99,104}, {154,156}}, //48 up 4
	{Orange_ArcMain_Scary6, { 99, 88,100,104}, {154,156}}, //49 up 5
	{Orange_ArcMain_Scary7, {  0,  0,105,100}, {156,153}}, //50 up 6
	{Orange_ArcMain_Scary7, {105,  0,106,100}, {157,153}}, //51 up 7
	{Orange_ArcMain_Scary7, {  0,100,103,102}, {156,154}}, //52 up 8
	{Orange_ArcMain_Scary7, {103,100,104,101}, {156,153}}, //53 up 9
	{Orange_ArcMain_Scary8, {  0,  0,103,102}, {156,155}}, //54 up 10
	{Orange_ArcMain_Scary8, {103,  0,103,102}, {156,155}}, //55 up 11
	{Orange_ArcMain_Scary8, {  0,102,103,102}, {156,155}}, //56 up 12
	{Orange_ArcMain_Scary8, {103,102,103,102}, {156,155}}, //57 up 13

	{Orange_ArcMain_Scary9, {  0,  0,112,108}, {131,155}}, //58 right 1
	{Orange_ArcMain_Scary9, {112,  0,112, 93}, {159,148}}, //59 right 2
	{Orange_ArcMain_Scary9, {  0,108,112, 93}, {158,148}}, //60 right 3
	{Orange_ArcMain_Scary9, {112,108,103,101}, {158,152}}, //61 right 4
	{Orange_ArcMain_Scary10, {  0,  0,103,101}, {158,152}}, //62 right 5
	{Orange_ArcMain_Scary10, {103,  0,103,101}, {159,152}}, //63 right 6
	{Orange_ArcMain_Scary10, {  0,101,103,101}, {159,152}}, //64 right 7
	{Orange_ArcMain_Scary10, {103,101,105, 99}, {160,151}}, //65 right 8
	{Orange_ArcMain_Scary11, {  0,  0,105, 99}, {160,151}}, //66 right 9
	{Orange_ArcMain_Scary11, {105,  0,104,101}, {159,152}}, //67 right 10
	{Orange_ArcMain_Scary11, {  0,101,104,101}, {159,152}}, //68 right 11
	{Orange_ArcMain_Scary11, {104,101,104,101}, {159,153}}, //69 right 12
	{Orange_ArcMain_Scary12, {  0,  0,103,102}, {158,153}}, //70 right 13
};

static const Animation char_orange_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 6,  7,  8,  9, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){10, 11, 12, 13, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){14, 15, 16, 17, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){18, 19, 20, 21, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing
};

static const Animation char_scary_anim[CharAnim_Max] = {
	{2, (const u8[]){22, 23, 24, 25, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){26, 27, 28, 29, 30, 31, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing
};

//Orange character functions
void Char_Orange_SetFrame(void *user, u8 frame)
{
	Char_Orange *this = (Char_Orange*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_orange_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Orange_Tick(Character *character)
{
	Char_Orange *this = (Char_Orange*)character;

	if (stage.stage_id == StageId_5_2)
	{
		//Camera stuff
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 0)
		{
			this->character.focus_x = FIXED_DEC(-100, 1);
			this->character.focus_y = FIXED_DEC(-100, 1);
			this->character.focus_zoom = FIXED_DEC(125,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 1040)
		{
			this->character.focus_x = FIXED_DEC(-40, 1);
			this->character.focus_y = FIXED_DEC(-60, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 1816)
		{
			this->character.focus_x = FIXED_DEC(-80, 1);
			this->character.focus_y = FIXED_DEC(-85, 1);
			this->character.focus_zoom = FIXED_DEC(96,100);
		}
	}
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	if (stage.stage_id == StageId_5_2)
	{
		switch(stage.song_step)
		{
			case 0:
				Animatable_Init(&this->character.animatable, char_orange_anim);
				break;
			case 784:
				Animatable_Init(&this->character.animatable, char_scary_anim);
				break;
		}
	}
	
	if (stage.stage_id == StageId_5_2 && stage.song_step >= 784)
	memcpy(this->character.health_i, char_scary_icons, sizeof(char_scary_icons));
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Orange_SetFrame);
	Character_Draw(character, &this->tex, &char_orange_frame[this->frame]);
}

void Char_Orange_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Orange_Free(Character *character)
{
	Char_Orange *this = (Char_Orange*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Orange_New(fixed_t x, fixed_t y)
{
	//Allocate orange object
	Char_Orange *this = Mem_Alloc(sizeof(Char_Orange));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Orange_New] Failed to allocate orange object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Orange_Tick;
	this->character.set_anim = Char_Orange_SetAnim;
	this->character.free = Char_Orange_Free;
	
	Animatable_Init(&this->character.animatable, char_orange_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_orange_icons, sizeof(char_orange_icons));

	//health bar color
	this->character.health_bar = 0xFFAD63D6;
	
	this->character.focus_x = FIXED_DEC(-100,1);
	this->character.focus_y = FIXED_DEC(-100,1);
	this->character.focus_zoom = FIXED_DEC(125,100);
	
	this->character.size = FIXED_DEC(50,100);
	
	//Load art
	this->arc_main = IO_Read("\\OCHAR\\ORANGE.ARC;1");
	
	const char **pathp = (const char *[]){
		"orange0.tim",
		"orange1.tim",
		"orange2.tim",
		"orange3.tim",
		"orange4.tim",
		"orange5.tim",
		"scary0.tim",
		"scary1.tim",
		"scary2.tim",
		"scary3.tim",
		"scary4.tim",
		"scary5.tim",
		"scary6.tim",
		"scary7.tim",
		"scary8.tim",
		"scary9.tim",
		"scary10.tim",
		"scary11.tim",
		"scary12.tim",

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
