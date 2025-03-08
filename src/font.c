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

// Define character positions and sizes with animation support
typedef struct {
    u8 x, y, width, height;
} CharRect;

CharRect char_map[128][4] = {
    // Uppercase letters
    ['A'] = {{0, 0, 16, 16}, {16, 0, 16, 16}, {0, 64, 16, 16}, {16, 64, 16, 16}}, 
    ['B'] = {{32, 0, 16, 16}, {48, 0, 16, 16}, {32, 64, 16, 16}, {48, 64, 16, 16}}, 
    ['C'] = {{64, 0, 16, 16}, {80, 0, 16, 16}, {64, 64, 16, 16}, {80, 64, 16, 16}}, 
    ['D'] = {{96, 0, 16, 16}, {112, 0, 16, 16}, {96, 64, 16, 16}, {112, 64, 16, 16}}, 
    ['E'] = {{128, 0, 16, 16}, {144, 0, 16, 16}, {128, 64, 16, 16}, {144, 64, 16, 16}}, 
    ['F'] = {{160, 0, 16, 16}, {176, 0, 16, 16}, {160, 64, 16, 16}, {176, 64, 16, 16}}, 
    ['G'] = {{192, 0, 16, 16}, {208, 0, 16, 16}, {192, 64, 16, 16}, {208, 64, 16, 16}}, 
    ['H'] = {{224, 0, 16, 16}, {240, 0, 16, 16}, {224, 64, 16, 16}, {240, 64, 16, 16}}, 
    ['I'] = {{0, 16, 16, 16}, {16, 16, 16, 16}, {0, 80, 16, 16}, {16, 80, 16, 16}}, 
    ['J'] = {{32, 16, 16, 16}, {48, 16, 16, 16}, {32, 80, 16, 16}, {48, 80, 16, 16}}, 
    ['K'] = {{64, 16, 16, 16}, {80, 16, 16, 16}, {64, 80, 16, 16}, {80, 80, 16, 16}}, 
    ['L'] = {{96, 16, 16, 16}, {112, 16, 16, 16}, {96, 80, 16, 16}, {112, 80, 16, 16}}, 
    ['M'] = {{128, 16, 16, 16}, {144, 16, 16, 16}, {128, 80, 16, 16}, {144, 80, 16, 16}}, 
    ['N'] = {{160, 16, 16, 16}, {176, 16, 16, 16}, {160, 80, 16, 16}, {176, 80, 16, 16}}, 
    ['O'] = {{192, 16, 16, 16}, {208, 16, 16, 16}, {192, 80, 16, 16}, {208, 80, 16, 16}}, 
    ['P'] = {{224, 16, 16, 16}, {240, 16, 16, 16}, {224, 80, 16, 16}, {240, 80, 16, 16}}, 
    ['Q'] = {{0, 32, 16, 16}, {16, 32, 16, 16}, {0, 96, 16, 16}, {16, 96, 16, 16}}, 
    ['R'] = {{32, 32, 16, 16}, {48, 32, 16, 16}, {32, 96, 16, 16}, {48, 96, 16, 16}}, 
    ['S'] = {{64, 32, 16, 16}, {80, 32, 16, 16}, {64, 96, 16, 16}, {80, 96, 16, 16}}, 
    ['T'] = {{96, 32, 16, 16}, {112, 32, 16, 16}, {96, 96, 16, 16}, {112, 96, 16, 16}}, 
    ['U'] = {{128, 32, 16, 16}, {144, 32, 16, 16}, {128, 96, 16, 16}, {144, 96, 16, 16}}, 
    ['V'] = {{160, 32, 16, 16}, {176, 32, 16, 16}, {160, 96, 16, 16}, {176, 96, 16, 16}}, 
    ['W'] = {{192, 32, 16, 16}, {208, 32, 16, 16}, {192, 96, 16, 16}, {208, 96, 16, 16}}, 
    ['X'] = {{224, 32, 16, 16}, {240, 32, 16, 16}, {224, 96, 16, 16}, {240, 96, 16, 16}}, 
    ['Y'] = {{0, 48, 16, 16}, {16, 48, 16, 16}, {0, 112, 16, 16}, {16, 112, 16, 16}}, 
    ['Z'] = {{32, 48, 16, 16}, {48, 48, 16, 16}, {32, 112, 16, 16}, {48, 112, 16, 16}}, 

	// Lowercase letters
    ['a'] = {{0, 0, 16, 16}, {16, 0, 16, 16}, {0, 64, 16, 16}, {16, 64, 16, 16}}, 
    ['b'] = {{32, 0, 16, 16}, {48, 0, 16, 16}, {32, 64, 16, 16}, {48, 64, 16, 16}}, 
    ['c'] = {{64, 0, 16, 16}, {80, 0, 16, 16}, {64, 64, 16, 16}, {80, 64, 16, 16}}, 
    ['d'] = {{96, 0, 16, 16}, {112, 0, 16, 16}, {96, 64, 16, 16}, {112, 64, 16, 16}}, 
    ['e'] = {{128, 0, 16, 16}, {144, 0, 16, 16}, {128, 64, 16, 16}, {144, 64, 16, 16}}, 
    ['f'] = {{160, 0, 16, 16}, {176, 0, 16, 16}, {160, 64, 16, 16}, {176, 64, 16, 16}}, 
    ['g'] = {{192, 0, 16, 16}, {208, 0, 16, 16}, {192, 64, 16, 16}, {208, 64, 16, 16}}, 
    ['h'] = {{224, 0, 16, 16}, {240, 0, 16, 16}, {224, 64, 16, 16}, {240, 64, 16, 16}}, 
    ['i'] = {{0, 16, 16, 16}, {16, 16, 16, 16}, {0, 80, 16, 16}, {16, 80, 16, 16}}, 
    ['j'] = {{32, 16, 16, 16}, {48, 16, 16, 16}, {32, 80, 16, 16}, {48, 80, 16, 16}}, 
    ['k'] = {{64, 16, 16, 16}, {80, 16, 16, 16}, {64, 80, 16, 16}, {80, 80, 16, 16}}, 
    ['l'] = {{96, 16, 16, 16}, {112, 16, 16, 16}, {96, 80, 16, 16}, {112, 80, 16, 16}}, 
    ['m'] = {{128, 16, 16, 16}, {144, 16, 16, 16}, {128, 80, 16, 16}, {144, 80, 16, 16}}, 
    ['n'] = {{160, 16, 16, 16}, {176, 16, 16, 16}, {160, 80, 16, 16}, {176, 80, 16, 16}}, 
    ['o'] = {{192, 16, 16, 16}, {208, 16, 16, 16}, {192, 80, 16, 16}, {208, 80, 16, 16}}, 
    ['p'] = {{224, 16, 16, 16}, {240, 16, 16, 16}, {224, 80, 16, 16}, {240, 80, 16, 16}}, 
    ['q'] = {{0, 32, 16, 16}, {16, 32, 16, 16}, {0, 96, 16, 16}, {16, 96, 16, 16}}, 
    ['r'] = {{32, 32, 16, 16}, {48, 32, 16, 16}, {32, 96, 16, 16}, {48, 96, 16, 16}}, 
    ['s'] = {{64, 32, 16, 16}, {80, 32, 16, 16}, {64, 96, 16, 16}, {80, 96, 16, 16}}, 
    ['t'] = {{96, 32, 16, 16}, {112, 32, 16, 16}, {96, 96, 16, 16}, {112, 96, 16, 16}}, 
    ['u'] = {{128, 32, 16, 16}, {144, 32, 16, 16}, {128, 96, 16, 16}, {144, 96, 16, 16}}, 
    ['v'] = {{160, 32, 16, 16}, {176, 32, 16, 16}, {160, 96, 16, 16}, {176, 96, 16, 16}}, 
    ['w'] = {{192, 32, 16, 16}, {208, 32, 16, 16}, {192, 96, 16, 16}, {208, 96, 16, 16}}, 
    ['x'] = {{224, 32, 16, 16}, {240, 32, 16, 16}, {224, 96, 16, 16}, {240, 96, 16, 16}}, 
    ['y'] = {{0, 48, 16, 16}, {16, 48, 16, 16}, {0, 112, 16, 16}, {16, 112, 16, 16}}, 
    ['z'] = {{32, 48, 16, 16}, {48, 48, 16, 16}, {32, 112, 16, 16}, {48, 112, 16, 16}}, 
	
    // Numbers
    ['0'] = {{80, 176, 16, 16}, {96, 176, 16, 16}, {80, 208, 16, 16}, {96, 208, 16, 16}}, 
    ['1'] = {{112, 176, 16, 16}, {128, 176, 16, 16}, {122, 208, 16, 16}, {128, 208, 16, 16}}, 
    ['2'] = {{144, 176, 16, 16}, {160, 176, 16, 16}, {144, 208, 16, 16}, {160, 208, 16, 16}}, 
    ['3'] = {{176, 176, 16, 16}, {192, 176, 16, 16}, {176, 208, 16, 16}, {192, 208, 16, 16}}, 
    ['4'] = {{208, 176, 16, 16}, {224, 176, 16, 16}, {208, 208, 16, 16}, {224, 208, 16, 16}}, 
    ['5'] = {{80, 192, 16, 16}, {96, 192, 16, 16}, {80, 224, 16, 16}, {96, 224, 16, 16}}, 
    ['6'] = {{112, 192, 16, 16}, {160, 192, 16, 16}, {122, 224, 16, 16}, {122, 224, 16, 16}}, 
    ['7'] = {{144, 192, 16, 16}, {160, 192, 16, 16}, {144, 224, 16, 16}, {144, 224, 16, 16}}, 
    ['8'] = {{176, 192, 16, 16}, {192, 192, 16, 16}, {176, 224, 16, 16}, {176, 224, 16, 16}}, 
    ['9'] = {{208, 192, 16, 16}, {224, 192, 16, 16}, {208, 224, 16, 16}, {224, 224, 16, 16}}, 

    // Special characters
    ['!'] = {{32, 240, 16, 16}, {48, 240, 16, 16}, {160, 240, 16, 16}, {176, 240, 16, 16}}, 
    ['&'] = {{0, 240, 16, 16}, {16, 240, 16, 16}, {128, 240, 16, 16}, {144, 240, 16, 16}}, 
    ['('] = {{64, 240, 16, 16}, {80, 240, 16, 16}, {192, 240, 16, 16}, {208, 240, 16, 16}}, 
    [')'] = {{96, 240, 16, 16}, {112, 240, 16, 16}, {224, 240, 16, 16}, {240, 240, 16, 16}}, 
    ['+'] = {{240, 112, 16, 16}, {240, 128, 16, 16}, {240, 176, 16, 16}, {240, 192, 16, 16}}, 
    ['-'] = {{240, 144, 16, 16}, {240, 160, 16, 16}, {240, 208, 16, 16}, {240, 224, 16, 16}}, 
    ['>'] = {{96, 48, 16, 16}, {112, 48, 16, 16}, {64, 112, 16, 16}, {80, 112, 16, 16}}, 
    ['<'] = {{64, 48, 16, 16}, {80, 48, 16, 16}, {96, 112, 16, 16}, {112, 112, 16, 16}}, 
    ['.'] = {{128, 48, 16, 16}, {144, 48, 16, 16}, {128, 112, 16, 16}, {144, 112, 16, 16}}, 
    ['\''] = {{160, 48, 16, 16}, {176, 48, 16, 16}, {160, 112, 16, 16}, {176, 112, 16, 16}}, 
    ['*'] = {{208, 112, 16, 16}, {208, 112, 16, 16}, {224, 144, 16, 16}, {224, 144, 16, 16}}, 
    ['?'] = {{208, 128, 16, 16}, {208, 128, 16, 16}, {224, 160, 16, 16}, {224, 160, 16, 16}},
	[' '] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
    // Add more mappings as needed
};

void Font_Bold_DrawCol(struct FontData *this, const char *text, s32 x, s32 y, FontAlign align, u8 r, u8 g, u8 b)
{
    // Offset position based off alignment
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

    // Get animation offsets
    u8 v1 = (animf_count >> 1) & 1;
	u8 v2 = ((animf_count >> 1) & 1) + 2;

    // Flag to track if there is at least one lowercase letter
    int has_lowercase = 0;

    // Check for lowercase letters in the text
    const char *ptr = text;
    while (*ptr != '\0') {
        if (*ptr >= 'a' && *ptr <= 'z') {
            has_lowercase = 1;
            break;
        }
        ptr++;
    }

    // Draw string character by character
    u8 c;
    while ((c = *text++) != '\0')
    {
        if (c == ' ') {
            x += 16; // Adjust spacing for space character
            continue;
        }

        if (c >= 128 || char_map[c][0].width == 0) continue; // Skip if character is out of range or not defined

        // Determine frame index based on whether there are lowercase letters
        u8 frame_index;
        if (has_lowercase) {
            // Use 3rd and 4th frames for numbers and special characters
            frame_index = (c >= '0' && c <= '9') ? v2 : v2;
        } else {
            // Use default frame index
            frame_index = v1;
        }

        // Draw character with animation
        CharRect *rect = &char_map[c][frame_index];
        RECT src = {rect->x, rect->y, rect->width, rect->height};
        Gfx_BlitTexCol(&this->tex, &src, x, y, r, g, b);

        x += 13; // Adjust spacing if needed
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