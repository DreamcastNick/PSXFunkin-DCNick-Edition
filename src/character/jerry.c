/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "jerry.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Jerry character structure
enum
{
	Jerry_ArcMain_Jerry0,
	Jerry_ArcMain_Jerry1,
	Jerry_ArcMain_Jerry2,
	Jerry_ArcMain_Jerry3,
	Jerry_ArcMain_Jerry4,
	Jerry_ArcMain_Jerry5,
	Jerry_ArcMain_Jerry6,
	Jerry_ArcMain_Jerry7,
	
	Jerry_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Jerry_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Jerry;

static const u16 char_jerry_icons[2][4] = {
	{72,0,36,36},
	{108,0,36,36}
};

//Jerry character definitions
static const CharFrame char_jerry_frame[] = {
	{Jerry_ArcMain_Jerry0,{0,0,86,100},{159,178}}, //0 idle 1
	{Jerry_ArcMain_Jerry0,{91,0,86,100},{158,178}}, //1 idle 2
	{Jerry_ArcMain_Jerry0,{0,105,86,101},{159,179}}, //2 idle 3
	{Jerry_ArcMain_Jerry0,{90,104,85,102},{158,180}}, //3 idle 4
	{Jerry_ArcMain_Jerry1,{0,0,85,102},{157,180}}, //4 idle 5
	{Jerry_ArcMain_Jerry1,{90,1,85,102},{157,179}}, //5 idle 6

	{Jerry_ArcMain_Jerry1,{0,107,95,97},{167,174}}, //6 left 1
	{Jerry_ArcMain_Jerry1,{100,107,95,97},{166,174}}, //7 left 2
	{Jerry_ArcMain_Jerry2,{0,0,94,97},{165,174}}, //8 left 3
	{Jerry_ArcMain_Jerry2,{100,0,94,97},{165,174}}, //9 left 4
	{Jerry_ArcMain_Jerry2,{0,100,94,98},{165,174}}, //10 left 5
	
	{Jerry_ArcMain_Jerry2,{100,100,93,92},{157,170}}, // 11 down 1
	{Jerry_ArcMain_Jerry3,{0,0,94,92},{158,170}}, //12 down 2
	{Jerry_ArcMain_Jerry3,{102,0,93,92},{158,170}}, //13 down 3
	{Jerry_ArcMain_Jerry3,{0,96,93,91},{158,169}}, //14 down 4
	{Jerry_ArcMain_Jerry3,{102,96,93,91},{158,169}}, // 15 down 5
	
	{Jerry_ArcMain_Jerry4,{0,0,81,124},{158,202}}, //16 up 1
	{Jerry_ArcMain_Jerry4,{87,0,82,124},{158,202}}, //17 up 2
	{Jerry_ArcMain_Jerry5,{0,0,84,122},{158,200}}, //18 up 3
	{Jerry_ArcMain_Jerry5,{88,0,85,122},{158,200}}, //19 up 4
	{Jerry_ArcMain_Jerry6,{0,0,85,125},{158,202}}, //20 up 5

	{Jerry_ArcMain_Jerry6,{89,0,105,90},{163,168}}, //21 right 1
	{Jerry_ArcMain_Jerry6,{0,127,112,90},{164,168}}, //22 right 2
	{Jerry_ArcMain_Jerry6,{125,127,99,90},{164,168}}, //23 right 3
	{Jerry_ArcMain_Jerry7,{0,0,99,89},{165,167}}, //24 right 4
	{Jerry_ArcMain_Jerry7,{124,0,99,89},{165,167}}, //25 right 5

	{Jerry_ArcMain_Jerry7,{0,93,85,103},{158,180}}, //26 talk 1
};

static const Animation char_jerry_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5,  5,  5, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 6,  7,  8,  9, 10, ASCR_BACK, 0}},         	  //CharAnim_Left
	{2, (const u8[]){26, ASCR_BACK, 0}},         	  				  //CharAnim_LeftAlt
	{2, (const u8[]){11, 12, 13, 14, 15, ASCR_BACK, 0}},         	  //CharAnim_Down
	{2, (const u8[]){26, ASCR_BACK, 0}},         	  				  //CharAnim_DownAlt
	{2, (const u8[]){16, 17, 18, 19, 20, ASCR_BACK, 0}},         	  //CharAnim_Up
	{2, (const u8[]){26, ASCR_BACK, 0}},         	  				  //CharAnim_UpAlt
	{2, (const u8[]){21, 22, 23, 24, 25, ASCR_BACK, 0}},         	  //CharAnim_Right
	{2, (const u8[]){26, ASCR_BACK, 0}},         	  				  //CharAnim_RightAlt
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnGrow
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UnShrink
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_Sing
};

//Jerry character functions
void Char_Jerry_SetFrame(void *user, u8 frame)
{
	Char_Jerry *this = (Char_Jerry*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_jerry_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Jerry_Tick(Character *character)
{
	Char_Jerry *this = (Char_Jerry*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Jerry_SetFrame);
	Character_Draw(character, &this->tex, &char_jerry_frame[this->frame]);
}

void Char_Jerry_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Jerry_Free(Character *character)
{
	Char_Jerry *this = (Char_Jerry*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Jerry_New(fixed_t x, fixed_t y)
{
	//Allocate jerry object
	Char_Jerry *this = Mem_Alloc(sizeof(Char_Jerry));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Jerry_New] Failed to allocate jerry object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Jerry_Tick;
	this->character.set_anim = Char_Jerry_SetAnim;
	this->character.free = Char_Jerry_Free;
	
	Animatable_Init(&this->character.animatable, char_jerry_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_jerry_icons, sizeof(char_jerry_icons));

	//health bar color
	this->character.health_bar = 0xFFAD63D6;
	
	this->character.focus_x = FIXED_DEC(-60,1);
	this->character.focus_y = FIXED_DEC(-150,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(165,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\JERRY.ARC;1");
	
	const char **pathp = (const char *[]){
		"jerry0.tim",
		"jerry1.tim",
		"jerry2.tim",
		"jerry3.tim",
		"jerry4.tim",
		"jerry5.tim",
		"jerry6.tim",
		"jerry7.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
