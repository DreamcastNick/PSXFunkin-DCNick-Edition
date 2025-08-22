/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "combo.h"

#include "../mem.h"
#include "../timer.h"
#include "../random.h"

//Combo object functions
boolean Obj_Combo_Tick(Object *obj)
{
	Obj_Combo *this = (Obj_Combo*)obj;
	
	//Tick hit type
	if (this->hit_type != 0xFF && this->ht < (FIXED_DEC(16,1) / 60))
	{
		//Get hit src and dst
		u8 clipp = 16;
		if (this->ht > 0)
			clipp = 16 - ((this->ht * 60) >> FIXED_SHIFT);
		
		RECT hit_src = {
			0,
			(this->hit_type << 5),
			80,
			clipp << 1
		};
		RECT_FIXED hit_dst = {
			this->x - FIXED_DEC(8,1),
			this->hy - FIXED_DEC(16,1),
			FIXED_DEC(80,1),
			(FIXED_DEC(32,1) * clipp) >> 4
		};
		Stage_DrawTex(&stage.tex_hud0, &hit_src, &hit_dst, stage.camera.bzoom, stage.camera.hudangle);
		
		//Apply gravity
		this->hy += FIXED_MUL(this->hv, timer_dt);
		this->hv += FIXED_MUL(FIXED_DEC(5,100) * 60 * 60, timer_dt);
	}
	
	//Increment hit type timer
	this->ht += timer_dt;
	
	//Tick combo
	if (this->num_count > 0 && this->ct < (FIXED_DEC(16,1) / 60))
	{
		//Get hit src and dst
		u8 clipp = 16;
		if (this->ct > 0)
			clipp = 16 - ((this->ct * 60) >> FIXED_SHIFT);
		
		RECT combo_src = {
			80,
			0,
			80,
			clipp << 1
		};
		RECT_FIXED combo_dst = {
			this->x + FIXED_DEC(48,1),
			this->cy - FIXED_DEC(16,1),
			FIXED_DEC(60,1),
			(FIXED_DEC(24,1) * clipp) >> 4
		};
		Stage_DrawTex(&stage.tex_hud0, &combo_src, &combo_dst, stage.camera.bzoom, stage.camera.hudangle);
		
		//Apply gravity
		this->cy += FIXED_MUL(this->cv, timer_dt);
		this->cv += FIXED_MUL(FIXED_DEC(3,100) * 60 * 60, timer_dt);
	}
	
	//Increment combo timer
	this->ct += timer_dt;
	
	//Tick numbers
	if (this->numt < (FIXED_DEC(16,1) / 60))
	{
		for (u8 i = 0; i < this->num_count; i++)
		{
			u8 num = this->num[i];
			if (num == 0xFF)
				continue;
			
			//Get number src and dst
			u8 clipp = 16;
			if (this->numt > 0)
				clipp = 16 - ((this->numt * 60) >> FIXED_SHIFT);
			
			RECT num_src = {
				80  + ((num % 5) << 5),
				32 + ((num / 5) << 5),
				32,
				clipp << 1
			};
			// Center the digit strip around the original 5-digit anchor
			fixed_t start_offset = FIXED_DEC((int)((5 - (int)this->num_count) * 8),1);
			RECT_FIXED num_dst = {
				this->x - FIXED_DEC(32,1) + (i * FIXED_DEC(16,1)) - FIXED_DEC(12,1) + start_offset,
				this->numy[i] - FIXED_DEC(12,1),
				FIXED_DEC(24,1),
				(FIXED_DEC(24,1) * clipp) >> 4
			};
			Stage_DrawTex(&stage.tex_hud0, &num_src, &num_dst, stage.camera.bzoom, stage.camera.hudangle);
			
			//Apply gravity
			this->numy[i] += FIXED_MUL(this->numv[i], timer_dt);
			this->numv[i] += FIXED_MUL(FIXED_DEC(3,100) * 60 * 60, timer_dt);
		}
	}
	
	//Increment number timer
	this->numt += timer_dt;
	
	return (this->numt >= FIXED_DEC(16,60)) && (this->ht >= FIXED_DEC(16,60)) && (this->ct >= FIXED_DEC(16,60));
}

boolean Obj_Combo_Tick_Weeb(Object *obj)
{
	Obj_Combo *this = (Obj_Combo*)obj;
	
	//Tick hit type
	if (this->hit_type != 0xFF && this->ht < (FIXED_DEC(16,1) / 60))
	{
		//Get hit src and dst
		u8 clipp = 16;
		if (this->ht > 0)
			clipp = 16 - ((this->ht * 60) >> FIXED_SHIFT);
		
		RECT hit_src = {
			1,
			129 + (this->hit_type * 24),
			70,
			(22 * clipp) >> 4
		};
		RECT_FIXED hit_dst = {
			this->x - FIXED_DEC(8,1),
			this->hy,
			FIXED_DEC(70,1),
			(FIXED_DEC(22,1) * clipp) >> 4
		};
		Stage_DrawTex(&stage.tex_hud0, &hit_src, &hit_dst, stage.camera.bzoom, stage.camera.hudangle);
		
		//Apply gravity
		this->hy += FIXED_MUL(this->hv, timer_dt) >> 1;
		this->hv += FIXED_MUL(FIXED_DEC(5,100) * 60 * 60, timer_dt);
	}
	
	//Increment hit type timer
	this->ht += timer_dt;
	
	//Tick combo
	if (this->num_count > 0 && this->ct < (FIXED_DEC(16,1) / 60))
	{
		//Get hit src and dst
		u8 clipp = 16;
		if (this->ct > 0)
			clipp = 16 - ((this->ct * 60) >> FIXED_SHIFT);
		
		RECT combo_src = {
			73,
			129,
			46,
			(22 * clipp) >> 4
		};
		RECT_FIXED combo_dst = {
			this->x + FIXED_DEC(48,1) - FIXED_DEC(16,1),
			this->cy - FIXED_DEC(16,1),
			FIXED_DEC(46,1),
			(FIXED_DEC(22,1) * clipp) >> 4
		};
		Stage_DrawTex(&stage.tex_hud0, &combo_src, &combo_dst, stage.camera.bzoom, stage.camera.hudangle);
		
		//Apply gravity
		this->cy += FIXED_MUL(this->cv, timer_dt) >> 1;
		this->cv += FIXED_MUL(FIXED_DEC(3,100) * 60 * 60, timer_dt);
	}
	
	//Increment combo timer
	this->ct += timer_dt;
	
	//Tick numbers
	if (this->numt < (FIXED_DEC(16,1) / 60))
	{
		for (u8 i = 0; i < this->num_count; i++)
		{
			u8 num = this->num[i];
			if (num == 0xFF)
				continue;
			
			//Get number src and dst
			u8 clipp = 16;
			if (this->numt > 0)
				clipp = 16 - ((this->numt * 60) >> FIXED_SHIFT);
			
			RECT num_src = {
				72  + (num * 12),
				152,
				11,
				(12 * clipp) >> 4
			};
			// Center the digit strip around the original 5-digit anchor (Weeb font narrower spacing)
			fixed_t start_offset_w = FIXED_DEC((int)((5 - (int)this->num_count) * 4),1);
			RECT_FIXED num_dst = {
				this->x - FIXED_DEC(32,1) + (i * FIXED_DEC(8,1)) + FIXED_DEC(16,1) + start_offset_w,
				this->numy[i] - FIXED_DEC(12,1),
				FIXED_DEC(11,1),
				(FIXED_DEC(12,1) * clipp) >> 4
			};
			Stage_DrawTex(&stage.tex_hud0, &num_src, &num_dst, stage.camera.bzoom, stage.camera.hudangle);
			
			//Apply gravity
			this->numy[i] += FIXED_MUL(this->numv[i], timer_dt) >> 1;
			this->numv[i] += FIXED_MUL(FIXED_DEC(3,100) * 60 * 60, timer_dt);
		}
	}
	
	//Increment number timer
	this->numt += timer_dt;
	
	return (this->numt >= FIXED_DEC(16,60)) && (this->ht >= FIXED_DEC(16,60)) && (this->ct >= FIXED_DEC(16,60));
}

void Obj_Combo_Free(Object *obj)
{
	(void)obj;
}

Obj_Combo *Obj_Combo_New(fixed_t x, fixed_t y, u8 hit_type, u32 combo)
{
	(void)x;
	
	//Allocate new object
	Obj_Combo *this = (Obj_Combo*)Mem_Alloc(sizeof(Obj_Combo));
	if (this == NULL)
		return NULL;
	
	//Set object functions and position
	
	//Regular combo
	this->obj.tick = Obj_Combo_Tick;
	if ((x >= 0) ^ (stage.prefs.mode < StageMode_2P))
		this->x = FIXED_DEC(-112,1) - FIXED_DEC(SCREEN_WIDEADD,4);
	else
		this->x = FIXED_DEC(30,1) + FIXED_DEC(SCREEN_WIDEADD,4);
	y = FIXED_DEC(73,1);
	
	this->obj.free = Obj_Combo_Free;
	
	//Setup hit type
	if ((this->hit_type = hit_type) != 0xFF)
	{
		this->hy = y - FIXED_DEC(38,1);
		this->hv = -(FIXED_DEC(8,10) + RandomRange(0, FIXED_DEC(3,10))) * 60;
	}
	
	//Setup numbers
	if (combo != 0xFFFFFFFF)
	{
		//Clear all digits
		for (u8 i = 0; i < 10; i++)
			this->num[i] = 0xFF;
		this->num_count = 0;
			
		//Write numbers (up to 10 digits)
		static const u32 dig[10] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};
		boolean hit = false;
		for (u8 d = 0; d < 10; d++)
		{
			u8 v = 0;
			while (combo >= dig[d])
			{
				combo -= dig[d];
				v++;
			}
			if (v || hit)
			{
				hit = true;
				this->num[this->num_count++] = v;
			}
		}
		
		//Initialize number positions
		for (u8 i = 0; i < this->num_count; i++)
		{
			if (this->num[i] == 0xFF)
				continue;
			this->numy[i] = y;
			this->numv[i] = -(FIXED_DEC(7,10) + RandomRange(0, FIXED_DEC(18,100))) * 60;
		}
		
		//Setup combo
		this->cy = y;
		this->cv = -(FIXED_DEC(7,10) + RandomRange(0, FIXED_DEC(16,100))) * 60;
	}
	else
	{
		//Write null numbers
		for (u8 i = 0; i < 10; i++) this->num[i] = 0xFF;
		this->num_count = 0;
	}
	
	//Initialize timers
	this->ht = FIXED_DEC(-30,60);
	this->ct = FIXED_DEC(-53,60);
	this->numt = FIXED_DEC(-56,60);
	
	return this;
}
