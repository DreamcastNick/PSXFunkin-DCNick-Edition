#ifndef STAGE_SWAP_CHARS_ONLY
#define STAGE_SWAP_CHARS_ONLY   (STAGE_LOAD_PLAYER | STAGE_LOAD_PLAYER2 | STAGE_LOAD_OPPONENT | STAGE_LOAD_OPPONENT2 | STAGE_LOAD_GIRLFRIEND)
#define STAGE_SWAP_STAGE_ONLY   (STAGE_LOAD_STAGE)
#define STAGE_SWAP_ALL          (STAGE_SWAP_CHARS_ONLY | STAGE_SWAP_STAGE_ONLY)
#endif
	{ //StageId_4_1 (Where Are You)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(22,10),FIXED_DEC(22,10),FIXED_DEC(22,10)},
		4, 1,
		XA_Where_Are_You, 0,
		false,
		false,
		
		StageId_4_2, 0
	},
	{ //StageId_4_2 (Eruption)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(23,10),FIXED_DEC(23,10),FIXED_DEC(23,10)},
		4, 2,
		XA_Eruption, 2,
		false,
		false,
		
		StageId_4_3, 0
	},
	{ //StageId_4_3 (Kaio-Ken)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(28,10),FIXED_DEC(25,10),FIXED_DEC(25,10)},
		4, 3,
		XA_Kaioken, 0,
		false,
		false,
		
		StageId_4_3, 0
	},
	{ //StageId_4_4 (Ferocious)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(35,10),FIXED_DEC(35,10),FIXED_DEC(35,10)},
		4, 4,
		XA_Ferocious, 2,
		false,
		true,
		
		StageId_4_4, 0
	},
	{ //StageId_4_5 (Monochrome)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(29,10),FIXED_DEC(29,10),FIXED_DEC(29,10)},
		4, 5,
		XA_Monochrome, 0,
		false,
		false,
		
		StageId_4_5, 0
	},
	{ //StageId_4_6 (Triple Trouble)
		//Characters
		{Char_BF_New,     FIXED_DEC(60,1),   FIXED_DEC(100,1)},
		{NULL},
		{Char_ExeP3_New, FIXED_DEC(-120,1),   FIXED_DEC(100,1)},
		{NULL},
		{NULL},
		
		//Stage background
		Back_Trio_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(32,10),FIXED_DEC(32,10),FIXED_DEC(32,10)},
		4, 6,
		XA_TripleTrouble, 2,
		true,
		true,
		
		StageId_4_6, 0
	},
	{ //StageId_4_7 (Unbeatable)
		//Characters
		{Char_BF_New,    FIXED_DEC(60,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_Dad_New, FIXED_DEC(-120,1),  FIXED_DEC(100,1)},
		{NULL},
		{Char_GF_New,     FIXED_DEC(0,1),  FIXED_DEC(-10,1)},
		
		//Stage background
		Back_Dummy_New,
		
		//Camera Offset (X, Y, Scale)
		FIXED_DEC(0,1), FIXED_DEC(0,1), FIXED_DEC(1,1),
		
		//Song info
		{FIXED_DEC(3,1),FIXED_DEC(3,1),FIXED_DEC(3,1)},
		4, 7,
		XA_Unbeatable, 0,
		false,
		false,
		
		StageId_4_7, 0
	},