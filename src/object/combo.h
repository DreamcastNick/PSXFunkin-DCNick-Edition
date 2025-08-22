/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_COMBO_H
#define PSXF_GUARD_COMBO_H

#include "../object.h"
#include "../stage.h"

//Combo object structure
typedef struct
{
	//Object base structure
	Object obj;
	
	//Combo state
	u8 hit_type; //SICK, GOOD, BAD, SHIT
	
	fixed_t x;
	
	fixed_t hy, hv; //Hit type
	fixed_t ht;
	
	fixed_t cy, cv; //COMBO
	fixed_t ct;
	
	// Support up to 10 digits (hundreds of millions and beyond)
	u8 num[10]; //0-9 per digit; 0xFF means unused
	fixed_t numy[10];
	fixed_t numv[10];
	fixed_t numt;
	u8 num_count; //active digit count
} Obj_Combo;

//Combo object functions
Obj_Combo *Obj_Combo_New(fixed_t x, fixed_t y, u8 hit_type, u32 combo);

#endif
