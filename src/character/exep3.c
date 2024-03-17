/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "exep3.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//ExeP3 character structure
enum
{
	ExeP3_ArcMain_Eggy0,
	ExeP3_ArcMain_Eggy1,
	ExeP3_ArcMain_Eggy2,
	ExeP3_ArcMain_Eggy3,
	ExeP3_ArcMain_Eggy4,
	ExeP3_ArcMain_Eggy5,
	ExeP3_ArcMain_Eggy6,
	ExeP3_ArcMain_Eggy7,
	ExeP3_ArcMain_Eggy8,
	ExeP3_ArcMain_Eggy9,
	ExeP3_ArcMain_Eggy10,
	ExeP3_ArcMain_Eggy11,
	ExeP3_ArcMain_Eggy12,
	ExeP3_ArcMain_Eggy13,
	ExeP3_ArcMain_Eggy14,
	ExeP3_ArcMain_Eggy15,
	ExeP3_ArcMain_Eggy16,
	ExeP3_ArcMain_Eggy17,
	ExeP3_ArcMain_Eggy18,
	ExeP3_ArcMain_Eggy19,
	ExeP3_ArcMain_Eggy20,
	ExeP3_ArcMain_Eggy21,
	ExeP3_ArcMain_Eggy22,
	ExeP3_ArcMain_Eggy23,
	ExeP3_ArcMain_Eggy24,
	ExeP3_ArcMain_Eggy25,
	ExeP3_ArcMain_Eggy26,
	ExeP3_ArcMain_Eggy27,
	ExeP3_ArcMain_Eggy28,
	ExeP3_ArcMain_Knux0,
	ExeP3_ArcMain_Knux1,
	ExeP3_ArcMain_Knux2,
	ExeP3_ArcMain_Knux3,
	ExeP3_ArcMain_Knux4,
	ExeP3_ArcMain_Knux5,
	ExeP3_ArcMain_Tails0,
	ExeP3_ArcMain_Tails1,
	ExeP3_ArcMain_Tails2,
	ExeP3_ArcMain_Tails3,
	ExeP3_ArcMain_Tails4,
	ExeP3_ArcMain_Tails5,
	ExeP3_ArcMain_Xeno0,
	ExeP3_ArcMain_Xeno1,
	ExeP3_ArcMain_Xeno2,
	ExeP3_ArcMain_Xeno3,
	ExeP3_ArcMain_Xeno4,
	ExeP3_ArcMain_Xeno5,
	ExeP3_ArcMain_Xeno6,
	ExeP3_ArcMain_Xeno7,
	ExeP3_ArcMain_Xeno8,
	ExeP3_ArcMain_Xeno9,
	ExeP3_ArcMain_Xeno10,
	ExeP3_ArcMain_Xeno11,
	ExeP3_ArcMain_Xeno12,
	ExeP3_ArcMain_Xeno13,
	ExeP3_ArcMain_Xeno14,
	ExeP3_ArcMain_Xeno15,
	
	ExeP3_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[ExeP3_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_ExeP3;

static const u16 char_tails_icons[2][4] = {
	{0,144,36,36},
	{36,144,36,36}
};

static const u16 char_knux_icons[2][4] = {
	{72,144,36,36},
	{108,144,36,36}
};

static const u16 char_eggman_icons[2][4] = {
	{144,144,36,36},
	{180,144,36,36}
};

static const u16 char_xeno_icons[2][4] = {
	{0,180,36,36},
	{36,180,36,36}
};

//ExeP3 character definitions
static const CharFrame char_exep3_frame[] = {
	{ExeP3_ArcMain_Eggy4, {  0,   0, 128, 146}, { 72, 143}}, //0 idle 1
	{ExeP3_ArcMain_Eggy5, {  0,   0, 128, 146}, { 72, 143}}, //1 idle 2
	{ExeP3_ArcMain_Eggy6, {  0,   0, 128, 146}, { 72, 143}}, //2 idle 3
	{ExeP3_ArcMain_Eggy7, {  0,   0, 128, 146}, { 72, 143}}, //3 idle 4	
	{ExeP3_ArcMain_Eggy8, {  0,   0, 128, 146}, { 72, 143}}, //4 idle 5
	{ExeP3_ArcMain_Eggy9, {  0,   0, 128, 146}, { 72, 143}}, //5 idle 6
	
	{ExeP3_ArcMain_Eggy17, {  0,   0, 172, 170}, { 133, 167}}, //6 left 1
	{ExeP3_ArcMain_Eggy18, {  0,   0, 172, 170}, { 133, 167}}, //7 left 2
	{ExeP3_ArcMain_Eggy19, {  0,   0, 172, 170}, { 133, 167}}, //8 left 3
	{ExeP3_ArcMain_Eggy20, {  0,   0, 172, 170}, { 133, 167}}, //9 left 4
	
	{ExeP3_ArcMain_Eggy0, {  0,   0, 156, 121}, { 81, 119}}, //10 down 1
	{ExeP3_ArcMain_Eggy1, {  0,   0, 156, 121}, { 81, 119}}, //11 down 2
	{ExeP3_ArcMain_Eggy2, {  0,   0, 156, 121}, { 81, 119}}, //12 down 3
	{ExeP3_ArcMain_Eggy3, {  0,   0, 156, 121}, { 81, 119}}, //13 down 4
	
	{ExeP3_ArcMain_Eggy25, {   0,   0, 196, 205}, { 107, 201}}, //14 up 1
	{ExeP3_ArcMain_Eggy26, {   0,   0, 196, 205}, { 107, 201}}, //15 up 2
	{ExeP3_ArcMain_Eggy27, {   0,   0, 196, 205}, { 107, 201}}, //16 up 3
	{ExeP3_ArcMain_Eggy28, {   0,   0, 196, 205}, { 107, 201}}, //17 up 4
	
	{ExeP3_ArcMain_Eggy21, {  0,   0, 180, 191}, { 107, 188}}, //18 right 1
	{ExeP3_ArcMain_Eggy22, {  0,   0, 180, 191}, { 107, 188}}, //19 right 2
	{ExeP3_ArcMain_Eggy23, {  0,   0, 180, 191}, { 107, 188}}, //20 right 3
	{ExeP3_ArcMain_Eggy24, {  0,   0, 180, 191}, { 107, 188}}, //21 right 4
	
	{ExeP3_ArcMain_Eggy10, {  0,   0, 136, 198}, { 75, 194}}, //22 laugh 1
	{ExeP3_ArcMain_Eggy11, {  0,   0, 136, 198}, { 75, 194}}, //23 laugh 2
	{ExeP3_ArcMain_Eggy12, {  0,   0, 136, 198}, { 75, 194}}, //24 laugh 3
	{ExeP3_ArcMain_Eggy13, {  0,   0, 136, 198}, { 75, 194}}, //25 laugh 4	
	{ExeP3_ArcMain_Eggy14, {  0,   0, 136, 198}, { 75, 194}}, //26 laugh 5
	{ExeP3_ArcMain_Eggy15, {  0,   0, 136, 198}, { 75, 194}}, //27 laugh 6
	{ExeP3_ArcMain_Eggy16, {  0,   0, 136, 198}, { 75, 194}}, //28 laugh 7
	
	{ExeP3_ArcMain_Knux1, {  4,   4,  74, 118}, { 62, 113}},  //29 idle 1
	{ExeP3_ArcMain_Knux1, { 86,   4,  74, 118}, { 62, 113}},  //30 idle 2
	{ExeP3_ArcMain_Knux1, {  4, 130,  74, 118}, { 62, 113}},  //31 idle 3
	{ExeP3_ArcMain_Knux1, { 86, 130,  74, 118}, { 62, 113}},  //32 idle 4
	{ExeP3_ArcMain_Knux2, {  4,   4,  74, 118}, { 62, 113}},  //33 idle 5
	{ExeP3_ArcMain_Knux2, { 86,   4,  74, 118}, { 62, 113}},  //34 idle 6
	{ExeP3_ArcMain_Knux2, {  4, 130,  74, 118}, { 62, 113}},  //35 idle 7
	{ExeP3_ArcMain_Knux2, { 86, 130,  74, 118}, { 62, 113}},  //36 idle 8
	
	{ExeP3_ArcMain_Knux3, {  5,   4, 101, 103}, { 96, 98}}, //37 left 1
	{ExeP3_ArcMain_Knux3, {114,   4, 101, 103}, { 96, 98}}, //38 left 2
	{ExeP3_ArcMain_Knux3, {  5, 115, 101, 103}, { 96, 98}}, //39 left 3
	{ExeP3_ArcMain_Knux3, {114, 115, 101, 103}, { 96, 98}}, //40 left 4
	
	{ExeP3_ArcMain_Knux0, {  4,   4, 100,  94}, { 78, 89}}, //41 down 1
	{ExeP3_ArcMain_Knux0, {112,   4, 100,  94}, { 78, 89}}, //42 down 2
	{ExeP3_ArcMain_Knux0, {  4, 106, 100,  94}, { 78, 89}}, //43 down 3
	{ExeP3_ArcMain_Knux0, {112, 106, 100,  94}, { 78, 89}}, //44 down 4
	
	{ExeP3_ArcMain_Knux5, {  7,   4,  87, 131}, { 70, 126}}, //45 up 1
	{ExeP3_ArcMain_Knux5, {102,   4,  87, 131}, { 70, 126}}, //46 up 2
	
	{ExeP3_ArcMain_Knux4, {  5,   4, 101, 102}, { 52, 96}}, //47 right 1
	{ExeP3_ArcMain_Knux4, {114,   4, 101, 102}, { 52, 96}}, //48 right 2
	{ExeP3_ArcMain_Knux4, {  5, 114, 101, 102}, { 52, 96}}, //49 right 3
	{ExeP3_ArcMain_Knux4, {114, 114, 101, 102}, { 52, 96}}, //50 right 4
	
	{ExeP3_ArcMain_Tails1, {  0,   0, 64, 103}, { 53, 103}},  //51 idle 1
	{ExeP3_ArcMain_Tails1, { 64,   0, 64, 103}, { 52, 103}},  //52 idle 2
	{ExeP3_ArcMain_Tails1, {128,   0, 63, 103}, { 52, 103}},  //53 idle 3
	{ExeP3_ArcMain_Tails1, {191,   0, 65, 103}, { 54, 103}},  //54 idle 4
	{ExeP3_ArcMain_Tails1, {  0, 103, 65, 103}, { 54, 103}},  //55 idle 5
	{ExeP3_ArcMain_Tails1, { 65, 103, 65, 103}, { 54, 103}},  //56 idle 6
	{ExeP3_ArcMain_Tails1, {130, 103, 65, 103}, { 54, 103}},  //57 idle 7
	{ExeP3_ArcMain_Tails2, {  0,   0, 65, 103}, { 54, 103}},  //58 idle 8
	{ExeP3_ArcMain_Tails2, { 65,   0, 65, 103}, { 54, 103}},  //59 idle 9
	{ExeP3_ArcMain_Tails2, {130,   0, 65, 103}, { 53, 103}},  //60 idle 10
	
	{ExeP3_ArcMain_Tails3, {  0,   0, 87, 99}, { 87, 99}},    //61 left 1
	{ExeP3_ArcMain_Tails3, { 87,   0, 85, 100}, { 85, 99}},   //62 left 2
	{ExeP3_ArcMain_Tails3, {172,   0, 84, 100}, { 84, 100}},  //63 left 3
	{ExeP3_ArcMain_Tails3, {  0,  99, 86, 100}, { 86, 100}},  //64 left 4
	{ExeP3_ArcMain_Tails3, { 86, 100, 85, 100}, { 85, 100}},  //65 left 5
	{ExeP3_ArcMain_Tails3, {171, 100, 85, 100}, { 85, 100}},  //66 left 6

	{ExeP3_ArcMain_Tails0, {  0,   0, 74, 88}, { 74, 88}},  //67 down 1
	{ExeP3_ArcMain_Tails0, { 74,   0, 73, 89}, { 73, 89}},  //68 down 2
	{ExeP3_ArcMain_Tails0, {147,   0, 74, 90}, { 74, 90}},  //69 down 3
	{ExeP3_ArcMain_Tails0, {  0,  88, 74, 90}, { 74, 90}},  //70 down 4
	{ExeP3_ArcMain_Tails0, { 74,  89, 74, 90}, { 73, 90}},  //71 down 5
	{ExeP3_ArcMain_Tails0, {148,  90, 74, 90}, { 73, 90}},  //72 down 6
	
	{ExeP3_ArcMain_Tails5, {  0,   0, 76, 113}, { 57, 113}},  //73 up 1
	{ExeP3_ArcMain_Tails5, { 76,   0, 76, 112}, { 57, 112}},  //74 up 2
	{ExeP3_ArcMain_Tails5, {152,   0, 77, 111}, { 58, 111}},  //75 up 3
	{ExeP3_ArcMain_Tails5, {  0, 113, 78, 111}, { 59, 111}},  //76 up 4
	{ExeP3_ArcMain_Tails5, { 78, 112, 78, 110}, { 59, 110}},  //77 up 5
	{ExeP3_ArcMain_Tails5, {156, 111, 78, 110}, { 59, 110}},  //78 up 6
	
	{ExeP3_ArcMain_Tails4, {  0,   0, 83, 99}, { 70, 99}},  //79 right 1
	{ExeP3_ArcMain_Tails4, { 83,   0, 80, 99}, { 70, 99}},  //80 right 2
	{ExeP3_ArcMain_Tails4, {163,   0, 80, 97}, { 70, 97}},  //81 right 3
	{ExeP3_ArcMain_Tails4, {  0,  99, 81, 97}, { 70, 97}},  //82 right 4
	{ExeP3_ArcMain_Tails4, { 81,  99, 81, 97}, { 70, 97}},  //83 right 5
	{ExeP3_ArcMain_Tails4, {162,  97, 81, 97}, { 70, 97}},  //84 right 6
	
	{ExeP3_ArcMain_Xeno2, {  0,   0, 156, 235}, { 92, 233}}, //85 idle 1
	{ExeP3_ArcMain_Xeno3, {  0,   0, 156, 235}, { 92, 233}}, //86 idle 2
	{ExeP3_ArcMain_Xeno4, {  0,   0, 156, 235}, { 92, 233}}, //87 idle 3
	{ExeP3_ArcMain_Xeno5, {  0,   0, 156, 235}, { 92, 233}}, //88 idle 4	
	{ExeP3_ArcMain_Xeno6, {  0,   0, 156, 235}, { 92, 233}}, //89 idle 5
	{ExeP3_ArcMain_Xeno7, {  0,   0, 156, 235}, { 92, 233}}, //90 idle 6
	
	{ExeP3_ArcMain_Xeno10, {  0,   0, 236, 213}, { 133, 210}}, //91 left 1
	{ExeP3_ArcMain_Xeno11, {  0,   0, 236, 213}, { 133, 210}}, //92 left 2

	{ExeP3_ArcMain_Xeno0, {  0,   0, 156, 214}, { 91, 210}}, //93 down 1
	{ExeP3_ArcMain_Xeno1, {  0,   0, 156, 214}, { 91, 210}}, //94 down 2
	
	{ExeP3_ArcMain_Xeno14, {   0,   0, 156, 255}, { 89, 252}}, //95 up 1
	{ExeP3_ArcMain_Xeno15, {   0,   0, 156, 255}, { 89, 252}}, //96 up 2
	
	{ExeP3_ArcMain_Xeno12, {  0,   0, 196, 219}, { 36, 218}}, //97 right 1
	{ExeP3_ArcMain_Xeno13, {  0,   0, 196, 219}, { 36, 218}}, //98 right 2
	
	{ExeP3_ArcMain_Xeno8, {  0,   0, 148, 181}, { 59, 178}}, //99 laugh 1
	{ExeP3_ArcMain_Xeno9, {  0,   0, 148, 181}, { 59, 178}}, //100 laugh 2
};

static const Animation char_eggman_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5, ASCR_BACK, 0}},     		   //CharAnim_Idle
	{2, (const u8[]){ 6,  7,  8,  9, ASCR_BACK, 0}},       		   		   //CharAnim_Left
	{2, (const u8[]){25, 26, 27, 28, ASCR_BACK, 0}},                       //CharAnim_LeftAlt
	{2, (const u8[]){10, 11, 12, 13, ASCR_BACK, 0}},  		     		   //CharAnim_Down
	{2, (const u8[]){22, 23, 24, 25, ASCR_BACK, 0}},                       //CharAnim_DownAlt
	{2, (const u8[]){14, 15, 16, 17, ASCR_BACK, 0}},					   //CharAnim_Up
	{2, (const u8[]){25, 26, 27, 28, ASCR_BACK, 0}},                       //CharAnim_UpAlt
	{2, (const u8[]){18, 19, 20, 21, ASCR_BACK, 0}}, 					   //CharAnim_Right
	{2, (const u8[]){22, 23, 24, 25, ASCR_BACK, 0}},                       //CharAnim_RightAlt
};

static const Animation char_knux_anim[CharAnim_Max] = {
	{2, (const u8[]){ 29, 30, 31, 32, 33, 34, 35, 36, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 47, 48, 49, 50, ASCR_BACK, 0}},         		  //CharAnim_Left
	{2, (const u8[]){ 47, 48, 49, 50, ASCR_BACK, 0}},         		  //CharAnim_LeftAlt
	{2, (const u8[]){ 41, 42, 43, 44, ASCR_BACK, 0}},         		  //CharAnim_Down
	{2, (const u8[]){ 41, 42, 43, 44, ASCR_BACK, 0}},         		  //CharAnim_DownAlt
	{2, (const u8[]){ 45, 46, ASCR_BACK, 0}},         				  //CharAnim_Up
	{2, (const u8[]){ 45, 46, ASCR_BACK, 0}},         				  //CharAnim_UpAlt
	{2, (const u8[]){ 37, 38, 39, 40, ASCR_BACK, 0}},         		  //CharAnim_Right
	{2, (const u8[]){ 37, 38, 39, 40, ASCR_BACK, 0}},         		  //CharAnim_RightAlt
};

static const Animation char_tails_anim[CharAnim_Max] = {
	{2, (const u8[]){ 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, ASCR_BACK, 0}}, //CharAnim_Idle
	{2, (const u8[]){ 61, 62, 63, 64, 65, 66, ASCR_BACK, 0}},         		  //CharAnim_Left
	{2, (const u8[]){ 61, 62, 63, 64, 65, 66, ASCR_BACK, 0}},         		  //CharAnim_LeftAlt
	{2, (const u8[]){ 67, 68, 69, 70, 71, 72, ASCR_BACK, 0}},         		  //CharAnim_Down
	{2, (const u8[]){ 67, 68, 69, 70, 71, 72, ASCR_BACK, 0}},         		  //CharAnim_DownAlt
	{2, (const u8[]){ 73, 74, 75, 76, 77, 78, ASCR_BACK, 0}},         		  //CharAnim_Up
	{2, (const u8[]){ 73, 74, 75, 76, 77, 78, ASCR_BACK, 0}},         		  //CharAnim_UpAlt
	{2, (const u8[]){ 79, 80, 81, 82, 83, 84, ASCR_BACK, 0}},         		  //CharAnim_Right
	{2, (const u8[]){ 79, 80, 81, 82, 83, 84, ASCR_BACK, 0}},         		  //CharAnim_RightAlt
};

static const Animation char_xeno_anim[CharAnim_Max] = {
	{2, (const u8[]){ 85, 86, 87, 88, 89, 90, ASCR_BACK, 0}},     		   //CharAnim_Idle
	{2, (const u8[]){ 91, 92, ASCR_BACK, 0}},       		   		   	   //CharAnim_Left
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_LeftAlt
	{2, (const u8[]){ 93, 94, ASCR_BACK, 0}},  		     		   		   //CharAnim_Down
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_DownAlt
	{2, (const u8[]){ 95, 96, ASCR_BACK, 0}},					   		   //CharAnim_Up
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_UpAlt
	{2, (const u8[]){ 97, 98, ASCR_BACK, 0}}, 					   		   //CharAnim_Right
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_RightAlt
};

static const Animation char_xenof_anim[CharAnim_Max] = {
	{2, (const u8[]){ 85, 86, 87, 88, 89, 90, ASCR_BACK, 0}},     		   //CharAnim_Idle
	{2, (const u8[]){ 97, 98, ASCR_BACK, 0}},       		   		   	   //CharAnim_Left
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_LeftAlt
	{2, (const u8[]){ 93, 94, ASCR_BACK, 0}},  		     		   		   //CharAnim_Down
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_DownAlt
	{2, (const u8[]){ 95, 96, ASCR_BACK, 0}},					   		   //CharAnim_Up
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_UpAlt
	{2, (const u8[]){ 91, 92, ASCR_BACK, 0}}, 					   		   //CharAnim_Right
	{2, (const u8[]){ 99, 100, ASCR_BACK, 0}},                             //CharAnim_RightAlt
};

//ExeP3 character functions
void Char_ExeP3_SetFrame(void *user, u8 frame)
{
	Char_ExeP3 *this = (Char_ExeP3*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_exep3_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_ExeP3_Tick(Character *character)
{
	Char_ExeP3 *this = (Char_ExeP3*)character;

	if (stage.stage_id == StageId_Mod1_6)
	{
		//Camera stuff
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 0)
		{
			this->character.focus_x = FIXED_DEC(65, 1);
			this->character.focus_y = FIXED_DEC(-85, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 1040)
		{
			this->character.focus_x = FIXED_DEC(65, 1);
			this->character.focus_y = FIXED_DEC(-115, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 1296)
		{
			this->character.focus_x = FIXED_DEC(-65, 1);
			this->character.focus_y = FIXED_DEC(-85, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 2320)
		{
			this->character.focus_x = FIXED_DEC(-65, 1);
			this->character.focus_y = FIXED_DEC(-115, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 2832)
		{
			this->character.focus_x = FIXED_DEC(65, 1);
			this->character.focus_y = FIXED_DEC(-115, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}
		if ((stage.flag & STAGE_FLAG_JUST_STEP) && stage.song_step >= 4111)
		{
			this->character.focus_x = FIXED_DEC(65, 1);
			this->character.focus_y = FIXED_DEC(-115, 1);
			this->character.focus_zoom = FIXED_DEC(100,100);
		}		
	}
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	if (stage.stage_id == StageId_Mod1_6)
	{
		if (stage.song_step == 1040)
		{
			this->character.health_bar = 0xFF290675;
		}
		if (stage.song_step == 1296)
		{
			this->character.health_bar = 0xFF660000;
		}
		if (stage.song_step == 2320)
		{
			this->character.health_bar = 0xFF290675;
		}
		if (stage.song_step == 2832)
		{
			this->character.health_bar = 0xFF986500;
		}
		if (stage.song_step == 4111)
		{
			this->character.health_bar = 0xFF290675;
		}
	}

	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040)
	memcpy(this->character.health_i, char_xeno_icons, sizeof(char_xeno_icons));

	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1296)
	memcpy(this->character.health_i, char_knux_icons, sizeof(char_knux_icons));

	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320)
	memcpy(this->character.health_i, char_xeno_icons, sizeof(char_xeno_icons));

	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2832)
	memcpy(this->character.health_i, char_eggman_icons, sizeof(char_eggman_icons));

	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111)
	memcpy(this->character.health_i, char_xeno_icons, sizeof(char_xeno_icons));

	if (stage.stage_id == StageId_Mod1_6)
	{
		switch(stage.song_step)
		{
			case 0:
				Animatable_Init(&this->character.animatable, char_tails_anim);
				break;
			case 1040:
				Animatable_Init(&this->character.animatable, char_xeno_anim);
				break;
			case 1296:
				Animatable_Init(&this->character.animatable, char_knux_anim);
				break;
			case 2320:
				Animatable_Init(&this->character.animatable, char_xenof_anim);
				break;
			case 2832:
				Animatable_Init(&this->character.animatable, char_eggman_anim);
				break;
			case 4111:
				Animatable_Init(&this->character.animatable, char_xeno_anim);
				break;
		}
	}

	if (stage.stage_id == StageId_Mod1_6)
	{
		switch (stage.song_step)
		{
			case 0:
			{
				this->character.x = FIXED_DEC(-120,1);
				this->character.y = FIXED_DEC(100,1);
				break;
			}
			case 1296:
			{
				this->character.x = FIXED_DEC(60,1);
				this->character.y = FIXED_DEC(100,1);
				break;
			}
			case 2832:
			{
				this->character.x = FIXED_DEC(-120,1);
				this->character.y = FIXED_DEC(100,1);
				break;
			}
		}
	}
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_ExeP3_SetFrame);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= -100 && stage.song_step <= 1296)
	Character_Draw(character, &this->tex, &char_exep3_frame[this->frame]);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1296 && stage.song_step <= 2832)
	Character_DrawFlipped(character, &this->tex, &char_exep3_frame[this->frame]);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2832 && stage.song_step <= 5250)
	Character_Draw(character, &this->tex, &char_exep3_frame[this->frame]);
}

void Char_ExeP3_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_ExeP3_Free(Character *character)
{
	Char_ExeP3 *this = (Char_ExeP3*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_ExeP3_New(fixed_t x, fixed_t y)
{
	//Allocate exep3 object
	Char_ExeP3 *this = Mem_Alloc(sizeof(Char_ExeP3));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_ExeP3_New] Failed to allocate exep3 object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_ExeP3_Tick;
	this->character.set_anim = Char_ExeP3_SetAnim;
	this->character.free = Char_ExeP3_Free;
	
	if (stage.stage_id == StageId_Mod1_6)
	{
		switch(stage.song_step)
		{
			case 0:
				Animatable_Init(&this->character.animatable, char_tails_anim);
				break;
			case 1040:
				Animatable_Init(&this->character.animatable, char_xeno_anim);
				break;
			case 1296:
				Animatable_Init(&this->character.animatable, char_knux_anim);
				break;
			case 2320:
				Animatable_Init(&this->character.animatable, char_xeno_anim);
				break;
			case 2832:
				Animatable_Init(&this->character.animatable, char_eggman_anim);
				break;
			case 4111:
				Animatable_Init(&this->character.animatable, char_xeno_anim);
				break;
		}
	}
	
	Animatable_Init(&this->character.animatable, char_tails_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_tails_icons, sizeof(char_tails_icons));

	//health bar color
	this->character.health_bar = 0xFF666666;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-85,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(100,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\EXEP3.ARC;1");
	
	const char **pathp = (const char *[]){
		"eggy0.tim",  //Eggman_ArcMain_Idle0
		"eggy1.tim",  //Eggman_ArcMain_Idle1
		"eggy2.tim",  //Eggman_ArcMain_Idle2
		"eggy3.tim",  //Eggman_ArcMain_Idle3
		"eggy4.tim",  //Eggman_ArcMain_Idle4
		"eggy5.tim",  //Eggman_ArcMain_Idle5
		"eggy6.tim",  //Eggman_ArcMain_Left0
		"eggy7.tim",  //Eggman_ArcMain_Left1
		"eggy8.tim",  //Eggman_ArcMain_Left2
		"eggy9.tim",  //Eggman_ArcMain_Left3
		"eggy10.tim", //Eggman_ArcMain_Down0
		"eggy11.tim", //Eggman_ArcMain_Down1
		"eggy12.tim", //Eggman_ArcMain_Down2
		"eggy13.tim", //Eggman_ArcMain_Down3
		"eggy14.tim", //Eggman_ArcMain_Up0
		"eggy15.tim", //Eggman_ArcMain_Up1
		"eggy16.tim", //Eggman_ArcMain_Up2
		"eggy17.tim", //Eggman_ArcMain_Up3
		"eggy18.tim", //Eggman_ArcMain_Right0
		"eggy19.tim", //Eggman_ArcMain_Right1
		"eggy20.tim", //Eggman_ArcMain_Right2
		"eggy21.tim", //Eggman_ArcMain_Right3
		"eggy22.tim", //Eggman_ArcMain_Laugh0
		"eggy23.tim", //Eggman_ArcMain_Laugh1
		"eggy24.tim", //Eggman_ArcMain_Laugh2
		"eggy25.tim", //Eggman_ArcMain_Laugh3
		"eggy26.tim", //Eggman_ArcMain_Laugh4
		"eggy27.tim", //Eggman_ArcMain_Laugh5
		"eggy28.tim", //Eggman_ArcMain_Laugh6
		"knux0.tim",     //Knux_ArcMain_Knux0
		"knux1.tim",     //Knux_ArcMain_Knux1
		"knux2.tim",     //Knux_ArcMain_Knux2
		"knux3.tim",   	 //Knux_ArcMain_Knux3
		"knux4.tim",     //Knux_ArcMain_Knux4
		"knux5.tim",     //Knux_ArcMain_Knux5
		"tails0.tim", //Tails_ArcMain_Idle0
		"tails1.tim", //Tails_ArcMain_Idle1
		"tails2.tim", //Tails_ArcMain_Left
		"tails3.tim", //Tails_ArcMain_Down
		"tails4.tim", //Tails_ArcMain_Up
		"tails5.tim", //Tails_ArcMain_Right
		"xeno0.tim",  //Xeno_ArcMain_Idle0
		"xeno1.tim",  //Xeno_ArcMain_Idle1
		"xeno2.tim",  //Xeno_ArcMain_Idle2
		"xeno3.tim",  //Xeno_ArcMain_Idle3
		"xeno4.tim",  //Xeno_ArcMain_Idle4
		"xeno5.tim",  //Xeno_ArcMain_Idle5
		"xeno6.tim",  //Xeno_ArcMain_Left0
		"xeno7.tim",  //Xeno_ArcMain_Left1
		"xeno8.tim",  //Xeno_ArcMain_Down0
		"xeno9.tim",  //Xeno_ArcMain_Down1
		"xeno10.tim", //Xeno_ArcMain_Up0
		"xeno11.tim", //Xeno_ArcMain_Up1
		"xeno12.tim", //Xeno_ArcMain_Right0
		"xeno13.tim", //Xeno_ArcMain_Right1
		"xeno14.tim", //Xeno_ArcMain_Laugh0
		"xeno15.tim", //Xeno_ArcMain_Laugh1
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
