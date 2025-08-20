/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_STRPLAY_H
#define PSXF_GUARD_STRPLAY_H

#include "psx.h"
#include <libpress.h>

void Str_Init(void);
void Str_PlayFile(CdlFILE* file);
void Str_Play(const char *filedir);
void Str_CanPlayDef(void);

// Movie progress and frame information functions
int Str_GetTotalFrames(void);
int Str_GetCurrentFrame(void);
int Str_GetProgressPercent(void);

// Global variables for easy access to movie frame information
extern int movie_total_frames;
extern int movie_current_frame;
extern int movie_progress_percent;
extern int movie_manual_frame_count;

extern boolean movie_is_playing;
#endif