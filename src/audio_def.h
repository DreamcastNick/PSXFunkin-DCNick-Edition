#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File1 file;
	u32 length;
} XA_TrackDef1;

typedef struct
{
	XA_File2 file;
	u32 length;
} XA_TrackDef2;

typedef struct
{
	XA_File3 file;
	u32 length;
} XA_TrackDef3;

static const XA_TrackDef1 xa_tracks_disc1[] = {
	//MENU.XA
	{XA_Menu1, XA_LENGTH(11300)}, //XA_GettinFreaky_Disc1
	{XA_Menu1, XA_LENGTH(3800)},  //XA_GameOver_Disc1
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(7700)}, //XA_Bopeebo
	{XA_Week1A, XA_LENGTH(8000)}, //XA_Fresh
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(8700)}, //XA_Dadbattle
	{XA_Week1B, XA_LENGTH(6800)}, //XA_Tutorial
	//WEEK2A.XA
	{XA_Week2A, XA_LENGTH(9900)}, //XA_Spookeez
	{XA_Week2A, XA_LENGTH(8900)}, //XA_South
	//WEEK2B.XA
	{XA_Week2B, XA_LENGTH(17800)}, //XA_Monster
	//WEEK3A.XA
	{XA_Week3A, XA_LENGTH(8400)},  //XA_Pico
	{XA_Week3A, XA_LENGTH(10000)}, //XA_Philly
	//WEEK3B.XA
	{XA_Week3B, XA_LENGTH(10700)}, //XA_Blammed
};

static const XA_TrackDef2 xa_tracks_disc2[] = {
	//MENU.XA
	{XA_Menu2, XA_LENGTH(11300)}, //XA_GettinFreaky_Disc2
	{XA_Menu2, XA_LENGTH(3800)},  //XA_GameOver_Disc2
	//MOD1A.XA
	{XA_MOD1A, XA_LENGTH(11500)},  //XA_Where_Are_You
	{XA_MOD1A, XA_LENGTH(15100)}, //XA_Eruption
	//MOD1B.XA
	{XA_MOD1B, XA_LENGTH(14800)}, //XA_Kaioken
	{XA_MOD1B, XA_LENGTH(92300)}, //XA_Ferocious
	//MOD1C.XA
	{XA_MOD1C, XA_LENGTH(25800)}, //XA_Monochrome
	{XA_MOD1C, XA_LENGTH(50700)}, //XA_TripleTrouble
	//MOD1D.XA
	{XA_MOD1D, XA_LENGTH(63600)}, //XA_Unbeatable
};

static const XA_TrackDef3 xa_tracks_disc3[] = {
	//MENU.XA
	{XA_Menu3, XA_LENGTH(11300)}, //XA_GettinFreaky_Disc3
	{XA_Menu3, XA_LENGTH(3800)},  //XA_GameOver_Disc3
	//AETHOS1.XA
	{XA_Aethos1, XA_LENGTH(32200)}, //XA_Aethos
	{XA_Aethos1, XA_LENGTH(21600)}, //XA_RottenSmoothie
	//AETHOS2.XA
	{XA_Aethos2, XA_LENGTH(22500)}, //XA_TwiddleFinger
	{XA_Aethos2, XA_LENGTH(35600)}, //XA_CrimsonAwakening
	//AETHOS3.XA
	{XA_Aethos3, XA_LENGTH(33000)}, //XA_WellDone
	{XA_Aethos3, XA_LENGTH(27900)}, //XA_HateBoner
	//BILLY.XA
	{XA_Billy, XA_LENGTH(37500)}, //XA_SillyBilly
};

static const char *xa_paths_disc1[] = {
	"\\MUSIC\\MENU1.XA;1",   //XA_Menu1
	"\\MUSIC\\WEEK1A.XA;1", //XA_Week1A
	"\\MUSIC\\WEEK1B.XA;1", //XA_Week1B
	"\\MUSIC\\WEEK2A.XA;1", //XA_Week2A
	"\\MUSIC\\WEEK2B.XA;1", //XA_Week2B
	"\\MUSIC\\WEEK3A.XA;1", //XA_Week3A
	"\\MUSIC\\WEEK3B.XA;1", //XA_Week3B
	NULL,
};

static const char *xa_paths_disc2[] = {
	"\\MUSIC\\MENU2.XA;1",   //XA_Menu2
	"\\MUSIC\\MOD1A.XA;1", //XA_Mod1A
	"\\MUSIC\\MOD1B.XA;1", //XA_Mod1B
	"\\MUSIC\\MOD1C.XA;1", //XA_Mod1C
	"\\MUSIC\\MOD1D.XA;1", //XA_Mod1D
	NULL,
};

static const char *xa_paths_disc3[] = {
	"\\MUSIC\\MENU3.XA;1",   //XA_Menu3
	"\\MUSIC\\AETHOS1.XA;1", //XA_Aethos1
	"\\MUSIC\\AETHOS2.XA;1", //XA_Aethos2
	"\\MUSIC\\AETHOS3.XA;1", //XA_Aethos3
	"\\MUSIC\\BILLY.XA;1",   //XA_Billy
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3_1;

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3_2;

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3_3;

static const XA_Mp3_1 xa_mp3s1[] = {
	//MENU1.XA
	{"freaky", false},   //XA_GettinFreaky_Disc1
	{"gameover", false}, //XA_GameOver_Disc1
	//WEEK1A.XA
	{"bopeebo", true}, //XA_Bopeebo
	{"fresh", true},   //XA_Fresh
	//WEEK1B.XA
	{"dadbattle", true}, //XA_Dadbattle
	{"tutorial", true}, //XA_Tutorial
	//WEEK2A.XA
	{"spookeez", true}, //XA_Spookeez
	{"south", true},    //XA_South
	//WEEK2B.XA
	{"monster", true}, //XA_Monster
	//WEEK3A.XA
	{"pico", true},   //XA_Pico
	{"philly", true}, //XA_Philly
	//WEEK3B.XA
	{"blammed", true}, //XA_Blammed
	
	{NULL, false}
};

static const XA_Mp3_2 xa_mp3s2[] = {
	//MENU2.XA
	{"freaky", false},   //XA_GettinFreaky_Disc2
	{"gameover", false}, //XA_GameOver_Disc2
	//MOD1A.XA
	{"where-are-you", true},   //XA_Where_Are_You
	{"eruption", true}, //XA_Eruption
	//MOD1B.XA
	{"kaioken", true}, //XA_Kaioken
	{"ferocious", true}, //XA_Ferocious
	//MOD1C.XA
	{"monochrome", true}, //XA_Monochrome
	{"triple-trouble", true}, //XA_TripleTrouble
	//MOD1D.XA
	{"unbeatable", true}, //XA_Unbeatable
	
	{NULL, false}
};

static const XA_Mp3_3 xa_mp3s3[] = {
	//MENU3.XA
	{"freaky", false},   //XA_GettinFreaky_Disc3
	{"gameover", false}, //XA_GameOver_Disc3
	//AETHOS1.XA
	{"aethos", true}, //XA_Aethos
	{"rotten-smoothie", true}, //XA_RottenSmoothie
	//AETHOS2.XA
	{"twiddlefinger", true}, //XA_TwiddleFinger
	{"crimson-awakening", true}, //XA_CrimsonAwakening
	//AETHOS3.XA
	{"well-done", true}, //XA_WellDone
	{"hate-boner", true}, //XA_HateBoner
	//BILLY.XA
	{"silly-billy", true}, //XA_SillyBilly
	
	{NULL, false}
};