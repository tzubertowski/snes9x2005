#include "../copyright"

#ifndef _SNES9X_H_
#define _SNES9X_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <libretro.h>

extern int cprintf(const char* fmt, ...);

#include "port.h"
#include "65c816.h"
#include "messages.h"

#define ROM_NAME_LEN 23

#define STREAM FILE*
#define READ_STREAM(p,l,s) fread (p,1,l,s)
#define WRITE_STREAM(p,l,s) fwrite (p,1,l,s)
#define OPEN_STREAM(f,m) fopen (f,m)
#define REOPEN_STREAM(f,m) fdopen (f,m)
#define FIND_STREAM(f)  ftell(f)
#define REVERT_STREAM(f,o,s)   fseek(f,o,s)
#define CLOSE_STREAM(s) fclose (s)


/* SNES screen width and height */
#define SNES_WIDTH      256
#define SNES_HEIGHT     224
#define SNES_HEIGHT_EXTENDED  239
#define IMAGE_WIDTH     (Settings.SupportHiRes ? SNES_WIDTH * 2 : SNES_WIDTH)
#define IMAGE_HEIGHT    (Settings.SupportHiRes ? SNES_HEIGHT_EXTENDED * 2 : SNES_HEIGHT_EXTENDED)

#define SNES_MAX_NTSC_VCOUNTER  262
#define SNES_MAX_PAL_VCOUNTER   312
#define SNES_HCOUNTER_MAX  342
#define SPC700_TO_65C816_RATIO   2
#define AUTO_FRAMERATE     200

/* NTSC master clock signal 21.47727MHz
 * PPU: master clock / 4
 * 1 / PPU clock * 342 -> 63.695us
 * 63.695us / (1 / 3.579545MHz) -> 228 cycles per scanline
 * From Earth Worm Jim: APU executes an average of 65.14285714 cycles per
 * scanline giving an APU clock speed of 1.022731096MHz                    */

/* PAL master clock signal 21.28137MHz
 * PPU: master clock / 4
 * 1 / PPU clock * 342 -> 64.281us
 * 64.281us / (1 / 3.546895MHz) -> 228 cycles per scanline.  */

#define SNES_SCANLINE_TIME (63.695e-6)
#define SNES_CLOCK_SPEED (3579545)

#define SNES_CLOCK_LEN (1.0 / SNES_CLOCK_SPEED)

#define SNES_CYCLES_PER_SCANLINE ((uint32_t) ((SNES_SCANLINE_TIME / SNES_CLOCK_LEN) * 6 + 0.5))

#define ONE_CYCLE 6
#define SLOW_ONE_CYCLE 8
#define TWO_CYCLES 12


#define SNES_TR_MASK     (1 << 4)
#define SNES_TL_MASK     (1 << 5)
#define SNES_X_MASK      (1 << 6)
#define SNES_A_MASK      (1 << 7)
#define SNES_RIGHT_MASK     (1 << 8)
#define SNES_LEFT_MASK      (1 << 9)
#define SNES_DOWN_MASK      (1 << 10)
#define SNES_UP_MASK     (1 << 11)
#define SNES_START_MASK     (1 << 12)
#define SNES_SELECT_MASK    (1 << 13)
#define SNES_Y_MASK      (1 << 14)
#define SNES_B_MASK      (1 << 15)

enum
{
   SNES_MULTIPLAYER5,
   SNES_JOYPAD,
   SNES_MOUSE_SWAPPED,
   SNES_MOUSE,
   SNES_SUPERSCOPE,
   SNES_JUSTIFIER,
   SNES_JUSTIFIER_2,
   SNES_MAX_CONTROLLER_OPTIONS
};

#define DEBUG_MODE_FLAG     (1 << 0)
#define TRACE_FLAG         (1 << 1)
#define SINGLE_STEP_FLAG    (1 << 2)
#define BREAK_FLAG         (1 << 3)
#define SCAN_KEYS_FLAG      (1 << 4)
#define SAVE_SNAPSHOT_FLAG  (1 << 5)
#define DELAYED_NMI_FLAG    (1 << 6)
#define NMI_FLAG        (1 << 7)
#define PROCESS_SOUND_FLAG  (1 << 8)
#define FRAME_ADVANCE_FLAG  (1 << 9)
#define DELAYED_NMI_FLAG2   (1 << 10)
#define IRQ_PENDING_FLAG    (1 << 11)

typedef struct
{
   uint32_t  Flags;
   bool   BranchSkip;
   bool   NMIActive;
   uint8_t   IRQActive;
   bool   WaitingForInterrupt;
   bool   InDMA;
   uint8_t   WhichEvent;
   uint8_t*   PC;
   uint8_t*   PCBase;
   uint8_t*   PCAtOpcodeStart;
   uint8_t*   WaitAddress;
   uint32_t  WaitCounter;
   long   Cycles;
   long   NextEvent;
   long   V_Counter;
   long   MemSpeed;
   long   MemSpeedx2;
   long   FastROMSpeed;
   uint32_t AutoSaveTimer;
   bool  SRAMModified;
   uint32_t NMITriggerPoint;
   bool  BRKTriggered;
   bool  TriedInterleavedMode2;
   uint32_t NMICycleCount;
   uint32_t IRQCycleCount;
} SCPUState;

#define HBLANK_START_EVENT 0
#define HBLANK_END_EVENT 1
#define HTIMER_BEFORE_EVENT 2
#define HTIMER_AFTER_EVENT 3
#define NO_EVENT 4

typedef struct
{
   /* CPU options */
   bool  APUEnabled;
   bool  Shutdown;
   uint8_t  SoundSkipMethod;
   long   H_Max;
   long   HBlankStart;
   long   CyclesPercentage;
   bool  DisableIRQ;
   bool  Paused;
   bool  ForcedPause;
   bool  StopEmulation;
   bool  FrameAdvance;

   /* Tracing options */
   bool  TraceDMA;
   bool  TraceHDMA;
   bool  TraceVRAM;
   bool  TraceUnknownRegisters;
   bool  TraceDSP;

   /* Joystick options */
   bool  SwapJoypads;
   bool  JoystickEnabled;

   /* ROM timing options (see also H_Max above) */
   bool  ForcePAL;
   bool  ForceNTSC;
   bool  PAL;
   uint32_t FrameTimePAL;
   uint32_t FrameTimeNTSC;
   uint32_t FrameTime;
   uint32_t SkipFrames;

   /* ROM image options */
   bool  ForceLoROM;
   bool  ForceHiROM;
   bool  ForceHeader;
   bool  ForceNoHeader;
   bool  ForceInterleaved;
   bool  ForceInterleaved2;
   bool  ForceNotInterleaved;

   /* Peripherial options */
   bool  ForceSuperFX;
   bool  ForceNoSuperFX;
   bool  ForceDSP1;
   bool  ForceNoDSP1;
   bool  ForceSA1;
   bool  ForceNoSA1;
   bool  ForceC4;
   bool  ForceNoC4;
   bool  ForceSDD1;
   bool  ForceNoSDD1;
   bool  MultiPlayer5;
   bool  Mouse;
   bool  SuperScope;
   bool  SRTC;
   uint32_t ControllerOption;

   bool  ShutdownMaster;
   bool  MultiPlayer5Master;
   bool  SuperScopeMaster;
   bool  MouseMaster;
   bool  SuperFX;
   bool  DSP1Master;
   bool  SA1;
   bool  C4;
   bool  SDD1;
   bool  SPC7110;
   bool  SPC7110RTC;
   bool  OBC1;
   /* Sound options */
   uint32_t SoundPlaybackRate;
#ifdef USE_BLARGG_APU
   uint32_t SoundInputRate;
#endif
   bool  TraceSoundDSP;
   bool  EightBitConsoleSound;  // due to caching, this needs S9xSetEightBitConsoleSound()
   int    SoundBufferSize;
   int    SoundMixInterval;
   bool  SoundEnvelopeHeightReading;
   bool  DisableSoundEcho;
   bool  DisableMasterVolume;
   bool  SoundSync;
   bool  InterpolatedSound;
   bool  ThreadSound;
   bool  Mute;
   bool  NextAPUEnabled;

   /* Graphics options */
   bool  Transparency;
   bool  SupportHiRes;
   bool  Mode7Interpolate;

   /* SNES graphics options */
   bool  BGLayering;
   bool  DisableGraphicWindows;
   bool  ForceTransparency;
   bool  ForceNoTransparency;
   bool  DisableHDMA;
   bool  DisplayFrameRate;
   bool  DisableRangeTimeOver; /* XXX: unused */

   /* Others */
   bool  ApplyCheats;

   /* Fixes for individual games */
   bool  StarfoxHack;
   bool  WinterGold;
   bool  BS; /* Japanese Satellite System games. */
   bool  DaffyDuck;
   uint8_t  APURAMInitialValue;
   bool  SampleCatchup;
   bool  JustifierMaster;
   bool  Justifier;
   bool  SecondJustifier;
   int8_t   SETA;
   bool  TakeScreenshot;
   int8_t   StretchScreenshots;
   uint16_t DisplayColor;
   int    SoundDriver;
   int    AIDOShmId;
   bool  NoPatch;
   bool  ForceInterleaveGD24;
} SSettings;

typedef struct
{
   uint8_t alienVSpredetorFix;
   uint8_t APU_OutPorts_ReturnValueFix;
   uint8_t SoundEnvelopeHeightReading2;
   uint8_t SRAMInitialValue;
   uint8_t Uniracers;
   bool EchoOnlyOutput;
} SSNESGameFixes;

extern SSettings Settings;
extern SCPUState CPU;
extern SSNESGameFixes SNESGameFixes;
extern char String [513];

void S9xMessage(int type, int number, const char* message);
void S9xLoadSDD1Data();

void S9xSetPause(uint32_t mask);
void S9xClearPause(uint32_t mask);

#endif

