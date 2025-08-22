/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "eyes.h"

#include "../io.h"
#include "../stage.h"
#include "../timer.h"
#include "../mutil.h"

// Eyes functions
void Eyes_Init(Eyes *this)
{
    // Load eyes graphics
    Gfx_LoadTex(&this->tex, IO_Read("\\GCHAR\\EYES.TIM;1"), GFX_LOADTEX_FREE);
}

void Eyes_Tick(Eyes *this, fixed_t x, fixed_t y)
{
    fixed_t fx;
    static fixed_t eye_offset_x = FIXED_DEC(0, 1);
    
    // Get camera position for smooth eye movement
    fx = stage.camera.x;
    
    // Calculate target eye movement based on camera
    // Move eyes 8 pixels left when camera goes negative, right when positive
    fixed_t target_offset = FIXED_DEC(0, 1);
    if (fx < 0) {
        // Camera moving left - eyes target left
        target_offset = FIXED_DEC(-8, 1);
    } else if (fx > 0) {
        // Camera moving right - eyes target right
        target_offset = FIXED_DEC(8, 1);
    }
    
    // Use mutil lerp for smooth movement
    eye_offset_x = lerp(eye_offset_x, target_offset, FIXED_DEC(1, 8));

    // Define source and destination rectangles for drawing
    RECT eyes_src = {0, 0, 64, 32};
    RECT_FIXED eyes_dst = {
        x + FIXED_DEC(0, 1) + eye_offset_x,
        y + FIXED_DEC(-51, 1),
        FIXED_DEC(64, 1),
        FIXED_DEC(32, 1)
    };

    // Debug and draw textures
    Stage_DrawTex(&this->tex, &eyes_src, &eyes_dst, stage.camera.bzoom, stage.camera.angle);
}
