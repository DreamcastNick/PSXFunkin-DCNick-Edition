#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11300)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3800)},  //XA_GameOver
	//WEEK1A.XA
	{XA_Week1A, XA_LENGTH(67600)}, //XA_LittleMan
	{XA_Week1A, XA_LENGTH(123800)}, //XA_LittleManTwo
	//WEEK1B.XA
	{XA_Week1B, XA_LENGTH(900)}, //XA_BigMan
	{XA_Week1B, XA_LENGTH(45400)}, //XA_Madness
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\WEEK1A.XA;1", //XA_Week1A
	"\\MUSIC\\WEEK1B.XA;1", //XA_Week1B
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},  		//XA_GettinFreaky
	{"gameover", false}, 		//XA_GameOver
	//WEEK1A.XA
	{"little-man", true}, 		//XA_Bopeebo
	{"little-man-two", true},   //XA_Fresh
	//WEEK1B.XA
	{"big-man", true}, //XA_BigMan
	{"madness", true}, //XA_Madness
	
	{NULL, false}
};
