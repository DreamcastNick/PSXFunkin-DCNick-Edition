/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "paps.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Paps character structure
enum
{
	Paps_ArcMain_PapsA0,
	Paps_ArcMain_PapsA1,
	Paps_ArcMain_PapsA2,
	Paps_ArcMain_PapsB0,
	Paps_ArcMain_PapsB1,
	Paps_ArcMain_PapsB2,
	Paps_ArcMain_PapsB3,
	Paps_ArcMain_PapsB4,
	Paps_ArcMain_PapsB5,
	Paps_ArcMain_PapsB6,
	Paps_ArcMain_PapsB7,
	Paps_ArcMain_PapsB8,
	Paps_ArcMain_PapsB9,
	Paps_ArcMain_PapsB10,
	Paps_ArcMain_PapsB11,
	Paps_ArcMain_PapsC0,
	Paps_ArcMain_PapsC1,
	Paps_ArcMain_PapsC2,
	Paps_ArcMain_PapsC3,
	Paps_ArcMain_PapsC4,
	Paps_ArcMain_PapsC5,
	Paps_ArcMain_PapsC6,
	Paps_ArcMain_PapsC7,
	Paps_ArcMain_PapsC8,
	Paps_ArcMain_PapsC9,
	Paps_ArcMain_PapsC10,
	Paps_ArcMain_PapsC11,
	
	Paps_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Paps_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Paps;

static const u16 char_paps_icons[2][4] = {
	{72,0,36,36},
	{108,0,36,36}
};

//Paps character definitions
static const CharFrame char_paps_frame[] = {
	{Paps_ArcMain_PapsA0, {  0,  0, 67,183}, {158,157}}, //0 idle 1

	{Paps_ArcMain_PapsA0, { 67,  0, 67,183}, {161,157}}, //1 left 1
	{Paps_ArcMain_PapsA0, {134,  0, 67,183}, {158,157}}, //2 left 2

	{Paps_ArcMain_PapsA1, {  0,  0, 67,183}, {159,155}}, //3 down 1
	{Paps_ArcMain_PapsA1, { 67,  0, 67,183}, {158,157}}, //4 down 2

	{Paps_ArcMain_PapsA1, {134,  0, 67,183}, {157,161}}, //5 up 1
	{Paps_ArcMain_PapsA2, {  0,  0, 67,183}, {158,157}}, //6 up 2

	{Paps_ArcMain_PapsA2, { 67,  0, 67,183}, {155,157}}, //7 right 1
	{Paps_ArcMain_PapsA2, {134,  0, 67,183}, {158,157}}, //8 right 2
	
	{Paps_ArcMain_PapsB0, {  0,  0,134,180}, {160,160}}, //9 idle 1
	{Paps_ArcMain_PapsB1, {  0,  0,134,180}, {160,160}}, //10 idle 2
	{Paps_ArcMain_PapsB2, {  0,  0,134,180}, {160,160}}, //11 idle 3
	{Paps_ArcMain_PapsB3, {  0,  0,134,180}, {160,160}}, //12 idle 4
	{Paps_ArcMain_PapsB4, {  0,  0,134,180}, {160,160}}, //13 idle 5
	{Paps_ArcMain_PapsB5, {  0,  0,134,180}, {160,160}}, //14 idle 6

	{Paps_ArcMain_PapsB6, {  0,  0,144,186}, {198,164}}, //15 left 1
	{Paps_ArcMain_PapsB7, {  0,  0,144,186}, {198,164}}, //16 left 2

	{Paps_ArcMain_PapsB8, {  0,  0,126,167}, {155,150}}, //17 down 1
	{Paps_ArcMain_PapsB8, {126,  0,126,167}, {155,150}}, //18 down 2

	{Paps_ArcMain_PapsB9, {  0,  0,104,209}, {118,182}}, //19 up 1
	{Paps_ArcMain_PapsB9, {104,  0,104,209}, {118,182}}, //20 up 2

	{Paps_ArcMain_PapsB10, {  0,  0,154,166}, {116,149}}, //21 right 1
	{Paps_ArcMain_PapsB11, {  0,  0,154,166}, {116,149}}, //22 right 2

	{Paps_ArcMain_PapsC0, {  0,  0,234,224}, {160,160}}, //23 idle 1
	{Paps_ArcMain_PapsC1, {  0,  0,234,224}, {160,160}}, //24 idle 2
	{Paps_ArcMain_PapsC2, {  0,  0,234,224}, {160,160}}, //25 idle 3
	{Paps_ArcMain_PapsC3, {  0,  0,233,224}, {160,160}}, //26 idle 4

	{Paps_ArcMain_PapsC4, {  0,  0,234,224}, {124,149}}, //27 left 1
	{Paps_ArcMain_PapsC5, {  0,  0,234,224}, {124,149}}, //28 left 2

	{Paps_ArcMain_PapsC6, {  0,  0,234,224}, {154,169}}, //29 down 1
	{Paps_ArcMain_PapsC7, {  0,  0,233,224}, {154,169}}, //30 down 2

	{Paps_ArcMain_PapsC8, {  0,  0,234,223}, {160,150}}, //31 up 1
	{Paps_ArcMain_PapsC9, {  0,  0,234,223}, {160,150}}, //32 up 2

	{Paps_ArcMain_PapsC10, {  0,  0,234,223}, {187,160}}, //33 right 1
	{Paps_ArcMain_PapsC11, {  0,  0,233,223}, {187,160}}, //34 right 2
};

static const Animation char_papsa_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, ASCR_BACK, 0}},			 //CharAnim_Idle
	{2, (const u8[]){ 1,  2, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 3,  4, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 5,  6, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_Sing
};

static const Animation char_papsb_anim[CharAnim_Max] = {
	{2, (const u8[]){ 9, 10, 11, 12, 13, 14, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){15, 16, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){17, 18, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){19, 20, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){21, 22, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_Sing
};

static const Animation char_papsc_anim[CharAnim_Max] = {
	{2, (const u8[]){23, 24, 25, 26, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){27, 28, ASCR_BACK, 0}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){29, 30, ASCR_BACK, 0}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){31, 32, ASCR_BACK, 0}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){33, 34, ASCR_BACK, 0}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_Sing
};

//Paps character functions
void Char_Paps_SetFrame(void *user, u8 frame)
{
	Char_Paps *this = (Char_Paps*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_paps_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Paps_Tick(Character *character)
{
	Char_Paps *this = (Char_Paps*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Paps_SetFrame);
	Character_Draw(character, &this->tex, &char_paps_frame[this->frame]);
}

void Char_Paps_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Paps_Free(Character *character)
{
	Char_Paps *this = (Char_Paps*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Paps_New(fixed_t x, fixed_t y)
{
	//Allocate paps object
	Char_Paps *this = Mem_Alloc(sizeof(Char_Paps));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Paps_New] Failed to allocate paps object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Paps_Tick;
	this->character.set_anim = Char_Paps_SetAnim;
	this->character.free = Char_Paps_Free;
	
	Animatable_Init(&this->character.animatable, char_papsa_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_paps_icons, sizeof(char_paps_icons));

	//health bar color
	this->character.health_bar = 0xFFFF0000;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(100,100);
	
	//Load art
	this->arc_main = IO_Read("\\OCHAR\\PAPS.ARC;1");
	
	const char **pathp = (const char *[]){
		"papsa0.tim",
		"papsa1.tim",
		"papsa2.tim",
		"papsb0.tim",
		"papsb1.tim",
		"papsb2.tim",
		"papsb3.tim",
		"papsb4.tim",
		"papsb5.tim",
		"papsb6.tim",
		"papsb7.tim",
		"papsb8.tim",
		"papsb9.tim",
		"papsb10.tim",
		"papsb11.tim",
		"papsc0.tim",
		"papsc1.tim",
		"papsc2.tim",
		"papsc3.tim",
		"papsc4.tim",
		"papsc5.tim",
		"papsc6.tim",
		"papsc7.tim",
		"papsc8.tim",
		"papsc9.tim",
		"papsc10.tim",
		"papsc11.tim",

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
