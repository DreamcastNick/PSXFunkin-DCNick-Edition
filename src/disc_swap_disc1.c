#include "disc_swap_disc1.h"

#include "audio.h"
#include "pad.h"
#include "psx.h" // Include psx.h for dependencies

int currentDisc = 1;

FileNode *fileList = NULL;

void HandleDiscSwap(void) {
    DisplayMessage("Attempting to switch the disc...");

    // Attempt to switch the disc
    int switchResult = CdSwitchDisc();
    if (switchResult == 0) {
        DisplayMessage("Verifying new disc...");
        if (CheckDiscIndicator()) {
            currentDisc = (currentDisc % 3) + 1; // Update currentDisc to cycle between 1, 2, and 3
            DisplayMessage("New disc loaded successfully.");

            // Update currentDisc based on the loaded disc
            CheckCurrentDisc();
			
            // Clear existing file list
            freeFileList();

            // Traverse the new disc and collect file paths
            traverseDirectory("\\");

            // Load all files from the collected file list
            loadAllFiles();

        } else {
            DisplayMessage("New disc is not valid. Please insert the correct disc.");
            return;
        }
    } else {
        DisplayMessage("Failed to load new disc.");
        switch (switchResult) {
            case -1:
                DisplayMessage("Error: Timeout while waiting for CD drive.");
                break;
            case -3:
                DisplayMessage("Error: DISC1.ID, DISC2.ID, or DISC3.ID not found on the new disc.");
                break;
            case -4:
                DisplayMessage("Error: Failed to stop or open the tray.");
                break;
            case -5:
                DisplayMessage("Error: Failed to close the tray.");
                break;
            default:
                DisplayMessage("Error: Unknown error occurred.");
                break;
        }
        return;
    }
}

int UserPressedButton(void) {
    // Implementation for checking if the user has pressed a button
    // This is a placeholder, and should be replaced with actual input handling code
    // For example, using PadRead() or equivalent function
    return (pad_state.press & PAD_SELECT) || (pad_state.press & PAD_START) || (pad_state.press & PAD_CROSS);
}

int CdSwitchDisc(void) {
    int timeout = 0;
    // Wait for the CD drive to be ready
    while (CdDiskReady(0) != CdlDiskReady) {
        CdControlF(CdlNop, NULL);
        if (++timeout > 10000) { // Adding timeout
            DisplayMessage("Timeout waiting for CD drive to be ready.");
            return -1; // Timeout error
        }
    }

    if (!CheckDiscIndicator()) {
        DisplayMessage("DISC1.ID, DISC2.ID, or DISC3.ID not found on the new disc.");
        return -3; // Error: DISC1.ID, DISC2.ID, or DISC3.ID not found
    }

    if (CdDiskReady(0) == CdlDiskReady) {
        if (CdStop() == 0) {
            DisplayMessage("Failed to stop current disc.");
            return -4; // Error: Failed to stop
        }

        if (CdTrayReq(CdlTrayOpen, NULL) == 0) {
            DisplayMessage("Failed to open the tray.");
            return -4; // Error: Failed to open tray
        }

        timeout = 0;
        while (CdDiskReady(0) != CdlTrayOpen) {
            CdControlF(CdlNop, NULL);
            if (++timeout > 10000) { // Adding timeout
                DisplayMessage("Timeout waiting for CD tray to open.");
                return -1; // Timeout error
            }
        }

        if (CdTrayReq(CdlTrayClose, NULL) == 0) {
            DisplayMessage("Failed to close the tray.");
            return -5; // Error: Failed to close tray
        }

        timeout = 0;
        while (CdDiskReady(0) != CdlDiskReady) {
            CdControlF(CdlNop, NULL);
            if (++timeout > 10000) { // Adding timeout
                DisplayMessage("Timeout waiting for CD tray to close.");
                return -1; // Timeout error
            }
        }
    }

    DisplayMessage("Switched to new disc.");
    return 0; // Success
}

int CheckDiscIndicator(void) {
    CdlFILE disc1File, disc2File, disc3File;

    // Attempt to find  DISC1.ID, DISC2.ID, or DISC3.ID on the new disc
    if (IO_FindFile(&disc1File, "\\DISC1.ID;1") || IO_FindFile(&disc2File, "\\DISC2.ID;1") || IO_FindFile(&disc3File, "\\DISC3.ID;1")) {
        return 1; //  DISC1.ID, DISC2.ID, or  found
    }

    return 0; // DISC1.ID, DISC2.ID, and DISC3.ID not found
}

int CdTrayReq(int req, void* param) {
    (void)param; // Mark param as unused to avoid warning
    // PSX hardware CD tray request
    switch (req) {
        case CdlTrayOpen:
            CdControlF(CdlTrayOpen, NULL);
            break;
        case CdlTrayClose:
            CdControlF(CdlTrayClose, NULL);
            break;
        default:
            DisplayMessage("Unsupported CD tray request.");
            return -1; // Error: unsupported request
    }
    return 0; // Success
}

void CheckCurrentDisc(void) {
    CdlFILE file;

    if (IO_FindFile(&file, "\\DISC3.ID;1")) {
        currentDisc = 3;
    }

    if (IO_FindFile(&file, "\\DISC2.ID;1")) {
        currentDisc = 2;
    }

    if (IO_FindFile(&file, "\\DISC1.ID;1")) {
        currentDisc = 1;
    }
}


void DisplayMessage(const char* message) {
    FntPrint(message);
    FntFlush(-1);
	Gfx_Flip();
    printf("%s\n", message); // For debugging via console
}

void CheckDiscSwap(void) {
    int result = CdDiskReady(0);

    switch (result) {
        case CdlDiskChanged:
            DisplayMessage("Disc has been changed.");
            HandleDiscSwap();
            break;
        case CdlNoDisk:
            DisplayMessage("No disc present. Please insert a disc.");
            break;
        case CdlDiskOK:
            // Optionally handle this case if needed
            break;
        default:
            DisplayMessage("Unknown disc status.");
            break;
    }
}

void addFileToList(const char *filename) {
    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newNode->name, filename);
    newNode->next = fileList;
    fileList = newNode;
}

void traverseDirectory(const char *path) {
    CdlFILE file;
    char searchPath[1024];
    sprintf(searchPath, "%s\\*.*;1", path);

    // Directory buffer (adjust size as necessary)
    CdlFILE dirEntries[128];

    // Read directory entries
    if (CdControlB(CdlReadN, (u_char *)searchPath, (u_char *)dirEntries) == 0) {
        printf("Failed to read directory: %s\n", path);
        return;
    }

    for (int i = 0; dirEntries[i].name[0] != '\0'; ++i) {
        char fullPath[1024];
        sprintf(fullPath, "%s\\%s", path, dirEntries[i].name);

        if (strchr(dirEntries[i].name, '.') == NULL) {
            // It's a directory (no extension)
            traverseDirectory(fullPath);
        } else {
            // It's a file (has extension)
            addFileToList(fullPath);
        }
    }
}

void listAllFiles() {
    traverseDirectory("\\");
}

void loadFile(const char *filename) {
    CdlFILE file;
    if (CdSearchFile(&file, filename) != NULL) {
        printf("Loading file: %s\n", filename);
        char *buffer = (char *)malloc(file.size);
        if (buffer == NULL) {
            printf("Failed to allocate memory for file: %s\n", filename);
            return;
        }
        if (CdReadFile(filename, buffer, file.size) != 0) {
            printf("File loaded successfully: %s\n", filename);
        } else {
            printf("Failed to read file: %s\n", filename);
            free(buffer);
        }
    } else {
        printf("File not found: %s\n", filename);
    }
}

void loadAllFiles(void) {
    FileNode *current = fileList;
    while (current != NULL) {
        loadFile(current->name);
        current = current->next;
    }
}

void freeFileList(void) {
    FileNode *current = fileList;
    while (current != NULL) {
        FileNode *temp = current;
        current = current->next;
        free(temp);
    }
    fileList = NULL;
}
