#include "../copyright"

#ifndef _CPUADDR_H_
#define _CPUADDR_H_

typedef enum
{
   NONE = 0,
   READ = 1,
   WRITE = 2,
   MODIFY = 3,
   JUMP = 4
} AccessMode;

// The type for a function that can run after the addressing mode is resolved:
// void NAME (long Addr) {...}
typedef void (*InternalOp)(long);

static void Immediate8(AccessMode a, InternalOp op)
{
   long Addr = ICPU.ShiftedPB + CPU.PC - CPU.PCBase;
   CPU.PC++;
   (*op)(Addr);
}

static void Immediate16(AccessMode a, InternalOp op)
{
   long Addr = ICPU.ShiftedPB + CPU.PC - CPU.PCBase;
   CPU.PC += 2;
   (*op)(Addr);
}

static void Relative(AccessMode a, InternalOp op)
{
   int8_t Int8 = *CPU.PC++;
   long Addr = ((intptr_t)(CPU.PC - CPU.PCBase) + Int8) & 0xffff;
   (*op)(Addr);
}

static void RelativeLong(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = *(uint16_t*) CPU.PC;
#else
   Addr = *CPU.PC + (*(CPU.PC + 1) << 8);
#endif
   CPU.PC += 2;
   Addr += (CPU.PC - CPU.PCBase);
   Addr &= 0xffff;
   (*op)(Addr);
}

static void AbsoluteIndexedIndirect(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = (ICPU.Registers.X.W + * (uint16_t*) CPU.PC) & 0xffff;
#else
   Addr = (ICPU.Registers.X.W + *CPU.PC + (*(CPU.PC + 1) << 8)) & 0xffff;
#endif
   OpenBus = *(CPU.PC + 1);
   CPU.PC += 2;
   Addr = S9xGetWord(ICPU.ShiftedPB + Addr);
   if (a & READ) OpenBus = (uint8_t)(Addr >> 8);
   (*op)(Addr);
}

static void AbsoluteIndirectLong(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = *(uint16_t*) CPU.PC;
#else
   Addr = *CPU.PC + (*(CPU.PC + 1) << 8);
#endif

   OpenBus = *(CPU.PC + 1);
   CPU.PC += 2;
   if (a & READ)
      Addr = S9xGetWord(Addr) | ((OpenBus = S9xGetByte(Addr + 2)) << 16);
   else
      Addr = S9xGetWord(Addr) | (S9xGetByte(Addr + 2) << 16);
   (*op)(Addr);
}

static void AbsoluteIndirect(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = *(uint16_t*) CPU.PC;
#else
   Addr = *CPU.PC + (*(CPU.PC + 1) << 8);
#endif

   OpenBus = *(CPU.PC + 1);
   CPU.PC += 2;
   Addr = S9xGetWord(Addr);
   if (a & READ) OpenBus = (uint8_t)(Addr >> 8);
   Addr += ICPU.ShiftedPB;
   (*op)(Addr);
}

static void Absolute(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = *(uint16_t*) CPU.PC + ICPU.ShiftedDB;
#else
   Addr = *CPU.PC + (*(CPU.PC + 1) << 8) + ICPU.ShiftedDB;
#endif
   if (a & READ) OpenBus = *(CPU.PC + 1);
   CPU.PC += 2;
   (*op)(Addr);
}

static void AbsoluteLong(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = (*(uint32_t*) CPU.PC) & 0xffffff;
#elif defined FAST_ALIGNED_LSB_WORD_ACCESS
   if (((intptr_t) CPU.PC & 1) == 0)
      Addr = (*(uint16_t*) CPU.PC) + (*(CPU.PC + 2) << 16);
   else
      Addr = *CPU.PC + ((*(uint16_t*)(CPU.PC + 1)) << 8);
#else
   Addr = *CPU.PC + (*(CPU.PC + 1) << 8) + (*(CPU.PC + 2) << 16);
#endif
   if (a & READ) OpenBus = *(CPU.PC + 2);
   CPU.PC += 3;
   (*op)(Addr);
}

static void Direct(AccessMode a, InternalOp op)
{
   if (a & READ) OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W) & 0xffff;
   //    if (ICPU.Registers.DL != 0) CPU.Cycles += ONE_CYCLE;
   (*op)(Addr);
}

static void DirectIndirectIndexed(AccessMode a, InternalOp op)
{
   OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W) & 0xffff;

   Addr = S9xGetWord(Addr);
   if (a & READ) OpenBus = (uint8_t)(Addr >> 8);
   Addr += ICPU.ShiftedDB + ICPU.Registers.Y.W;

   //    if (ICPU.Registers.DL != 0) CPU.Cycles += ONE_CYCLE;
   // XXX: always add one if STA
   // XXX: else Add one cycle if crosses page boundary
   (*op)(Addr);
}

static void DirectIndirectIndexedLong(AccessMode a, InternalOp op)
{
   OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W) & 0xffff;

   if (a & READ)
      Addr = S9xGetWord(Addr) + ((OpenBus = S9xGetByte(Addr + 2)) << 16) +
             ICPU.Registers.Y.W;
   else
      Addr = S9xGetWord(Addr) + (S9xGetByte(Addr + 2) << 16) + ICPU.Registers.Y.W;
   //    if (ICPU.Registers.DL != 0) CPU.Cycles += ONE_CYCLE;
   (*op)(Addr);
}

static void DirectIndexedIndirect(AccessMode a, InternalOp op)
{
   OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W + ICPU.Registers.X.W) & 0xffff;

   Addr = S9xGetWord(Addr);
   if (a & READ) OpenBus = (uint8_t)(Addr >> 8);
   Addr += ICPU.ShiftedDB;
   (*op)(Addr);
}

static void DirectIndexedX(AccessMode a, InternalOp op)
{
   if (a & READ) OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W + ICPU.Registers.X.W);
   Addr &= CheckEmulation() ? 0xff : 0xffff;

   (*op)(Addr);
}

static void DirectIndexedY(AccessMode a, InternalOp op)
{
   if (a & READ) OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W + ICPU.Registers.Y.W);
   Addr &= CheckEmulation() ? 0xff : 0xffff;
   (*op)(Addr);
}

static void AbsoluteIndexedX(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = ICPU.ShiftedDB + *(uint16_t*) CPU.PC + ICPU.Registers.X.W;
#else
   Addr = ICPU.ShiftedDB + *CPU.PC + (*(CPU.PC + 1) << 8) +
          ICPU.Registers.X.W;
#endif
   if (a & READ) OpenBus = *(CPU.PC + 1);
   CPU.PC += 2;
   // XXX: always add one cycle for ROL, LSR, etc
   // XXX: else is cross page boundary add one cycle
   (*op)(Addr);
}

static void AbsoluteIndexedY(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = ICPU.ShiftedDB + *(uint16_t*) CPU.PC + ICPU.Registers.Y.W;
#else
   Addr = ICPU.ShiftedDB + *CPU.PC + (*(CPU.PC + 1) << 8) +
          ICPU.Registers.Y.W;
#endif
   if (a & READ) OpenBus = *(CPU.PC + 1);
   CPU.PC += 2;
   // XXX: always add cycle for STA
   // XXX: else is cross page boundary add one cycle
   (*op)(Addr);
}

static void AbsoluteLongIndexedX(AccessMode a, InternalOp op)
{
   long Addr;
#ifdef FAST_LSB_WORD_ACCESS
   Addr = (*(uint32_t*) CPU.PC + ICPU.Registers.X.W) & 0xffffff;
#elif defined FAST_ALIGNED_LSB_WORD_ACCESS
   if (((intptr_t) CPU.PC & 1) == 0)
      Addr = ((*(uint16_t*) CPU.PC) + (*(CPU.PC + 2) << 16) + ICPU.Registers.X.W) &
             0xFFFFFF;
   else
      Addr = (*CPU.PC + ((*(uint16_t*)(CPU.PC + 1)) << 8) + ICPU.Registers.X.W) &
             0xFFFFFF;
#else
   Addr = (*CPU.PC + (*(CPU.PC + 1) << 8) + (*(CPU.PC + 2) << 16) +
           ICPU.Registers.X.W) & 0xffffff;
#endif
   if (a & READ) OpenBus = *(CPU.PC + 2);
   CPU.PC += 3;
   (*op)(Addr);
}

static void DirectIndirect(AccessMode a, InternalOp op)
{
   OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W) & 0xffff;
   Addr = S9xGetWord(Addr);
   if (a & READ) OpenBus = (uint8_t)(Addr >> 8);
   Addr += ICPU.ShiftedDB;

   //    if (ICPU.Registers.DL != 0) CPU.Cycles += ONE_CYCLE;
   (*op)(Addr);
}

static void DirectIndirectLong(AccessMode a, InternalOp op)
{
   OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.D.W) & 0xffff;
   if (a & READ)
      Addr = S9xGetWord(Addr) + ((OpenBus = S9xGetByte(Addr + 2)) << 16);
   else
      Addr = S9xGetWord(Addr) + (S9xGetByte(Addr + 2) << 16);
   //    if (ICPU.Registers.DL != 0) CPU.Cycles += ONE_CYCLE;
   (*op)(Addr);
}

static void StackRelative(AccessMode a, InternalOp op)
{
   if (a & READ) OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.S.W) & 0xffff;
   (*op)(Addr);
}

static void StackRelativeIndirectIndexed(AccessMode a, InternalOp op)
{
   OpenBus = *CPU.PC;
   long Addr = (*CPU.PC++ + ICPU.Registers.S.W) & 0xffff;
   Addr = S9xGetWord(Addr);
   if (a & READ) OpenBus = (uint8_t)(Addr >> 8);
   Addr = (Addr + ICPU.ShiftedDB +
           ICPU.Registers.Y.W) & 0xffffff;
   (*op)(Addr);
}
#endif

