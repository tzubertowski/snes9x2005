#include "../copyright"

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

// Routines the port specific code has to implement
uint32_t S9xReadJoypad(int port);
bool S9xReadMousePosition(int which1_0_to_1, int* x, int* y, uint32_t* buttons);
bool S9xReadSuperScopePosition(int* x, int* y, uint32_t* buttons);

void S9xInitDisplay(void);
void S9xDeinitDisplay();
void S9xToggleSoundChannel(int channel);
void S9xSetInfoString(const char* string);
void S9xNextController();
bool S9xLoadROMImage(const char* string);

const char* S9xGetFilename(const char* extension);

#endif
