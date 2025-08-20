/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_EYES_H
#define PSXF_GUARD_EYES_H

#include "../gfx.h"
#include "../fixed.h"

//Eyes structure
typedef struct
{
	//Eyes state
	Gfx_Tex tex;
	fixed_t bump;
} Eyes;

//Eyes functions
void Eyes_Init(Eyes *this);
void Eyes_Tick(Eyes *this, fixed_t x, fixed_t y);

#endif
