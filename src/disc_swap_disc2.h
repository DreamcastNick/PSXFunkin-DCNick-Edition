#ifndef PSXF_GUARD_DISC_SWAP_DISC2_H
#define PSXF_GUARD_DISC_SWAP_DISC2_H

extern int currentDisc;

// CD-ROM Mode structure
typedef struct {
    unsigned long size;
    unsigned long bufaddr;
    unsigned char flag;
} CdRMode2;

// FileNode structure for linked list of files
typedef struct FileNode2 {
    char name[128];
    struct FileNode2 *next;
} FileNode2;

// Global pointer to the head of the file list
extern FileNode2 *fileList2;

// CD-ROM directory mode constants
#define CdMdirNormal 0
#define CdMdirAll 1

// CD-ROM disk status constants
#define CdlDiskReady 0
#define CdlNoDisk 1
#define CdlTrayOpen 2
#define CdlTrayClose 3
#define CdlReading 4
#define CdlResetting 5
#define CdlError 6
#define CdlIdent 7
#define CdlNoBios 8
#define CdlInvalid 9
#define CdlOpen 10
#define CdlShellOpen 11
#define CdlRetry 12
#define CdlFail 13
#define CdlDiskChanged 14
#define CdlNoDiskSwapped 15
#define CdlDiskOK 16

void HandleDiscSwap(void);
int UserPressedButton(void);
int CdSwitchDisc(void);
int CheckDiscIndicator(void);
int CdTrayReq(int req, void* param);
void CheckCurrentDisc(void);
void DisplayMessage(const char* message);
void CheckDiscSwap(void);
void addFileToList(const char *filename);
void traverseDirectory(const char *path);
void listAllFiles(void);
void loadFile(const char *filename);
void loadAllFiles(void);
void freeFileList(void);

#endif // PSXF_GUARD_DISC_SWAP_H
