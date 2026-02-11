#ifndef PSXF_GUARD_DISC_SWAP_H
#define PSXF_GUARD_DISC_SWAP_H

extern int currentDisc;

void HandleDiscSwap(void);
int UserPressedButton(void);
int CdSwitchDisc(void);
int CheckDiscIndicator(void);
int CdTrayReq(int req, void* param);
void CheckCurrentDisc(void);
void DisplayMessage(const char* message);
void CheckDiscSwap(void);

#endif
