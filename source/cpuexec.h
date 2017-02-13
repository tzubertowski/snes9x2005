#include "../copyright"

#ifndef _CPUEXEC_H_
#define _CPUEXEC_H_

typedef struct
{
#ifdef __WIN32__
   void (__cdecl* S9xOpcode)();
#else
   void (*S9xOpcode)();
#endif
} SOpcodes;

#include "ppu.h"
#include "memmap.h"
#include "65c816.h"

#define DO_HBLANK_CHECK_SFX() \
    if (CPU.Cycles >= CPU.NextEvent) \
   S9xDoHBlankProcessing_SFX ();

#define DO_HBLANK_CHECK_NoSFX() \
    if (CPU.Cycles >= CPU.NextEvent) \
   S9xDoHBlankProcessing_NoSFX ();

typedef struct
{
   uint8_t*   Speed;
   SOpcodes*  S9xOpcodes;
   SRegisters Registers;
   uint8_t    _Carry;
   uint8_t    _Zero;
   uint8_t    _Negative;
   uint8_t    _Overflow;
   bool       CPUExecuting;
   uint32_t   ShiftedPB;
   uint32_t   ShiftedDB;
   uint32_t   Frame;
   uint32_t   Scanline;
   uint32_t   FrameAdvanceCount;
} SICPU;

void S9xMainLoop();
void S9xReset();
void S9xSoftReset();
void S9xDoHBlankProcessing_SFX();
void S9xDoHBlankProcessing_NoSFX();
void S9xClearIRQ(uint32_t);
void S9xSetIRQ(uint32_t);

extern SOpcodes S9xOpcodesE1 [256];
extern SOpcodes S9xOpcodesM1X1 [256];
extern SOpcodes S9xOpcodesM1X0 [256];
extern SOpcodes S9xOpcodesM0X1 [256];
extern SOpcodes S9xOpcodesM0X0 [256];

extern SICPU ICPU;

static inline void S9xUnpackStatus()
{
   ICPU._Zero = (ICPU.Registers.PL & Zero) == 0;
   ICPU._Negative = (ICPU.Registers.PL & Negative);
   ICPU._Carry = (ICPU.Registers.PL & Carry);
   ICPU._Overflow = (ICPU.Registers.PL & Overflow) >> 6;
}

static inline void S9xPackStatus()
{
   ICPU.Registers.PL &= ~(Zero | Negative | Carry | Overflow);
   ICPU.Registers.PL |= ICPU._Carry | ((ICPU._Zero == 0) << 1) | (ICPU._Negative & 0x80) | (ICPU._Overflow << 6);
}

static inline void CLEAR_IRQ_SOURCE(uint32_t M)
{
   CPU.IRQActive &= ~M;
   if (!CPU.IRQActive)
      CPU.Flags &= ~IRQ_PENDING_FLAG;
}

static inline void S9xFixCycles()
{
   if (CheckEmulation())
      ICPU.S9xOpcodes = S9xOpcodesE1;
   else if (CheckMemory())
   {
      if (CheckIndex())
         ICPU.S9xOpcodes = S9xOpcodesM1X1;
      else
         ICPU.S9xOpcodes = S9xOpcodesM1X0;
   }
   else
   {
      if (CheckIndex())
         ICPU.S9xOpcodes = S9xOpcodesM0X1;
      else
         ICPU.S9xOpcodes = S9xOpcodesM0X0;
   }
}

static inline void S9xReschedule()
{
   uint8_t which;
   int32_t max;

   if (CPU.WhichEvent == HBLANK_START_EVENT || CPU.WhichEvent == HTIMER_AFTER_EVENT)
   {
      which = HBLANK_END_EVENT;
      max = Settings.H_Max;
   }
   else
   {
      which = HBLANK_START_EVENT;
      max = Settings.HBlankStart;
   }

   if (PPU.HTimerEnabled &&
         (int32_t) PPU.HTimerPosition < max &&
         (int32_t) PPU.HTimerPosition > CPU.NextEvent &&
         (!PPU.VTimerEnabled ||
         (PPU.VTimerEnabled && CPU.V_Counter == PPU.IRQVBeamPos)))
   {
      which = (int32_t) PPU.HTimerPosition < Settings.HBlankStart ? HTIMER_BEFORE_EVENT : HTIMER_AFTER_EVENT;
      max = PPU.HTimerPosition;
   }
   CPU.NextEvent = max;
   CPU.WhichEvent = which;
}

#endif
