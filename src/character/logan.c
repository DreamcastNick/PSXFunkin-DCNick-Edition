/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "logan.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

#include "eyes.h"

//Logan character structure
enum
{
	Logan_ArcMain_Logan0,
	
	Logan_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main, arc_scene;
	IO_Data arc_ptr[Logan_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
	
	//Eyes
	Eyes eyes;
	
} Char_Logan;

static const u16 char_logan_icons[2][4] = {
	{144,0,36,36},
	{180,0,36,36}
};

//Logan character definitions
static const CharFrame char_logan_frame[] = {
	{Logan_ArcMain_Logan0, { 11,   8,  64,  94}, { 37,  72}}, //0 bop left 1
	{Logan_ArcMain_Logan0, { 95,   8,  64,  94}, { 37,  72}}, //1 bop left 2
	{Logan_ArcMain_Logan0, {179,   8,  65,  94}, { 36,  72}}, //2 bop left 3
	{Logan_ArcMain_Logan0, {  8, 118,  64,  94}, { 37,  72}}, //3 bop left 4
	{Logan_ArcMain_Logan0, { 92, 118,  64,  94}, { 37,  72}}, //4 bop left 5
};

static const Animation char_logan_anim[CharAnim_Max] = {
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_Idle
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_Left
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}}, 	 //CharAnim_LeftAlt
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_Down
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_DownAlt
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_Up
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_UpAlt
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}},                        //CharAnim_Right
	{6, (const u8[]){ 0,  0,  1,  1,  2,  2,  3,  3,  4,  4, ASCR_BACK, 1}}, 	 //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing
};

//Logan character functions
void Char_Logan_SetFrame(void *user, u8 frame)
{
	Char_Logan *this = (Char_Logan*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_logan_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Logan_Tick(Character *character)
{
	Char_Logan *this = (Char_Logan*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);

	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Logan_SetFrame);
	Character_Draw(character, &this->tex, &char_logan_frame[this->frame]);
	
	//Tick eyess
	Eyes_Tick(&this->eyes, character->x, character->y);
}

void Char_Logan_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Logan_Free(Character *character)
{
	Char_Logan *this = (Char_Logan*)character;
	
	//Free art
	Mem_Free(this->arc_main);
	Mem_Free(this->arc_scene);
}

Character *Char_Logan_New(fixed_t x, fixed_t y)
{
	//Allocate logan object
	Char_Logan *this = Mem_Alloc(sizeof(Char_Logan));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Logan_New] Failed to allocate logan object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Logan_Tick;
	this->character.set_anim = Char_Logan_SetAnim;
	this->character.free = Char_Logan_Free;
	
	Animatable_Init(&this->character.animatable, char_logan_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_logan_icons, sizeof(char_logan_icons));

	//health bar color
	this->character.health_bar = 0xFFA5004A;
	
	this->character.focus_x = FIXED_DEC(2,1);
	this->character.focus_y = FIXED_DEC(-40,1);
	this->character.focus_zoom = FIXED_DEC(200,100);
	
	this->character.size = FIXED_DEC(100,100);
	
		//Load art
		this->arc_main = IO_Read("\\CHAR\\LOGAN.ARC;1");
		
		const char **pathp = (const char *[]){
			"logan0.tim", //Logan_ArcMain_Logan0
			NULL
		};
		IO_Data *arc_ptr = this->arc_ptr;
		for (; *pathp != NULL; pathp++)
			*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	//Initialize eyes
	Eyes_Init(&this->eyes);
	
	return (Character*)this;
}
