/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef PSXF_GUARD_AUDIO_H
#define PSXF_GUARD_AUDIO_H

#include "psx.h"

//XA enumerations
typedef enum
{
	XA_Menu1,   //MENU1.XA
	XA_Week1A, //WEEK1A.XA
	XA_Week1B, //WEEK1B.XA
	XA_Week2A, //WEEK2A.XA
	XA_Week2B, //WEEK2B.XA
	XA_Week3A, //WEEK3A.XA
	XA_Week3B, //WEEK3B.XA
	
	XA_Max1,
} XA_File1;

typedef enum
{
	XA_Menu2,   //MENU2.XA
	XA_MOD1A, //MOD1A.XA
	XA_MOD1B, //MOD1B.XA
	XA_MOD1C, //MOD1C.XA
	XA_MOD1D, //MOD1D.XA
	
	XA_Max2,
} XA_File2;

typedef enum
{
	XA_Menu3,   //MENU3.XA
	XA_Aethos1,	   //AETHOS1.XA
	XA_Aethos2,	   //AETHOS2.XA
	XA_Aethos3,	   //AETHOS3.XA
	
	XA_Max3,
} XA_File3;

typedef enum
{
	//MENU.XA
	XA_GettinFreaky_Disc1, //Gettin' Freaky
	XA_GameOver_Disc1,     //Game Over
	//WEEK1A.XA
	XA_Bopeebo, //Bopeebo
	XA_Fresh,   //Fresh
	//WEEK1B.XA
	XA_Dadbattle, //DadBattle
	XA_Tutorial,  //Tutorial
	//WEEK2A.XA
	XA_Spookeez, //Spookeez
	XA_South,    //South
	//WEEK2B.XA
	XA_Monster, //Monster
	//WEEK3A.XA
	XA_Pico,   //Pico
	XA_Philly, //Philly
	//WEEK3B.XA
	XA_Blammed, //Blammed
	
	XA_TrackMax1,
} XA_Track1;

typedef enum
{
	//MENU.XA
	XA_GettinFreaky_Disc2, //Gettin' Freaky
	XA_GameOver_Disc2,     //Game Over
	//MOD1A.XA
	XA_Where_Are_You,   //Where Are You
	XA_Eruption, //Eruption
	//MOD1B.XA
	XA_Kaioken, //Kaioken
	XA_Ferocious, //Ferocious
	//MOD1C.XA
	XA_Monochrome, //Monochrome
	XA_TripleTrouble, //Triple Trouble
	//MOD1D.XA
	XA_Unbeatable, //Unbeatable
	
	XA_TrackMax2,
} XA_Track2;

typedef enum
{
	//MENU.XA
	XA_GettinFreaky_Disc3, //Gettin' Freaky
	XA_GameOver_Disc3,     //Game Over
	//AETHOS1.XA
	XA_Aethos, //Aethos
	XA_RottenSmoothie, //Rotten Smoothie
	//AETHOS2.XA
	XA_Twiddlefinger, //Twiddlefinger
	XA_CrimsonAwakening, //Crimson Awakening
	//AETHOS3.XA
	XA_WellDone, //Well Done
	XA_HateBoner, //Hate Boner
	
	XA_TrackMax3,
} XA_Track3;

//Audio functions
u32 Audio_GetLengthDisc1(XA_Track1 lengthtrack);
u32 Audio_GetLengthDisc2(XA_Track2 lengthtrack);
u32 Audio_GetLengthDisc3(XA_Track3 lengthtrack);
void Audio_Init(void);
void Audio_Quit(void);
void Audio_Reset(void);
void Audio_PlayXA_TrackDisc1(XA_Track1 track, u8 volume, u8 channel, boolean loop, s32 start_position_seconds);
void Audio_SeekXA_TrackDisc1(XA_Track1 track, s16 start_position_seconds);
void Audio_PlayXA_TrackDisc2(XA_Track2 track, u8 volume, u8 channel, boolean loop, s32 start_position_seconds);
void Audio_SeekXA_TrackDisc2(XA_Track2 track, s16 start_position_seconds);
void Audio_PlayXA_TrackDisc3(XA_Track3 track, u8 volume, u8 channel, boolean loop, s32 start_position_seconds);
void Audio_SeekXA_TrackDisc3(XA_Track3 track, s16 start_position_seconds);
void Audio_SetPos(s32 time);
void Audio_PauseXA(void);
void Audio_ResumeXA(void);
void Audio_StopXA(void);
void Audio_ChannelXA(u8 channel);
s32 Audio_TellXA_Sector(void);
s32 Audio_TellXA_Milli(void);
boolean Audio_PlayingXA(void);
void Audio_WaitPlayXA(void);
void Audio_ProcessXA(void);
void findFreeChannel(void);
void Audio_StartAt(u16 music_start_position_seconds);
u32 Audio_LoadVAGData(u32 *sound, u32 sound_size);
void AudioPlayVAG(int channel, u32 addr);
void Audio_PlaySoundOnChannel(u32 addr, u32 channel, int volume);
void Audio_PlaySound(u32 addr, int volume);
u32 VAG_IsPlaying(u32 channel);
void Audio_ClearAlloc(void);

#endif