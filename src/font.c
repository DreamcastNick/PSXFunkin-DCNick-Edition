#include "font.h"

#include "main.h"
#include "timer.h"
#include "stage.h"

#include <string.h>

Fonts fonts;

void Initalize_Fonts(void)
{
	FontData_Load(&fonts.font_cdr, Font_CDR);
	FontData_Load(&fonts.font_bold, Font_Bold);
	FontData_Load(&fonts.font_arial, Font_Arial);
}

//Font_Bold
s32 Font_Bold_GetWidth(struct FontData *this, const char *text)
{
	(void)this;
	return strlen(text) * 13;
}

void Font_Bold_DrawCol(struct FontData *this, const char *text, s32 x, s32 y, FontAlign align, u8 r, u8 g, u8 b)
{
	//Offset position based off alignment
	switch (align)
	{
		case FontAlign_Left:
			break;
		case FontAlign_Center:
			x -= Font_Bold_GetWidth(this, text) >> 1;
			break;
		case FontAlign_Right:
			x -= Font_Bold_GetWidth(this, text);
			break;
	}
	
	//Get animation offsets
	u32 v0 = 0;
	u8 v1 = (animf_count >> 1) & 1;
	
	//Draw string character by character
	u8 c;
	while ((c = *text++) != '\0')
	{
		//Draw character
		if ((c -= 'A') <= 'z' - 'A') //Lower-case will show inverted colours
		{
			RECT src = {((c & 0x7) << 5) + ((((v0 >> (c & 0x1F)) & 1) ^ v1) << 4), (c & ~0x7) << 1, 16, 16};
			Gfx_BlitTexCol(&this->tex, &src, x, y, r, g, b);
			v0 ^= 1 << (c & 0x1F);
		}
		x += 13;
	}
}

//Font_Arial
#include "font_arialmap.h"

s32 Font_Arial_GetWidth(struct FontData *this, const char *text)
{
	(void)this;
	
	//Draw string width character by character
	s32 width = 0;
	
	u8 c;
	while ((c = *text++) != '\0')
	{
		//Shift and validate character
		if ((c -= 0x20) >= 0x60)
			continue;
		
		//Add width
		width += font_arialmap[c].gw;
	}
	
	return width;
}

void Font_Arial_DrawCol(struct FontData *this, const char *text, s32 x, s32 y, FontAlign align, u8 r, u8 g, u8 b)
{
	//Offset position based off alignment
	switch (align)
	{
		case FontAlign_Left:
			break;
		case FontAlign_Center:
			x -= Font_Arial_GetWidth(this, text) >> 1;
			break;
		case FontAlign_Right:
			x -= Font_Arial_GetWidth(this, text);
			break;
	}
	
	//Draw string character by character
	u8 c;
	while ((c = *text++) != '\0')
	{
		//Shift and validate character
		if ((c -= 0x20) >= 0x60)
			continue;
		
		//Draw character
		RECT src = {font_arialmap[c].ix, 173 + font_arialmap[c].iy, font_arialmap[c].iw, font_arialmap[c].ih};
		Gfx_BlitTexCol(&this->tex, &src, x + font_arialmap[c].gx, y + font_arialmap[c].gy, r, g, b);
		
		//Increment X
		x += font_arialmap[c].gw;
	}
}

//CD-R font by bilious
#include "font_cdrmap.h"

s32 Font_CDR_GetWidth(struct FontData *this, const char *text)
{
	(void)this;
	
	//Draw string width character by character
	s32 width = 0;
	
	u8 c;
	while ((c = *text++) != '\0')
	{
		//Shift and validate character
		if ((c -= 0x20) >= 0x60)
			continue;
		
		//Add width
		width += font_cdrmap[c].charW;
	}
	
	return width;
}

void Font_CDR_DrawCol(struct FontData *this, const char *text, s32 x, s32 y, FontAlign align, u8 r, u8 g, u8 b)
{
	//Offset position based off alignment
	s32 alignoffset = Font_CDR_GetWidth(this, text);
	
	switch (align)
	{
		case FontAlign_Left:
			alignoffset = 0;
			break;
		case FontAlign_Center:
			alignoffset = alignoffset / 2;
			break;
		case FontAlign_Right:
			alignoffset = alignoffset;
			break;
	}
	
	//Draw string character by character
	u8 c;
	s16 xhold = x;
	while ((c = *text++) != '\0')
	{
		if (c == '\n')
		{
			x = xhold;
			y += (gameloop == GameLoop_Stage) ? FIXED_DEC(11,1) : 11;
		}
		//Shift and validate character
		if ((c -= 0x20) >= 0x60)
			continue;
		
		//Draw character
		RECT src = {font_cdrmap[c].charX, 129 + font_cdrmap[c].charY, font_cdrmap[c].charW, font_cdrmap[c].charL};
		
		if (gameloop == GameLoop_Stage)
		{
			RECT_FIXED dst = {x - FIXED_DEC(alignoffset,1), y, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
			Stage_DrawTexCol(&this->tex, &src, &dst, stage.bump, stage.camera.hudangle, r, g, b);
			x += (font_cdrmap[c].charW - 1) << FIXED_SHIFT;
		}
		else
		{
			RECT dst_f = {x - alignoffset, y, src.w, src.h};
			Gfx_DrawTexCol(&this->tex, &src, &dst_f, r, g, b);
			x += (font_cdrmap[c].charW - 1);
		}
		
	}
}

//Common font functions
void Font_Draw(struct FontData *this, const char *text, s32 x, s32 y, FontAlign align)
{
	this->draw_col(this, text, x, y, align, 0x80, 0x80, 0x80);
}

// Load font texture
static void FontData_LoadTex(FontData *this)
{
    static const char *filename = "\\FONTS\\FONTS.TIM;1";
    Gfx_LoadTex(&this->tex, IO_Read(filename), GFX_LOADTEX_FREE);
}

//Font functions
void FontData_Load(FontData *this, Font font)
{
    //Load texture and set functions
    FontData_LoadTex(this);

    switch (font)
    {
        case Font_Bold:
            this->get_width = Font_Bold_GetWidth;
            this->draw_col = Font_Bold_DrawCol;
            break;
        case Font_Arial:
            this->get_width = Font_Arial_GetWidth;
            this->draw_col = Font_Arial_DrawCol;
            break;
        case Font_CDR:
            this->get_width = Font_CDR_GetWidth;
            this->draw_col = Font_CDR_DrawCol;
            break;
    }

    this->draw = Font_Draw;
}
