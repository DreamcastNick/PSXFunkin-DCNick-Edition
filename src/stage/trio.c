/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "trio.h"

#include "../mem.h"
#include "../archive.h"

int swapfard;
int opacity;

//Trio background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Normal Background
	Gfx_Tex tex_back1; //Normal Forground Trees
	Gfx_Tex tex_back2; //Normal Backdrop
	Gfx_Tex tex_back3; //Xenophanes Backdrop
	Gfx_Tex tex_back4; //Xenophanes Forground Trees
	Gfx_Tex tex_back5; //Start Static
	Gfx_Tex tex_back6; //Xenophanes Background
	Gfx_Tex tex_back7; //Normal Background Flip
	Gfx_Tex tex_back8; //Normal Forground Trees Flip
	Gfx_Tex tex_back9; //Xenophanes Forground Trees Flip
	Gfx_Tex tex_back10; //Xenophanes Background Flip
} Back_Trio;

void Back_Trio_DrawHUD(StageBack* back)
{
	Back_Trio* this = (Back_Trio*)back;

	fixed_t fx, fy;

	swapfard++;
	opacity++;

	if (swapfard == 44)
	{
		swapfard = 0;
	}

	if (opacity > 255)
	{
		opacity = 255;
	}


	if (StageId_Mod1_6)
	{	
		if (stage.song_step == 0)
			opacity = 0;
		if (stage.song_step == 1024)
			opacity = 0;
		if (stage.song_step == 1088)
			opacity = 0;
		if (stage.song_step == 1216)
			opacity = 0;
		if (stage.song_step == 1280)
			opacity = 0;
		if (stage.song_step == 2304)
			opacity = 0;
		if (stage.song_step == 2816)
			opacity = 0;
		if (stage.song_step == 4096)
			opacity = 0;
	}

	fx = stage.camera.x;
	fy = stage.camera.y;

	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back0_src = { 0, 0, 85, 85 };
	RECT_FIXED back0_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back1_src = { 85, 0, 85, 85 };
	RECT_FIXED back1_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back2_src = { 170, 0, 85, 85 };
	RECT_FIXED back2_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back3_src = { 0, 85, 85, 85 };
	RECT_FIXED back3_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
		//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back4_src = { 85, 85, 85, 85 };
	RECT_FIXED back4_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back5_src = { 170, 85, 85, 85 };
	RECT_FIXED back5_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back6_src = { 0, 170, 85, 85 };
	RECT_FIXED back6_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back7_src = { 85, 170, 85, 85 };
	RECT_FIXED back7_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	//Draw back
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back8_src = { 170, 170, 85, 85 };
	RECT_FIXED back8_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};

	switch (swapfard)
	{
		case 0:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 1:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 2:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 3:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 4:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back1_src, &back1_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 5:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 6:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 7:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 8:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 9:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back2_src, &back2_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 10:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 11:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 12:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 13:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 14:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back3_src, &back3_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 15:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 16:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 17:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 18:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 19:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back4_src, &back4_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 20:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 21:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 22:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 23:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 24:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back5_src, &back5_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 25:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 26:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 27:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 28:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 29:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back6_src, &back6_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 30:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 31:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 32:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 33:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 34:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back7_src, &back7_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 35:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 36:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 37:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 38:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 39:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back8_src, &back8_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 40:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 41:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 42:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 43:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		case 44:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
		default:
			if (stage.stage_id == StageId_Mod1_6)
			{
				if (stage.song_step >= 0 && stage.song_step <= 16)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1024 && stage.song_step <= 1040)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1088 && stage.song_step <= 1104)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1216 && stage.song_step <= 1232)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 1280 && stage.song_step <= 1296)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2304 && stage.song_step <= 2320)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 2816 && stage.song_step <= 2832)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
				if (stage.song_step >= 4096 && stage.song_step <= 4112)
				{
					Stage_BlendTexV2(&this->tex_back5, &back0_src, &back0_dst, stage.camera.bzoom, 0, opacity);
				}
			}
		break;
	}
}

void Back_Trio_DrawFG(StageBack *back)
{
	Back_Trio *this = (Back_Trio*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;

	//Draw normal foreground trees
	RECT normtrees_src = {0, 0, 256, 256};
	RECT_FIXED normtrees_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= -100 && stage.song_step <= 1040)
		Stage_DrawTex(&this->tex_back1, &normtrees_src, &normtrees_dst, stage.camera.bzoom, stage.camera.angle);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2832 && stage.song_step <= 4111)
		Stage_DrawTex(&this->tex_back1, &normtrees_src, &normtrees_dst, stage.camera.bzoom, stage.camera.angle);
	
	RECT normtreesflip_src = {0, 0, 256, 256};
	RECT_FIXED normtreesflip_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
		Stage_DrawTex(&this->tex_back8, &normtreesflip_src, &normtreesflip_dst, stage.camera.bzoom, stage.camera.angle);
	
	//Draw xenophanes foreground trees
	RECT xenotrees_src = {0, 0, 256, 256};
	RECT_FIXED xenotrees_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
		Stage_DrawTex(&this->tex_back4, &xenotrees_src, &xenotrees_dst, stage.camera.bzoom, stage.camera.angle);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
		Stage_DrawTex(&this->tex_back4, &xenotrees_src, &xenotrees_dst, stage.camera.bzoom, stage.camera.angle);
	
	RECT xenotreesflip_src = {0, 0, 256, 256};
	RECT_FIXED xenotreesflip_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
		Stage_DrawTex(&this->tex_back9, &xenotreesflip_src, &xenotreesflip_dst, stage.camera.bzoom, stage.camera.angle);
}

//Trio background functions
void Back_Trio_DrawBG(StageBack *back)
{
	Back_Trio *this = (Back_Trio*)back;
	
	fixed_t fx, fy;
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	//Draw background
	RECT normbg_src = {0, 0, 256, 256};
	RECT_FIXED normbg_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= -100 && stage.song_step <= 1040)
		Stage_DrawTex(&this->tex_back0, &normbg_src, &normbg_dst, stage.camera.bzoom, stage.camera.angle);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2832 && stage.song_step <= 4111)
		Stage_DrawTex(&this->tex_back0, &normbg_src, &normbg_dst, stage.camera.bzoom, stage.camera.angle);
	
	RECT normbgflip_src = {0, 0, 256, 256};
	RECT_FIXED normbgflip_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
		Stage_DrawTex(&this->tex_back7, &normbgflip_src, &normbgflip_dst, stage.camera.bzoom, stage.camera.angle);
	
	//Draw xenophanes background
	RECT xenobg_src = {0, 0, 256, 256};
	RECT_FIXED xenobg_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
		Stage_DrawTex(&this->tex_back6, &xenobg_src, &xenobg_dst, stage.camera.bzoom, stage.camera.angle);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
		Stage_DrawTex(&this->tex_back6, &xenobg_src, &xenobg_dst, stage.camera.bzoom, stage.camera.angle);
	
	RECT xenobgflip_src = {0, 0, 256, 256};
	RECT_FIXED xenobgflip_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
		Stage_DrawTex(&this->tex_back10, &xenobgflip_src, &xenobgflip_dst, stage.camera.bzoom, stage.camera.angle);	
	
	//Draw backdrop
	RECT backdrop_src = {0, 0, 256, 256};
	RECT_FIXED backdrop_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= -100 && stage.song_step <= 1040)
		Stage_DrawTex(&this->tex_back2, &backdrop_src, &backdrop_dst, stage.camera.bzoom, stage.camera.angle);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1296 && stage.song_step <= 2320)
		Stage_DrawTex(&this->tex_back2, &backdrop_src, &backdrop_dst, stage.camera.bzoom, stage.camera.angle);
	if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2832 && stage.song_step <= 4111)
		Stage_DrawTex(&this->tex_back2, &backdrop_src, &backdrop_dst, stage.camera.bzoom, stage.camera.angle);

	//Draw xenophanes backdrop
	RECT static0_src = { 0, 0, 85, 85 };
	RECT_FIXED static0_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static1_src = { 85, 0, 85, 85 };
	RECT_FIXED static1_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static2_src = { 170, 0, 85, 85 };
	RECT_FIXED static2_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static3_src = { 0, 85, 85, 85 };
	RECT_FIXED static3_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static4_src = { 85, 85, 85, 85 };
	RECT_FIXED static4_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static5_src = { 170, 85, 85, 85 };
	RECT_FIXED static5_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static6_src = { 0, 170, 85, 85 };
	RECT_FIXED static6_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static7_src = { 85, 170, 85, 85 };
	RECT_FIXED static7_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};

	RECT static8_src = { 170, 170, 85, 85 };
	RECT_FIXED static8_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(490,1),
		FIXED_DEC(380,1)
	};
	
	switch (swapfard)
	{
		case 0:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 1:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 2:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 3:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 4:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static1_src, &static1_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 5:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 6:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 7:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 8:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 9:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static2_src, &static2_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 10:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 11:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 12:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 13:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 14:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static3_src, &static3_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 15:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 16:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 17:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 18:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 19:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static4_src, &static4_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 20:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 21:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 22:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 23:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 24:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static5_src, &static5_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 25:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 26:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 27:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 28:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 29:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static6_src, &static6_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 30:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 31:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 32:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 33:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 34:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static7_src, &static7_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 35:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 36:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 37:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 38:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 39:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static8_src, &static8_dst, stage.camera.bzoom, stage.camera.angle);
		break;
				case 40:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 41:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 42:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 43:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		case 44:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
		break;
		default:		
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 1040 && stage.song_step <= 1296)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 2320 && stage.song_step <= 2832)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
			if (stage.stage_id == StageId_Mod1_6 && stage.song_step >= 4111 && stage.song_step <= 5250)
			Stage_DrawTex(&this->tex_back3, &static0_src, &static0_dst, stage.camera.bzoom, stage.camera.angle);
		break;
	}
}

void Back_Trio_Free(StageBack *back)
{
	Back_Trio *this = (Back_Trio*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Trio_New(void)
{
	//Allocate background structure
	Back_Trio *this = (Back_Trio*)Mem_Alloc(sizeof(Back_Trio));
	if (this == NULL)
		return NULL;

	//Set background functions
	this->back.draw_hud = Back_Trio_DrawHUD;
	this->back.draw_fg = Back_Trio_DrawFG;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Trio_DrawBG;
	this->back.free = Back_Trio_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\TRIO\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "back2.tim"), 0);
	Gfx_LoadTex(&this->tex_back3, Archive_Find(arc_back, "back3.tim"), 0);
	Gfx_LoadTex(&this->tex_back4, Archive_Find(arc_back, "back4.tim"), 0);
	Gfx_LoadTex(&this->tex_back5, Archive_Find(arc_back, "back5.tim"), 0);
	Gfx_LoadTex(&this->tex_back6, Archive_Find(arc_back, "back6.tim"), 0);
	Gfx_LoadTex(&this->tex_back7, Archive_Find(arc_back, "back7.tim"), 0);
	Gfx_LoadTex(&this->tex_back8, Archive_Find(arc_back, "back8.tim"), 0);
	Gfx_LoadTex(&this->tex_back9, Archive_Find(arc_back, "back9.tim"), 0);
	Gfx_LoadTex(&this->tex_back10, Archive_Find(arc_back, "back10.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
