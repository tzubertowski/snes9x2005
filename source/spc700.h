#include "../copyright"

#ifndef USE_BLARGG_APU

#ifndef _SPC700_H_
#define _SPC700_H_

#ifdef SPCTOOL
#define NO_CHANNEL_STRUCT
#include "spctool/dsp.h"
#include "spctool/spc700.h"
#include "spctool/soundmod.h"
#endif

#define Carry       1
#define Zero        2
#define Interrupt   4
#define HalfCarry   8
#define BreakFlag  16
#define DirectPageFlag 32
#define Overflow   64
#define Negative  128

#define APUClearCarry() (IAPU._Carry = 0)
#define APUSetCarry() (IAPU._Carry = 1)
#define APUSetInterrupt() (IAPU.Registers.P |= Interrupt)
#define APUClearInterrupt() (IAPU.Registers.P &= ~Interrupt)
#define APUSetHalfCarry() (IAPU.Registers.P |= HalfCarry)
#define APUClearHalfCarry() (IAPU.Registers.P &= ~HalfCarry)
#define APUSetBreak() (IAPU.Registers.P |= BreakFlag)
#define APUClearBreak() (IAPU.Registers.P &= ~BreakFlag)
#define APUSetDirectPage() (IAPU.Registers.P |= DirectPageFlag)
#define APUClearDirectPage() (IAPU.Registers.P &= ~DirectPageFlag)
#define APUSetOverflow() (IAPU._Overflow = 1)
#define APUClearOverflow() (IAPU._Overflow = 0)

#define APUCheckZero() (IAPU._Zero == 0)
#define APUCheckCarry() (IAPU._Carry)
#define APUCheckInterrupt() (IAPU.Registers.P & Interrupt)
#define APUCheckHalfCarry() (IAPU.Registers.P & HalfCarry)
#define APUCheckBreak() (IAPU.Registers.P & BreakFlag)
#define APUCheckDirectPage() (IAPU.Registers.P & DirectPageFlag)
#define APUCheckOverflow() (IAPU._Overflow)
#define APUCheckNegative() (IAPU._Zero & 0x80)

#define APUClearFlags(f) (IAPU.Registers.P &= ~(f))
#define APUSetFlags(f)   (IAPU.Registers.P |=  (f))
#define APUCheckFlag(f)  (IAPU.Registers.P &   (f))

typedef union
{
#ifdef MSB_FIRST
   struct
   {
      uint8_t Y, A;
   } B;
#else
   struct
   {
      uint8_t A, Y;
   } B;
#endif
   uint16_t W;
} YAndA;

typedef struct
{
   uint8_t  P;
   YAndA YA;
   uint8_t  X;
   uint8_t  S;
   uint16_t  PC;
} SAPURegisters;

// Needed by ILLUSION OF GAIA
//#define ONE_APU_CYCLE 14
#define ONE_APU_CYCLE 21

// Needed by all games written by the software company called Human
//#define ONE_APU_CYCLE_HUMAN 17
#define ONE_APU_CYCLE_HUMAN 21

// 1.953us := 1.024065.54MHz

#ifdef SPCTOOL
int32_t ESPC(int32_t);

#define APU_EXECUTE() \
{ \
    int32_t l = (CPU.Cycles - APU.Cycles) / 14; \
    if (l > 0) \
    { \
        l -= _EmuSPC(l); \
        APU.Cycles += l * 14; \
    } \
}

#else

#define APU_EXECUTE1() \
{ \
    APU.Cycles += S9xAPUCycles [*IAPU.PC]; \
    (*S9xApuOpcodes[*IAPU.PC]) (); \
}

#define APU_EXECUTE() \
if (IAPU.APUExecuting) \
{\
    while (APU.Cycles <= CPU.Cycles) \
   APU_EXECUTE1(); \
}
#endif

#endif

#endif
