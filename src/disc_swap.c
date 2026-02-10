#include "disc_swap.h"

#include "audio.h"
#include "pad.h"
#include "io.h"
#include "psx.h"

int currentDisc = 1;

void DisplayMessage(const char* message) {
    FntPrint(message);
    FntFlush(-1);
    Gfx_Flip();
    printf("%s\n", message);
}

int UserPressedButton(void) {
    return (pad_state.press & PAD_SELECT) || (pad_state.press & PAD_START) || (pad_state.press & PAD_CROSS);
}

int CheckDiscIndicator(void) {
    CdlFILE file;
    if (IO_FindFile(&file, "\\DISC1.ID;1")) return 1;
    if (IO_FindFile(&file, "\\DISC2.ID;1")) return 1;
    if (IO_FindFile(&file, "\\DISC3.ID;1")) return 1;
    if (IO_FindFile(&file, "\\DISC4.ID;1")) return 1;
    return 0;
}

void CheckCurrentDisc(void) {
    CdlFILE file;
    if (IO_FindFile(&file, "\\DISC4.ID;1")) { currentDisc = 4; return; }
    if (IO_FindFile(&file, "\\DISC3.ID;1")) { currentDisc = 3; return; }
    if (IO_FindFile(&file, "\\DISC2.ID;1")) { currentDisc = 2; return; }
    if (IO_FindFile(&file, "\\DISC1.ID;1")) { currentDisc = 1; return; }
}

int CdTrayReq(int req, void* param) {
    (void)param;
    switch (req) {
        case CdlTrayOpen:
            CdControlF(CdlTrayOpen, NULL);
            break;
        case CdlTrayClose:
            CdControlF(CdlTrayClose, NULL);
            break;
        default:
            return -1;
    }
    return 0;
}

int CdSwitchDisc(void) {
    int timeout = 0;
    while (CdDiskReady(0) != CdlDiskReady) {
        CdControlF(CdlNop, NULL);
        if (++timeout > 10000) return -1;
    }

    if (!CheckDiscIndicator())
        return -3;

    if (CdDiskReady(0) == CdlDiskReady) {
        if (CdStop() == 0) return -4;
        if (CdTrayReq(CdlTrayOpen, NULL) == 0) return -4;

        timeout = 0;
        while (CdDiskReady(0) != CdlTrayOpen) {
            CdControlF(CdlNop, NULL);
            if (++timeout > 10000) return -1;
        }

        if (CdTrayReq(CdlTrayClose, NULL) == 0) return -5;

        timeout = 0;
        while (CdDiskReady(0) != CdlDiskReady) {
            CdControlF(CdlNop, NULL);
            if (++timeout > 10000) return -1;
        }
    }

    return 0;
}

void HandleDiscSwap(void) {
    DisplayMessage("Attempting to switch the disc...");
    int switchResult = CdSwitchDisc();
    if (switchResult == 0) {
        DisplayMessage("Verifying new disc...");
        if (CheckDiscIndicator()) {
            CheckCurrentDisc();
            DisplayMessage("New disc loaded successfully.");
        } else {
            DisplayMessage("New disc is not valid.");
        }
    } else {
        DisplayMessage("Failed to load new disc.");
    }
}

void CheckDiscSwap(void) {
    int result = CdDiskReady(0);
    if (result == CdlDiskChanged)
        HandleDiscSwap();
}
