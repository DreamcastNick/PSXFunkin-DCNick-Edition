#ifndef PSXF_GUARD_EVENT_H
#define PSXF_GUARD_EVENT_H

#include "io.h"
#include "gfx.h"
#include "pad.h"

#include "fixed.h"
#include "character.h"
#include "player.h"
#include "object.h"
#include "font.h"

typedef struct
{
	fixed_t zoom, speed;
	boolean hidehud;
	
	fixed_t shake;
	
	u16 flash, whiteflash, fade;
	boolean fadebool;
	boolean fadebehind;
	
	boolean movieview;
} Event;

extern Event event;

void FollowCharCamera();
void Load_Events();
void Events_Front();
void Events();
void Events_Back();

#endif