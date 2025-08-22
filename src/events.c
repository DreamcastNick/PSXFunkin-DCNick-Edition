/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "events.h"
#include "stage.h"
#include "timer.h"
#include "random.h"
#include "mutil.h"

Events event_speed;

static void Events_Check(ChartEvent* event)
{
    switch (event->event & EVENTS_FLAG_VARIANT)
    {
        case EVENTS_FLAG_SPEED:
            event_speed.value1 = (fixed_t)event->value1;
            event_speed.value2 = (fixed_t)event->value2;
            break;
        case EVENTS_FLAG_GF:
            stage.gf_speed = (u8)((event->value1 >> FIXED_SHIFT) * 4);
            break;
        case EVENTS_FLAG_CAMZOOM:
            stage.bump += (fixed_t)event->value1;
            stage.sbump += (fixed_t)event->value2;
            break;
        default:
            break;
    }
}

void Events_Tick(void)
{
    stage.speed += (FIXED_MUL(stage.ogspeed, event_speed.value1) - stage.speed) / (((event_speed.value2 / 60) + 1));
}

void Events_StartEvents(void)
{
    for (ChartEvent *event = stage.cur_event; event != NULL && event->pos != 0xFFFFFFFF; event++)
    {
        if (event->pos > (u32)(stage.note_scroll >> FIXED_SHIFT))
            break;
        else
            stage.cur_event++;

        if (event->event & EVENTS_FLAG_PLAYED)
            continue;

        Events_Check(event);
        event->event |= EVENTS_FLAG_PLAYED;
    }

    for (ChartEvent *event = stage.event_cur_event; event != NULL && event->pos != 0xFFFFFFFF; event++)
    {
        if (event->pos > (u32)(stage.note_scroll >> FIXED_SHIFT))
            break;
        else
            stage.event_cur_event++;

        if (event->event & EVENTS_FLAG_PLAYED)
            continue;

        Events_Check(event);
        event->event |= EVENTS_FLAG_PLAYED;
    }

    Events_Tick();
}

void Events_Load(void)
{
    event_speed.value1 = FIXED_UNIT;
    event_speed.value2 = 0;
}


