/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "kitchen.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"
#include <stdlib.h>

//Kitchen background structure
typedef struct
{
	//Stage background base structure
	StageBack back;

	IO_Data arc_handa, arc_handa_ptr[1];
	IO_Data arc_handb, arc_handb_ptr[1];
	IO_Data arc_handc, arc_handc_ptr[1];
	
	//Textures
	Gfx_Tex tex_back0;
	Gfx_Tex tex_back1;
	Gfx_Tex tex_cut0;
	
	Gfx_Tex tex_handa;
	u8 handa_frame, handa_tex_id;
	Gfx_Tex tex_handb;
	u8 handb_frame, handb_tex_id;
	Gfx_Tex tex_handc;
	u8 handc_frame, handc_tex_id;
	
	Animatable handa_animatable;
	Animatable handb_animatable;
	Animatable handc_animatable;
	
	fixed_t handb_y;
    fixed_t handa_y;
    fixed_t handc_y;
	
	fixed_t fade, fadespd;
	
} Back_Kitchen;

static const CharFrame handa_frame[3] = {
  {0, {  0,  0, 50, 51}, {160,160}}, //0 handa 1
  {0, { 50,  0, 50, 51}, {160,160}}, //1 handa 2
  {0, { 100,  0, 50, 51}, {160,160}}, //2 handa 3
};

static const CharFrame handb_frame[3] = {
  {0, {0,  0, 41,  71}, {160,160}}, //3 handb 1
  {0, {41,  0, 41, 71}, {160,160}}, //4 handb 2
  {0, {82,  0, 41, 71}, {160,160}}, //5 handb 3
};

static const CharFrame handc_frame[3] = {
  {0, {  0, 0, 44, 72}, {160,160}}, //6 handc 1
  {0, { 44, 0, 44, 72}, {160,160}}, //7 handc 2
  {0, { 88, 0, 44, 72}, {160,160}}, //8 handc 3
};

static const Animation handa_anim[1] = {
	{6, (const u8[]) {0, 1, 2, ASCR_BACK, 0}},
};

static const Animation handb_anim[1] = {
	{6, (const u8[]) {0, 1, 2, ASCR_BACK, 0}},
};

static const Animation handc_anim[1] = {
	{6, (const u8[]) {0, 1, 2, ASCR_BACK, 0}},
};

void Kitchen_HandA_SetFrame(void* user, u8 frame)
{
	Back_Kitchen* this = (Back_Kitchen*)user;

	//Check if this is a new frame
	if (frame != this->handa_frame)
	{
		//Check if new art shall be loaded
		const CharFrame* cframe = &handa_frame[this->handa_frame = frame];
		if (cframe->tex != this->handa_tex_id)
			Gfx_LoadTex(&this->tex_handa, this->arc_handa_ptr[this->handa_tex_id = cframe->tex], 0);
	}
}

void Kitchen_HandB_SetFrame(void* user, u8 frame)
{
	Back_Kitchen* this = (Back_Kitchen*)user;

	//Check if this is a new frame
	if (frame != this->handb_frame)
	{
		//Check if new art shall be loaded
		const CharFrame* cframe = &handb_frame[this->handb_frame = frame];
		if (cframe->tex != this->handb_tex_id)
			Gfx_LoadTex(&this->tex_handb, this->arc_handb_ptr[this->handb_tex_id = cframe->tex], 0);
	}
}

void Kitchen_HandC_SetFrame(void* user, u8 frame)
{
	Back_Kitchen* this = (Back_Kitchen*)user;

	//Check if this is a new frame
	if (frame != this->handc_frame)
	{
		//Check if new art shall be loaded
		const CharFrame* cframe = &handc_frame[this->handc_frame = frame];
		if (cframe->tex != this->handc_tex_id)
			Gfx_LoadTex(&this->tex_handc, this->arc_handc_ptr[this->handc_tex_id = cframe->tex], 0);
	}
}

void Kitchen_HandA_Draw(Back_Kitchen* this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame* cframe = &handa_frame[this->handa_frame];

	fixed_t ox = x;
	fixed_t oy = y;

	RECT src = { cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3] };
	RECT_FIXED dst = { ox, oy, (-src.w * 3) << FIXED_SHIFT, (src.h * 3) << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_handa, &src, &dst, stage.camera.bzoom, stage.camera.angle);
}

void Kitchen_HandB_Draw(Back_Kitchen* this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame* cframe = &handb_frame[this->handb_frame];

	fixed_t ox = x;
	fixed_t oy = y;

	RECT src = { cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3] };
	RECT_FIXED dst = { ox, oy, (src.w * 3) << FIXED_SHIFT, (src.h * 3) << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_handb, &src, &dst, stage.camera.bzoom, stage.camera.angle);
}

void Kitchen_HandC_Draw(Back_Kitchen* this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame* cframe = &handc_frame[this->handc_frame];

	fixed_t ox = x;
	fixed_t oy = y;

	RECT src = { cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3] };
	RECT_FIXED dst = { ox, oy, (-src.w * 3) << FIXED_SHIFT, (src.h * 3) << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_handc, &src, &dst, stage.camera.bzoom, stage.camera.angle);
}

void Back_Kitchen_DrawHUD(StageBack *back)
{
    Back_Kitchen *this = (Back_Kitchen*)back;
    
    fixed_t fx, fy;
    
    fx = stage.camera.x;
    fy = stage.camera.y;
    
    if (stage.song_step == 784)
    {
        this->fade = FIXED_DEC(255,1);
        this->fadespd = FIXED_DEC(175,1);
    }
    if (this->fade > 0)
    {
        RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        u8 flash_col = this->fade >> FIXED_SHIFT;
        Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 2);
        if (!stage.paused)
            this->fade -= FIXED_MUL(this->fadespd, timer_dt);
    }

    // Persistent fruit variables to maintain state between steps
    static int last_fruit_step = 1824;
    static int random_fruit = -1;
    static RECT src;
    static RECT_FIXED dst;
    static boolean fruit_active = false;

    // Reset when stage is restarted (before step 1824)
    if (stage.song_step == 0)
    {
        last_fruit_step = 1824;
        fruit_active = false;
    }

    // Trigger a new fruit every 128 steps starting from step 1824
    if (stage.song_step >= 1824 && (stage.song_step - last_fruit_step) >= 128)
    {
        last_fruit_step = stage.song_step; // Update the last trigger step

        // Choose a new random fruit and its starting x, y position
        random_fruit = rand() % 4; // 4 fruits in total
        fixed_t start_x = FIXED_DEC(-100 - (rand() % 157), 1); // Random x in range -100 to -256
        fixed_t start_y = FIXED_DEC(-100 - (rand() % 157), 1); // Random y in range -100 to -256

        // Set fruit source based on random selection
        switch (random_fruit)
        {
            case 0:
                src.x = 128; src.y = 128; src.w = 64; src.h = 64;
                break;
            case 1:
                src.x = 192; src.y = 128; src.w = 64; src.h = 64;
                break;
            case 2:
                src.x = 128; src.y = 192; src.w = 64; src.h = 64;
                break;
            case 3:
                src.x = 192; src.y = 192; src.w = 64; src.h = 64;
                break;
        }

        // Set destination starting position and mark fruit as active
        dst.x = start_x - fx;
        dst.y = start_y - fy;
        dst.w = FIXED_DEC(128, 1);
        dst.h = FIXED_DEC(128, 1);
        fruit_active = true;
    }

    // If fruit is active, move it and draw it
    if (fruit_active)
    {
        // Move the fruit across the screen with a fixed speed
        dst.x += FIXED_DEC(2, 1); // Speed along x-axis
        dst.y += FIXED_DEC(1, 1); // Speed along y-axis

        Stage_DrawTex(&this->tex_back0, &src, &dst, stage.camera.bzoom, stage.camera.angle);

        // Deactivate fruit after the next 128 steps
        if ((stage.song_step - last_fruit_step) >= 128)
        {
            fruit_active = false;
        }
    }
}

void Back_Kitchen_DrawFG(StageBack *back)
{
	Back_Kitchen *this = (Back_Kitchen*)back;
	
	fixed_t fx, fy;
	
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;
	
	RECT window_src = { 0, 172, 256, 83 };
	RECT_FIXED window_dst = {
		FIXED_DEC(-80 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-64,1) - fy,
		FIXED_DEC(172 + SCREEN_WIDEOADD,1),
		FIXED_DEC(83,1)
	};
	
	if (stage.song_step >= -48 && stage.song_step <= 16)
	{
		Stage_DrawTex(&this->tex_back1, &window_src, &window_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT jump_src = { 0, 0, 256, 128 };
	RECT_FIXED jump_dst = {
		FIXED_DEC(-140 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-90,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(240,1)
	};
	
	if (stage.song_step >= 512 && stage.song_step <= 528)
	{
		Stage_DrawTex(&this->tex_back1, &jump_src, &jump_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT gracetext_src = { 0, 145, 130, 17 };
	RECT_FIXED gracetext_dst = {
		FIXED_DEC(-80 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-64,1) - fy,
		FIXED_DEC(130 + SCREEN_WIDEOADD,1),
		FIXED_DEC(17,1)
	};
	
	Debug_StageMoveDebug(&gracetext_dst, 9, fx, fy);
	if (stage.song_step >= 2080 && stage.song_step <= 2084)
	{
		Stage_DrawTex(&this->tex_back1, &gracetext_src, &gracetext_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT soundtext_src = { 0, 129, 197, 14 };
	RECT_FIXED soundtext_dst = {
		FIXED_DEC(-80 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-64,1) - fy,
		FIXED_DEC(197 + SCREEN_WIDEOADD,1),
		FIXED_DEC(14,1)
	};
	
	Debug_StageMoveDebug(&soundtext_dst, 9, fx, fy);
	if (stage.song_step >= 2208 && stage.song_step <= 2224)
	{
		// Update src.x and dst.w based on song_step
		switch(stage.song_step) {
			case 2208:
				soundtext_src.w = 37;
				soundtext_dst.w = FIXED_DEC(37 + SCREEN_WIDEOADD, 1);
				break;
			case 2209:
				soundtext_src.w = 37;
				soundtext_dst.w = FIXED_DEC(37 + SCREEN_WIDEOADD, 1);
				break;
			case 2210:
				soundtext_src.w = 37;
				soundtext_dst.w = FIXED_DEC(37 + SCREEN_WIDEOADD, 1);
				break;
			case 2211:
				soundtext_src.w = 37;
				soundtext_dst.w = FIXED_DEC(37 + SCREEN_WIDEOADD, 1);
				break;
			case 2212:
				soundtext_src.w = 95;
				soundtext_dst.w = FIXED_DEC(95 + SCREEN_WIDEOADD, 1);
				break;
			case 2213:
				soundtext_src.w = 95;
				soundtext_dst.w = FIXED_DEC(95 + SCREEN_WIDEOADD, 1);
				break;
			case 2214:
				soundtext_src.w = 95;
				soundtext_dst.w = FIXED_DEC(95 + SCREEN_WIDEOADD, 1);
				break;
			case 2215:
				soundtext_src.w = 95;
				soundtext_dst.w = FIXED_DEC(95 + SCREEN_WIDEOADD, 1);
				break;
			case 2216:
				soundtext_src.w = 131;
				soundtext_dst.w = FIXED_DEC(131 + SCREEN_WIDEOADD, 1);
				break;
			case 2217:
				soundtext_src.w = 131;
				soundtext_dst.w = FIXED_DEC(131 + SCREEN_WIDEOADD, 1);
				break;
			case 2218:
				soundtext_src.w = 131;
				soundtext_dst.w = FIXED_DEC(131 + SCREEN_WIDEOADD, 1);
				break;
			case 2219:
				soundtext_src.w = 131;
				soundtext_dst.w = FIXED_DEC(131 + SCREEN_WIDEOADD, 1);
				break;
			case 2220:
				soundtext_src.w = 197;
				soundtext_dst.w = FIXED_DEC(197 + SCREEN_WIDEOADD, 1);
				break;
		}
		
		// Draw the texture with updated src and dst
		Stage_DrawTex(&this->tex_back1, &soundtext_src, &soundtext_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT back_src = {0, 0, 8, 8};
	RECT_FIXED back_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	if (stage.song_step >= -48 && stage.song_step <= 16)
	{
		Stage_DrawTex(&this->tex_back1, &back_src, &back_dst, stage.camera.bzoom, stage.camera.angle);
	}
	if (stage.song_step >= 512 && stage.song_step <= 528)
	{
		Stage_DrawTex(&this->tex_back1, &back_src, &back_dst, stage.camera.bzoom, stage.camera.angle);
	}
	if (stage.song_step >= 1304 && stage.song_step <= 1824)
	{
		Stage_DrawTex(&this->tex_back1, &back_src, &back_dst, stage.camera.bzoom, stage.camera.angle);
	}
	if (stage.song_step >= 2080 && stage.song_step <= 2084)
	{
		Stage_DrawTex(&this->tex_back1, &back_src, &back_dst, stage.camera.bzoom, stage.camera.angle);
	}
	if (stage.song_step >= 2208 && stage.song_step <= 2224)
	{
		Stage_DrawTex(&this->tex_back1, &back_src, &back_dst, stage.camera.bzoom, stage.camera.angle);
	}
		if (stage.song_step >= 2364 && stage.song_step <= 5500)
	{
		Stage_DrawTex(&this->tex_back1, &back_src, &back_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x >> 4;
	fy = stage.camera.y >> 4;

	RECT fire_src = {0, 0, 128, 128};
	RECT_FIXED fire_dst = {
		FIXED_DEC(-172 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-132,1) - fy,
		FIXED_DEC(400 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	if (stage.song_step >= 1816 && stage.song_step <= 5500)
	{
		Stage_BlendTex(&this->tex_cut0, &fire_src, &fire_dst, stage.camera.bzoom, stage.camera.angle, 0);
	}
}


//Kitchen background functions
void Back_Kitchen_DrawBG(StageBack *back)
{
	Back_Kitchen *this = (Back_Kitchen*)back;
	
	fixed_t fx, fy;
	
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT back1_src = {0, 0, 128, 128};
	RECT_FIXED back1_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(553,1),
		FIXED_DEC(367,1)
	};
	
	if (stage.song_step >= -48 && stage.song_step <= 784)
	{
		Stage_DrawTex(&this->tex_back0, &back1_src, &back1_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT back2_src = {128, 0, 128, 128};
	RECT_FIXED back2_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(553,1),
		FIXED_DEC(367,1)
	};
	
	if (stage.song_step >= 784 && stage.song_step <= 1824)
	{
		Stage_DrawTex(&this->tex_back0, &back2_src, &back2_dst, stage.camera.bzoom, stage.camera.angle);
	}
	
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT back3_src = {0, 128, 128, 128};
	RECT_FIXED back3_dst = {
		FIXED_DEC(-275,1) - fx,
		FIXED_DEC(-200,1) - fy,
		FIXED_DEC(553,1),
		FIXED_DEC(367,1)
	};
	
	if (stage.song_step >= 1824 && stage.song_step <= 5500)
	{
		Stage_DrawTex(&this->tex_back0, &back3_src, &back3_dst, stage.camera.bzoom, stage.camera.angle);
	}
}

void Back_Kitchen_DrawMD(StageBack *back)
{
    Back_Kitchen *this = (Back_Kitchen*)back;

    fixed_t fx, fy;
    fixed_t target_y_handb = FIXED_DEC(-56, 1);
    fixed_t target_y_handa = FIXED_DEC(-70, 1);
    fixed_t target_y_handc = FIXED_DEC(-116, 1);

    fx = stage.camera.x;
    fy = stage.camera.y;

    if (stage.flag & STAGE_FLAG_JUST_STEP)
    {
        switch (stage.song_step % 4)
        {
            case 0:
                Animatable_SetAnim(&this->handb_animatable, 0);
                Animatable_SetAnim(&this->handa_animatable, 0);
                Animatable_SetAnim(&this->handc_animatable, 0);
            break;
        }

        // Initialize the hands' y positions once if they are at the ground level
        if (stage.song_step == 0)
        {
            this->handb_y = FIXED_DEC(256, 1);
            this->handa_y = FIXED_DEC(256, 1);
            this->handc_y = FIXED_DEC(256, 1);
        }
        else if (stage.song_step >= 1924 && stage.song_step <= 1948)
        {
            fixed_t rise_speed = FIXED_DEC(16, 1);  // Adjust speed as needed

            // Gradually increase y position towards target values
            if (this->handb_y > target_y_handb)
                this->handb_y -= rise_speed;
            if (this->handa_y > target_y_handa)
                this->handa_y -= rise_speed;
            if (this->handc_y > target_y_handc)
                this->handc_y -= rise_speed;
        }
        else if (stage.song_step == 1948)
        {
            // Set the final positions once risen
            this->handb_y = target_y_handb;
            this->handa_y = target_y_handa;
            this->handc_y = target_y_handc;
        }
    }

    // Animate and draw hands
    Animatable_Animate(&this->handb_animatable, (void*)this, Kitchen_HandB_SetFrame);
    Kitchen_HandB_Draw(this, FIXED_DEC(-48, 1) - fx, this->handb_y - fy);

    Animatable_Animate(&this->handa_animatable, (void*)this, Kitchen_HandA_SetFrame);
    Kitchen_HandA_Draw(this, FIXED_DEC(55, 1) - fx, this->handa_y - fy);

    Animatable_Animate(&this->handc_animatable, (void*)this, Kitchen_HandC_SetFrame);
    Kitchen_HandC_Draw(this, FIXED_DEC(20, 1) - fx, this->handc_y - fy);
}

void Back_Kitchen_Free(StageBack *back)
{
	Back_Kitchen *this = (Back_Kitchen*)back;
	
	Mem_Free(this->arc_handa);
	Mem_Free(this->arc_handb);
	Mem_Free(this->arc_handc);
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Kitchen_New(void)
{
	//Allocate background structure
	Back_Kitchen *this = (Back_Kitchen*)Mem_Alloc(sizeof(Back_Kitchen));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_hud = Back_Kitchen_DrawHUD;
	this->back.draw_fg = Back_Kitchen_DrawFG;
	this->back.draw_md = Back_Kitchen_DrawMD;
	this->back.draw_bg = Back_Kitchen_DrawBG;
	this->back.free = Back_Kitchen_Free;
	
	//Use pitch black background
	Gfx_SetClear(0, 0, 0);
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\KITCHEN\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_cut0, Archive_Find(arc_back, "cut0.tim"), 0);
	Mem_Free(arc_back);
	
	//Load hand textures
	this->arc_handa = IO_Read("\\KITCHEN\\HANDA.ARC;1");
	this->arc_handa_ptr[0] = Archive_Find(this->arc_handa, "handa.tim");
	this->arc_handb = IO_Read("\\KITCHEN\\HANDB.ARC;1");
	this->arc_handb_ptr[0] = Archive_Find(this->arc_handb, "handb.tim");
	this->arc_handc = IO_Read("\\KITCHEN\\HANDC.ARC;1");
	this->arc_handc_ptr[0] = Archive_Find(this->arc_handc, "handc.tim");

	//Initialize hand state
	Animatable_Init(&this->handa_animatable, handa_anim);
	Animatable_Init(&this->handb_animatable, handb_anim);
	Animatable_Init(&this->handc_animatable, handc_anim);
	
	Animatable_SetAnim(&this->handa_animatable, 0);
	Animatable_SetAnim(&this->handb_animatable, 0);
	Animatable_SetAnim(&this->handc_animatable, 0);
	
	this->handa_frame = this->handa_tex_id = 0xFF; //Force art load
	this->handb_frame = this->handb_tex_id = 0xFE; //Force art load
	this->handc_frame = this->handc_tex_id = 0xFD; //Force art load
	
	return (StageBack*)this;
}
