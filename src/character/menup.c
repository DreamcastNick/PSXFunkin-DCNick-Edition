/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

//menu bf sprites and offsets by bilious
#include "menup.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Menu MenuP player types
enum
{
	MenuP_ArcMain_MenuP,
	
	MenuP_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[MenuP_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_MenuP;

static const u16 char_bf_icons[2][4] = {
	{0,0,36,36},
	{36,0,36,36}
};

//Menu MenuP player definitions
const CharFrame char_menup_frame[] = {
	{MenuP_ArcMain_MenuP, {  0,   0,  83,  80}, { 53,  92}}, //idle0 0
	{MenuP_ArcMain_MenuP, { 83,   0,  83,  79}, { 53,  91}}, //idle1 1
	{MenuP_ArcMain_MenuP, {166,   0,  82,  81}, { 52,  93}}, //idle2 2
	{MenuP_ArcMain_MenuP, {  0,  81,  84,  84}, { 52,  96}}, //idle3 3
	{MenuP_ArcMain_MenuP, { 84,  81,  83,  84}, { 52,  96}}, //idle4 4

	{MenuP_ArcMain_MenuP, {167,  81,  80,  84}, { 52,  96}}, //peace0 5
	{MenuP_ArcMain_MenuP, {  0, 165,  84,  84}, { 53,  95}}, //peace1 6
	{MenuP_ArcMain_MenuP, { 84, 165,  85,  84}, { 53,  95}}, //peace2 7
};

const Animation char_menup_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, ASCR_BACK, 1}}, 	   //CharAnim_Idle
	{2, (const u8[]){ 5, 6, 6, 7, 7, 7,  ASCR_BACK, 1}},   //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_LeftAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_DownAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_UpAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},         //CharAnim_RightAlt
};

//Menu MenuP player functions
void Char_MenuP_SetFrame(void *user, u8 frame)
{
	Char_MenuP *this = (Char_MenuP*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_menup_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_MenuP_Tick(Character *character)
{
	Char_MenuP *this = (Char_MenuP*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw character
	Animatable_Animate(&character->animatable, (void*)this, Char_MenuP_SetFrame);
	Character_Draw(character, &this->tex, &char_menup_frame[this->frame]);
}

void Char_MenuP_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_MenuP_Free(Character *character)
{
	Char_MenuP *this = (Char_MenuP*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_MenuP_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_MenuP *this = Mem_Alloc(sizeof(Char_MenuP));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_MenuP_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_MenuP_Tick;
	this->character.set_anim = Char_MenuP_SetAnim;
	this->character.free = Char_MenuP_Free;
	
	Animatable_Init(&this->character.animatable, char_menup_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_bf_icons, sizeof(char_bf_icons));
	
	this->character.health_bar = 0xFF29B5D6;
	
	this->character.focus_x = FIXED_DEC(-50,1);
	this->character.focus_y = (stage.stage_id == StageId_1_4) ? FIXED_DEC(-85,1) : FIXED_DEC(-65,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(100,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MENUP.ARC;1");
	
	const char **pathp = (const char *[]){
		"bf.tim",   //MenuP_ArcMain_MenuP0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
