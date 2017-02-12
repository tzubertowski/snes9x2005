#include "../copyright"

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

// Routines the port specific code has to implement
uint32_t S9xReadJoypad(int32_t port);
bool S9xReadMousePosition(int32_t which1_0_to_1, int32_t* x, int32_t* y, uint32_t* buttons);
bool S9xReadSuperScopePosition(int32_t* x, int32_t* y, uint32_t* buttons);

void S9xInitDisplay();
void S9xDeinitDisplay();
void S9xToggleSoundChannel(int32_t channel);
void S9xSetInfoString(const char* string);
void S9xNextController();
bool S9xLoadROMImage(const char* string);

const char* S9xGetFilename(const char* extension);

#endif
