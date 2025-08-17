/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sillybg.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

int lyrics_select;
int movenotes;
//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;

	//Textures
	Gfx_Tex tex_back0; //Normal mirror left
	Gfx_Tex tex_back1; //Normal mirror right

	Gfx_Tex tex_broke0; //Broken mirror left
	Gfx_Tex tex_broke1; //Broken mirror right

	Gfx_Tex tex_floor; //Floor

	Gfx_Tex tex_frame0; //mirror pillars left

} Back_Sillybg;

void Back_Sillybg_DrawHUD()
{

	struct
	{
              const char *Lyrics;
	} song_lyrics[] = {
	      {" "},
    	  {"ILL MAKE"},
	      {"YOU SAY"},
	      {"HOW PROUD"},
	      {"YOU"},
	      {"YOU ARE"},
	      {"YOU ARE OF"},
	      {"YOU ARE OF ME"},
	      {"SO"},
	      {"SO STAY"},
	      {"AWAKE"},
	      {"JUST"},
	      {"LONG"},
	      {"ENOUGH TO SEE"},
	      {"MY"},
	      {"MY WAY"},
	};
	if(stage.stage_id == StageId_5_7)
	{
		switch (stage.song_step)
		{
			case 0:
				lyrics_select=0;
				break;
			case 1424:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1428:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1440:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1444:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1456:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1460:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1472:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1476:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1488:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1492:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1504:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1508:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1520:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1524:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1536:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1540:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1552:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1568:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1572:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1584:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1588:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1600:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1604:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1616:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1620:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1632:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1636:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1648:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1652:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1664:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1668:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1680:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1684:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1688:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1692:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1696:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1700:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1704:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1708:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1712:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1716:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1720:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1724:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1728:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1744:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1748:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1752:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1756:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1760:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1764:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1768:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1772:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1776:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1780:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1784:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1788:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1792:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1824:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1830:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1836:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1856:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1862:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 1868:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2464:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2466:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2467:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2469:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2470:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2496:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2498:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2499:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2501:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2502:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2506:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2508:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2510:
				stage.bump = FIXED_DEC(110,100);
				break;
			case 2512:
				stage.bump = FIXED_DEC(110,100);
				break;
			/*case 3356:
				stage.nohud=1;
				break;*/
			case 3360:
				lyrics_select=1;
				break;
			case 3400:
				lyrics_select=2;
				break;
			case 3432:
				lyrics_select=3;
				break;
			case 3464:
				lyrics_select=4;
				break;
			case 3472:
				lyrics_select=5;
				break;
			case 3478:
				lyrics_select=6;
				break;
			case 3484:
				lyrics_select=7;
				break;
			case 3495:
				lyrics_select=8;
				break;
			case 3504:
				lyrics_select=9;
				break;
			case 3528:
				lyrics_select=10;
				break;
			case 3561:
				lyrics_select=11;
				break;
			case 3568:
				lyrics_select=12;
				break;
			case 3592:
				lyrics_select=13;
				//stage.note_x[0] = FIXED_DEC(-51,1);
				//stage.note_x[1] = FIXED_DEC(-17,1);
				//stage.note_x[2] = FIXED_DEC(17,1);
				//stage.note_x[3] = FIXED_DEC(51,1);
				break;
			/*case 3621:
				stage.nohud=0;
				stage.notesonly=true;*/
				break;
			case 3623:
				lyrics_select=14;
				break;
			case 3632:
				lyrics_select=15;
				break;
			case 3696:
				lyrics_select=0;
				break;
			case 3752:
				lyrics_select=15;
				break;
			case 3807:
				lyrics_select=0;
				break;
			/*case 3888:
				stage.bluenotes=true;
				break;*/
		}
	}
	stage.font_bold.draw_col(&stage.font_bold, song_lyrics[lyrics_select].Lyrics, 160, 223, FontAlign_Center, 202 >> 1, 172 >> 1, 61 >> 1);

	/*if(stage.song_step >= 3888 && movenotes <77)
	{
		stage.note_x[0] = FIXED_DEC(-51+movenotes,1);
		stage.note_x[1] = FIXED_DEC(-17+movenotes,1);
		stage.note_x[2] = FIXED_DEC(17+movenotes,1);
		stage.note_x[3] = FIXED_DEC(51+movenotes,1);
		movenotes++;
	}
	else if (movenotes==77)
	{
		stage.note_x[0] = FIXED_DEC(26,1);
		stage.note_x[1] = FIXED_DEC(60,1);
		stage.note_x[2] = FIXED_DEC(94,1);
		stage.note_x[3] = FIXED_DEC(128,1);
	}*/
	RECT tline = {0, 16, 320, 1};
	Gfx_DrawRect(&tline, 255, 255, 255);

	RECT Top_bar = {0, 0, 320, 16};
    Gfx_BlendRect(&Top_bar, 0, 0, 0, -100);

    RECT bline = {0, 221, 320, 1};
	Gfx_DrawRect(&bline, 255, 255, 255);

	RECT Bottom_bar = {0, 222, 320, 18};
	Gfx_BlendRect(&Bottom_bar, 0, 0, 0, -100);

	if(stage.song_step >= -30 && stage.song_step <= 128)
	{
		RECT blackstart = {0, 0, 320, 240};
		Gfx_DrawRect(&blackstart, 0, 0, 0);
	}
	if(stage.song_step >= 4176)
	{
		RECT blackend = {0, 0, 320, 240};
		Gfx_DrawRect(&blackend, 0, 0, 0);
	}
}
void Back_Sillybg_DrawMG(StageBack *back)
{
	Back_Sillybg *this = (Back_Sillybg*)back;

	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;

	RECT framel_src = {0, 0, 252, 187};
	RECT_FIXED framel_dst = {
		FIXED_DEC(-250+83 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-137-92,1) - fy,
		FIXED_DEC(297 + SCREEN_WIDEOADD,1),//252
		FIXED_DEC(282,1)//187
	};
	Stage_DrawTex(&this->tex_frame0, &framel_src, &framel_dst, stage.camera.bzoom, stage.camera.angle); //frame left

	RECT floorl_src = {0, 0, 256, 128};
	RECT_FIXED floorl_dst = {
		FIXED_DEC(-205+26 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140+161,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};
	Stage_DrawTex(&this->tex_floor, &floorl_src, &floorl_dst, stage.camera.bzoom, stage.camera.angle); //floor left

	RECT floorr_src = {0, 128, 256, 128};
	RECT_FIXED floorr_dst = {
		FIXED_DEC(49+26 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140+161,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};
	Stage_DrawTex(&this->tex_floor, &floorr_src, &floorr_dst, stage.camera.bzoom, stage.camera.angle); //floor right
}
//Week 1 background functions
void Back_Sillybg_DrawBG(StageBack *back)
{
	Back_Sillybg *this = (Back_Sillybg*)back;

	fixed_t fx, fy;

	fx = stage.camera.x;
	fy = stage.camera.y;

	//26 161
	RECT halll_src = {0, 0, 256, 256};
	RECT_FIXED halll_dst = {
		FIXED_DEC(-205-40 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140-91,1) - fy,
		FIXED_DEC(302 + SCREEN_WIDEOADD,1),
		FIXED_DEC(387,1)
	};

	RECT hallr_src = {0, 0, 256, 256};
	RECT_FIXED hallr_dst = {
		FIXED_DEC(49+45-40 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140-91,1) - fy,
		FIXED_DEC(301 + SCREEN_WIDEOADD,1),
		FIXED_DEC(387,1)
	};
	if(stage.song_step >=3439 && stage.song_step <=3459)//3439 to 3459
	{
		RECT flash = {0, 0, 320, 240};
       		Gfx_BlendRect(&flash, 255, 255, 255, 255);
       	}


	if(stage.song_step >=3440)//3439
	{
		Stage_DrawTex(&this->tex_broke0, &halll_src, &halll_dst, stage.camera.bzoom, stage.camera.angle); //Broken mirror left
		Stage_DrawTex(&this->tex_broke1, &hallr_src, &hallr_dst, stage.camera.bzoom, stage.camera.angle); //Broken mirror right
	}
	else
	{
		Stage_DrawTex(&this->tex_back0, &halll_src, &halll_dst, stage.camera.bzoom, stage.camera.angle); //Normal mirror left
		Stage_DrawTex(&this->tex_back1, &hallr_src, &hallr_dst, stage.camera.bzoom, stage.camera.angle); //Normal mirror right
	}
}
void Back_Sillybg_Free(StageBack *back)
{
	Back_Sillybg *this = (Back_Sillybg*)back;

	//Free structure
	Mem_Free(this);
}

StageBack *Back_Sillybg_New(void)
{
	//Allocate background structure
	Back_Sillybg *this = (Back_Sillybg*)Mem_Alloc(sizeof(Back_Sillybg));
	if (this == NULL)
		return NULL;

	//Set background functions
	this->back.draw_hud = Back_Sillybg_DrawHUD;
	this->back.draw_fg = NULL;
	this->back.draw_md = Back_Sillybg_DrawMG;
	this->back.draw_bg = Back_Sillybg_DrawBG;
	this->back.free = Back_Sillybg_Free;

	//Load background textures
	IO_Data arc_back = IO_Read("\\BILLY\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_broke0, Archive_Find(arc_back, "broke0.tim"), 0);
	Gfx_LoadTex(&this->tex_broke1, Archive_Find(arc_back, "broke1.tim"), 0);
	Gfx_LoadTex(&this->tex_floor, Archive_Find(arc_back, "floor.tim"), 0);
	Gfx_LoadTex(&this->tex_frame0, Archive_Find(arc_back, "frame0.tim"), 0);
	Mem_Free(arc_back);

	lyrics_select=0;
	//stage.bluenotes=false;

	Gfx_SetClear(0, 0, 0);

	return (StageBack*)this;
}