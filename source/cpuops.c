#include "../copyright"

/*****************************************************************************/
/* CPU-S9xOpcodes.CPP                                                        */
/* This file contains all the opcodes                                        */
/*****************************************************************************/

#include "snes9x.h"
#include "memmap.h"
#include "missing.h"
#include "apu.h"
#include "sa1.h"
#include "spc7110.h"

#include "cpuexec.h"
#include "cpuaddr.h"
#include "cpuops.h"
#include "cpumacro.h"
#include "apu.h"

int32_t OpAddress;

/* ADC *************************************************************************************** */
static void Op69M1()
{
   Immediate8();
   ADC8();
}

static void Op69M0()
{
   Immediate16();
   ADC16();
}

static void Op65M1()
{
   Direct(true);
   ADC8();
}

static void Op65M0()
{
   Direct(true);
   ADC16();
}

static void Op75M1()
{
   DirectIndexedX(true);
   ADC8();
}

static void Op75M0()
{
   DirectIndexedX(true);
   ADC16();
}

static void Op72M1()
{
   DirectIndirect(true);
   ADC8();
}

static void Op72M0()
{
   DirectIndirect(true);
   ADC16();
}

static void Op61M1()
{
   DirectIndexedIndirect(true);
   ADC8();
}

static void Op61M0()
{
   DirectIndexedIndirect(true);
   ADC16();
}

static void Op71M1()
{
   DirectIndirectIndexed(true);
   ADC8();
}

static void Op71M0()
{
   DirectIndirectIndexed(true);
   ADC16();
}

static void Op67M1()
{
   DirectIndirectLong(true);
   ADC8();
}

static void Op67M0()
{
   DirectIndirectLong(true);
   ADC16();
}

static void Op77M1()
{
   DirectIndirectIndexedLong(true);
   ADC8();
}

static void Op77M0()
{
   DirectIndirectIndexedLong(true);
   ADC16();
}

static void Op6DM1()
{
   Absolute(true);
   ADC8();
}

static void Op6DM0()
{
   Absolute(true);
   ADC16();
}

static void Op7DM1()
{
   AbsoluteIndexedX(true);
   ADC8();
}

static void Op7DM0()
{
   AbsoluteIndexedX(true);
   ADC16();
}

static void Op79M1()
{
   AbsoluteIndexedY(true);
   ADC8();
}

static void Op79M0()
{
   AbsoluteIndexedY(true);
   ADC16();
}

static void Op6FM1()
{
   AbsoluteLong(true);
   ADC8();
}

static void Op6FM0()
{
   AbsoluteLong(true);
   ADC16();
}

static void Op7FM1()
{
   AbsoluteLongIndexedX(true);
   ADC8();
}

static void Op7FM0()
{
   AbsoluteLongIndexedX(true);
   ADC16();
}

static void Op63M1()
{
   StackRelative(true);
   ADC8();
}

static void Op63M0()
{
   StackRelative(true);
   ADC16();
}

static void Op73M1()
{
   StackRelativeIndirectIndexed(true);
   ADC8();
}

static void Op73M0()
{
   StackRelativeIndirectIndexed(true);
   ADC16();
}

/**********************************************************************************************/

/* AND *************************************************************************************** */
static void Op29M1()
{
   ICPU.Registers.AL &= *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
   SetZN8(ICPU.Registers.AL);
}

static void Op29M0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU.Registers.A.W &= *(uint16_t*) CPU.PC;
#else
   ICPU.Registers.A.W &= *CPU.PC + (*(CPU.PC + 1) << 8);
#endif
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   SetZN16(ICPU.Registers.A.W);
}

static void Op25M1()
{
   Direct(true);
   AND8();
}

static void Op25M0()
{
   Direct(true);
   AND16();
}

static void Op35M1()
{
   DirectIndexedX(true);
   AND8();
}

static void Op35M0()
{
   DirectIndexedX(true);
   AND16();
}

static void Op32M1()
{
   DirectIndirect(true);
   AND8();
}

static void Op32M0()
{
   DirectIndirect(true);
   AND16();
}

static void Op21M1()
{
   DirectIndexedIndirect(true);
   AND8();
}

static void Op21M0()
{
   DirectIndexedIndirect(true);
   AND16();
}

static void Op31M1()
{
   DirectIndirectIndexed(true);
   AND8();
}

static void Op31M0()
{
   DirectIndirectIndexed(true);
   AND16();
}

static void Op27M1()
{
   DirectIndirectLong(true);
   AND8();
}

static void Op27M0()
{
   DirectIndirectLong(true);
   AND16();
}

static void Op37M1()
{
   DirectIndirectIndexedLong(true);
   AND8();
}

static void Op37M0()
{
   DirectIndirectIndexedLong(true);
   AND16();
}

static void Op2DM1()
{
   Absolute(true);
   AND8();
}

static void Op2DM0()
{
   Absolute(true);
   AND16();
}

static void Op3DM1()
{
   AbsoluteIndexedX(true);
   AND8();
}

static void Op3DM0()
{
   AbsoluteIndexedX(true);
   AND16();
}

static void Op39M1()
{
   AbsoluteIndexedY(true);
   AND8();
}

static void Op39M0()
{
   AbsoluteIndexedY(true);
   AND16();
}

static void Op2FM1()
{
   AbsoluteLong(true);
   AND8();
}

static void Op2FM0()
{
   AbsoluteLong(true);
   AND16();
}

static void Op3FM1()
{
   AbsoluteLongIndexedX(true);
   AND8();
}

static void Op3FM0()
{
   AbsoluteLongIndexedX(true);
   AND16();
}

static void Op23M1()
{
   StackRelative(true);
   AND8();
}

static void Op23M0()
{
   StackRelative(true);
   AND16();
}

static void Op33M1()
{
   StackRelativeIndirectIndexed(true);
   AND8();
}

static void Op33M0()
{
   StackRelativeIndirectIndexed(true);
   AND16();
}
/**********************************************************************************************/

/* ASL *************************************************************************************** */
static void Op0AM1()
{
   A_ASL8();
}

static void Op0AM0()
{
   A_ASL16();
}

static void Op06M1()
{
   Direct(false);
   ASL8();
}

static void Op06M0()
{
   Direct(false);
   ASL16();
}

static void Op16M1()
{
   DirectIndexedX(false);
   ASL8();
}

static void Op16M0()
{
   DirectIndexedX(false);
   ASL16();
}

static void Op0EM1()
{
   Absolute(false);
   ASL8();
}

static void Op0EM0()
{
   Absolute(false);
   ASL16();
}

static void Op1EM1()
{
   AbsoluteIndexedX(false);
   ASL8();
}

static void Op1EM0()
{
   AbsoluteIndexedX(false);
   ASL16();
}
/**********************************************************************************************/

/* BIT *************************************************************************************** */
static void Op89M1()
{
   ICPU._Zero = ICPU.Registers.AL & *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
}

static void Op89M0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU._Zero = (ICPU.Registers.A.W & *(uint16_t*) CPU.PC) != 0;
#else
   ICPU._Zero = (ICPU.Registers.A.W & (*CPU.PC + (*(CPU.PC + 1) << 8))) != 0;
#endif
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   CPU.PC += 2;
}

static void Op24M1()
{
   Direct(true);
   BIT8();
}

static void Op24M0()
{
   Direct(true);
   BIT16();
}

static void Op34M1()
{
   DirectIndexedX(true);
   BIT8();
}

static void Op34M0()
{
   DirectIndexedX(true);
   BIT16();
}

static void Op2CM1()
{
   Absolute(true);
   BIT8();
}

static void Op2CM0()
{
   Absolute(true);
   BIT16();
}

static void Op3CM1()
{
   AbsoluteIndexedX(true);
   BIT8();
}

static void Op3CM0()
{
   AbsoluteIndexedX(true);
   BIT16();
}
/**********************************************************************************************/

/* CMP *************************************************************************************** */
static void OpC9M1()
{
   int32_t Int32 = (int32_t) ICPU.Registers.AL - (int32_t) *CPU.PC++;
   ICPU._Carry = Int32 >= 0;
   SetZN8((uint8_t) Int32);
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
}

static void OpC9M0()
{
#ifdef FAST_LSB_WORD_ACCESS
   int32_t Int32 = (int32_t) ICPU.Registers.A.W - (int32_t) *(uint16_t*)CPU.PC;
#else
   int32_t Int32 = (int32_t) ICPU.Registers.A.W - (int32_t)(*CPU.PC + (*(CPU.PC + 1) << 8));
#endif
   ICPU._Carry = Int32 >= 0;
   SetZN16((uint16_t) Int32);
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
}

static void OpC5M1()
{
   Direct(true);
   CMP8();
}

static void OpC5M0()
{
   Direct(true);
   CMP16();
}

static void OpD5M1()
{
   DirectIndexedX(true);
   CMP8();
}

static void OpD5M0()
{
   DirectIndexedX(true);
   CMP16();
}

static void OpD2M1()
{
   DirectIndirect(true);
   CMP8();
}

static void OpD2M0()
{
   DirectIndirect(true);
   CMP16();
}

static void OpC1M1()
{
   DirectIndexedIndirect(true);
   CMP8();
}

static void OpC1M0()
{
   DirectIndexedIndirect(true);
   CMP16();
}

static void OpD1M1()
{
   DirectIndirectIndexed(true);
   CMP8();
}

static void OpD1M0()
{
   DirectIndirectIndexed(true);
   CMP16();
}

static void OpC7M1()
{
   DirectIndirectLong(true);
   CMP8();
}

static void OpC7M0()
{
   DirectIndirectLong(true);
   CMP16();
}

static void OpD7M1()
{
   DirectIndirectIndexedLong(true);
   CMP8();
}

static void OpD7M0()
{
   DirectIndirectIndexedLong(true);
   CMP16();
}

static void OpCDM1()
{
   Absolute(true);
   CMP8();
}

static void OpCDM0()
{
   Absolute(true);
   CMP16();
}

static void OpDDM1()
{
   AbsoluteIndexedX(true);
   CMP8();
}

static void OpDDM0()
{
   AbsoluteIndexedX(true);
   CMP16();
}

static void OpD9M1()
{
   AbsoluteIndexedY(true);
   CMP8();
}

static void OpD9M0()
{
   AbsoluteIndexedY(true);
   CMP16();
}

static void OpCFM1()
{
   AbsoluteLong(true);
   CMP8();
}

static void OpCFM0()
{
   AbsoluteLong(true);
   CMP16();
}

static void OpDFM1()
{
   AbsoluteLongIndexedX(true);
   CMP8();
}

static void OpDFM0()
{
   AbsoluteLongIndexedX(true);
   CMP16();
}

static void OpC3M1()
{
   StackRelative(true);
   CMP8();
}

static void OpC3M0()
{
   StackRelative(true);
   CMP16();
}

static void OpD3M1()
{
   StackRelativeIndirectIndexed(true);
   CMP8();
}

static void OpD3M0()
{
   StackRelativeIndirectIndexed(true);
   CMP16();
}

/**********************************************************************************************/

/* CMX *************************************************************************************** */
static void OpE0X1()
{
   int32_t Int32 = (int32_t) ICPU.Registers.XL - (int32_t) *CPU.PC++;
   ICPU._Carry = Int32 >= 0;
   SetZN8((uint8_t) Int32);
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
}

static void OpE0X0()
{
#ifdef FAST_LSB_WORD_ACCESS
   int32_t Int32 = (int32_t) ICPU.Registers.X.W - (int32_t) *(uint16_t*)CPU.PC;
#else
   int32_t Int32 = (int32_t) ICPU.Registers.X.W - (int32_t)(*CPU.PC + (*(CPU.PC + 1) << 8));
#endif
   ICPU._Carry = Int32 >= 0;
   SetZN16((uint16_t) Int32);
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
}

static void OpE4X1()
{
   Direct(true);
   CMX8();
}

static void OpE4X0()
{
   Direct(true);
   CMX16();
}

static void OpECX1()
{
   Absolute(true);
   CMX8();
}

static void OpECX0()
{
   Absolute(true);
   CMX16();
}

/**********************************************************************************************/

/* CMY *************************************************************************************** */
static void OpC0X1()
{
   int32_t Int32 = (int32_t) ICPU.Registers.YL - (int32_t) *CPU.PC++;
   ICPU._Carry = Int32 >= 0;
   SetZN8((uint8_t) Int32);
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
}

static void OpC0X0()
{
#ifdef FAST_LSB_WORD_ACCESS
   int32_t Int32 = (int32_t) ICPU.Registers.Y.W - (int32_t) *(uint16_t*)CPU.PC;
#else
   int32_t Int32 = (int32_t) ICPU.Registers.Y.W - (int32_t)(*CPU.PC + (*(CPU.PC + 1) << 8));
#endif
   ICPU._Carry = Int32 >= 0;
   SetZN16((uint16_t) Int32);
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
}

static void OpC4X1()
{
   Direct(true);
   CMY8();
}

static void OpC4X0()
{
   Direct(true);
   CMY16();
}

static void OpCCX1()
{
   Absolute(true);
   CMY8();
}

static void OpCCX0()
{
   Absolute(true);
   CMY16();
}

/**********************************************************************************************/

/* DEC *************************************************************************************** */
static void Op3AM1()
{
   A_DEC8();
}

static void Op3AM0()
{
   A_DEC16();
}

static void OpC6M1()
{
   Direct(false);
   DEC8();
}

static void OpC6M0()
{
   Direct(false);
   DEC16();
}

static void OpD6M1()
{
   DirectIndexedX(false);
   DEC8();
}

static void OpD6M0()
{
   DirectIndexedX(false);
   DEC16();
}

static void OpCEM1()
{
   Absolute(false);
   DEC8();
}

static void OpCEM0()
{
   Absolute(false);
   DEC16();
}

static void OpDEM1()
{
   AbsoluteIndexedX(false);
   DEC8();
}

static void OpDEM0()
{
   AbsoluteIndexedX(false);
   DEC16();
}

/**********************************************************************************************/

/* EOR *************************************************************************************** */
static void Op49M1()
{
   ICPU.Registers.AL ^= *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
   SetZN8(ICPU.Registers.AL);
}

static void Op49M0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU.Registers.A.W ^= *(uint16_t*) CPU.PC;
#else
   ICPU.Registers.A.W ^= *CPU.PC + (*(CPU.PC + 1) << 8);
#endif
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   SetZN16(ICPU.Registers.A.W);
}

static void Op45M1()
{
   Direct(true);
   EOR8();
}

static void Op45M0()
{
   Direct(true);
   EOR16();
}

static void Op55M1()
{
   DirectIndexedX(true);
   EOR8();
}

static void Op55M0()
{
   DirectIndexedX(true);
   EOR16();
}

static void Op52M1()
{
   DirectIndirect(true);
   EOR8();
}

static void Op52M0()
{
   DirectIndirect(true);
   EOR16();
}

static void Op41M1()
{
   DirectIndexedIndirect(true);
   EOR8();
}

static void Op41M0()
{
   DirectIndexedIndirect(true);
   EOR16();
}

static void Op51M1()
{
   DirectIndirectIndexed(true);
   EOR8();
}

static void Op51M0()
{
   DirectIndirectIndexed(true);
   EOR16();
}

static void Op47M1()
{
   DirectIndirectLong(true);
   EOR8();
}

static void Op47M0()
{
   DirectIndirectLong(true);
   EOR16();
}

static void Op57M1()
{
   DirectIndirectIndexedLong(true);
   EOR8();
}

static void Op57M0()
{
   DirectIndirectIndexedLong(true);
   EOR16();
}

static void Op4DM1()
{
   Absolute(true);
   EOR8();
}

static void Op4DM0()
{
   Absolute(true);
   EOR16();
}

static void Op5DM1()
{
   AbsoluteIndexedX(true);
   EOR8();
}

static void Op5DM0()
{
   AbsoluteIndexedX(true);
   EOR16();
}

static void Op59M1()
{
   AbsoluteIndexedY(true);
   EOR8();
}

static void Op59M0()
{
   AbsoluteIndexedY(true);
   EOR16();
}

static void Op4FM1()
{
   AbsoluteLong(true);
   EOR8();
}

static void Op4FM0()
{
   AbsoluteLong(true);
   EOR16();
}

static void Op5FM1()
{
   AbsoluteLongIndexedX(true);
   EOR8();
}

static void Op5FM0()
{
   AbsoluteLongIndexedX(true);
   EOR16();
}

static void Op43M1()
{
   StackRelative(true);
   EOR8();
}

static void Op43M0()
{
   StackRelative(true);
   EOR16();
}

static void Op53M1()
{
   StackRelativeIndirectIndexed(true);
   EOR8();
}

static void Op53M0()
{
   StackRelativeIndirectIndexed(true);
   EOR16();
}

/**********************************************************************************************/

/* INC *************************************************************************************** */
static void Op1AM1()
{
   A_INC8();
}

static void Op1AM0()
{
   A_INC16();
}

static void OpE6M1()
{
   Direct(false);
   INC8();
}

static void OpE6M0()
{
   Direct(false);
   INC16();
}

static void OpF6M1()
{
   DirectIndexedX(false);
   INC8();
}

static void OpF6M0()
{
   DirectIndexedX(false);
   INC16();
}

static void OpEEM1()
{
   Absolute(false);
   INC8();
}

static void OpEEM0()
{
   Absolute(false);
   INC16();
}

static void OpFEM1()
{
   AbsoluteIndexedX(false);
   INC8();
}

static void OpFEM0()
{
   AbsoluteIndexedX(false);
   INC16();
}

/**********************************************************************************************/
/* LDA *************************************************************************************** */
static void OpA9M1()
{
   ICPU.Registers.AL = *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
   SetZN8(ICPU.Registers.AL);
}

static void OpA9M0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU.Registers.A.W = *(uint16_t*) CPU.PC;
#else
   ICPU.Registers.A.W = *CPU.PC + (*(CPU.PC + 1) << 8);
#endif

   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   SetZN16(ICPU.Registers.A.W);
}

static void OpA5M1()
{
   Direct(true);
   LDA8();
}

static void OpA5M0()
{
   Direct(true);
   LDA16();
}

static void OpB5M1()
{
   DirectIndexedX(true);
   LDA8();
}

static void OpB5M0()
{
   DirectIndexedX(true);
   LDA16();
}

static void OpB2M1()
{
   DirectIndirect(true);
   LDA8();
}

static void OpB2M0()
{
   DirectIndirect(true);
   LDA16();
}

static void OpA1M1()
{
   DirectIndexedIndirect(true);
   LDA8();
}

static void OpA1M0()
{
   DirectIndexedIndirect(true);
   LDA16();
}

static void OpB1M1()
{
   DirectIndirectIndexed(true);
   LDA8();
}

static void OpB1M0()
{
   DirectIndirectIndexed(true);
   LDA16();
}

static void OpA7M1()
{
   DirectIndirectLong(true);
   LDA8();
}

static void OpA7M0()
{
   DirectIndirectLong(true);
   LDA16();
}

static void OpB7M1()
{
   DirectIndirectIndexedLong(true);
   LDA8();
}

static void OpB7M0()
{
   DirectIndirectIndexedLong(true);
   LDA16();
}

static void OpADM1()
{
   Absolute(true);
   LDA8();
}

static void OpADM0()
{
   Absolute(true);
   LDA16();
}

static void OpBDM1()
{
   AbsoluteIndexedX(true);
   LDA8();
}

static void OpBDM0()
{
   AbsoluteIndexedX(true);
   LDA16();
}

static void OpB9M1()
{
   AbsoluteIndexedY(true);
   LDA8();
}

static void OpB9M0()
{
   AbsoluteIndexedY(true);
   LDA16();
}

static void OpAFM1()
{
   AbsoluteLong(true);
   LDA8();
}

static void OpAFM0()
{
   AbsoluteLong(true);
   LDA16();
}

static void OpBFM1()
{
   AbsoluteLongIndexedX(true);
   LDA8();
}

static void OpBFM0()
{
   AbsoluteLongIndexedX(true);
   LDA16();
}

static void OpA3M1()
{
   StackRelative(true);
   LDA8();
}

static void OpA3M0()
{
   StackRelative(true);
   LDA16();
}

static void OpB3M1()
{
   StackRelativeIndirectIndexed(true);
   LDA8();
}

static void OpB3M0()
{
   StackRelativeIndirectIndexed(true);
   LDA16();
}

/**********************************************************************************************/

/* LDX *************************************************************************************** */
static void OpA2X1()
{
   ICPU.Registers.XL = *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
   SetZN8(ICPU.Registers.XL);
}

static void OpA2X0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU.Registers.X.W = *(uint16_t*) CPU.PC;
#else
   ICPU.Registers.X.W = *CPU.PC + (*(CPU.PC + 1) << 8);
#endif
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   SetZN16(ICPU.Registers.X.W);
}

static void OpA6X1()
{
   Direct(true);
   LDX8();
}

static void OpA6X0()
{
   Direct(true);
   LDX16();
}

static void OpB6X1()
{
   DirectIndexedY(true);
   LDX8();
}

static void OpB6X0()
{
   DirectIndexedY(true);
   LDX16();
}

static void OpAEX1()
{
   Absolute(true);
   LDX8();
}

static void OpAEX0()
{
   Absolute(true);
   LDX16();
}

static void OpBEX1()
{
   AbsoluteIndexedY(true);
   LDX8();
}

static void OpBEX0()
{
   AbsoluteIndexedY(true);
   LDX16();
}
/**********************************************************************************************/

/* LDY *************************************************************************************** */
static void OpA0X1()
{
   ICPU.Registers.YL = *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
   SetZN8(ICPU.Registers.YL);
}

static void OpA0X0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU.Registers.Y.W = *(uint16_t*) CPU.PC;
#else
   ICPU.Registers.Y.W = *CPU.PC + (*(CPU.PC + 1) << 8);
#endif

   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   SetZN16(ICPU.Registers.Y.W);
}

static void OpA4X1()
{
   Direct(true);
   LDY8();
}

static void OpA4X0()
{
   Direct(true);
   LDY16();
}

static void OpB4X1()
{
   DirectIndexedX(true);
   LDY8();
}

static void OpB4X0()
{
   DirectIndexedX(true);
   LDY16();
}

static void OpACX1()
{
   Absolute(true);
   LDY8();
}

static void OpACX0()
{
   Absolute(true);
   LDY16();
}

static void OpBCX1()
{
   AbsoluteIndexedX(true);
   LDY8();
}

static void OpBCX0()
{
   AbsoluteIndexedX(true);
   LDY16();
}
/**********************************************************************************************/

/* LSR *************************************************************************************** */
static void Op4AM1()
{
   A_LSR8();
}

static void Op4AM0()
{
   A_LSR16();
}

static void Op46M1()
{
   Direct(false);
   LSR8();
}

static void Op46M0()
{
   Direct(false);
   LSR16();
}

static void Op56M1()
{
   DirectIndexedX(false);
   LSR8();
}

static void Op56M0()
{
   DirectIndexedX(false);
   LSR16();
}

static void Op4EM1()
{
   Absolute(false);
   LSR8();
}

static void Op4EM0()
{
   Absolute(false);
   LSR16();
}

static void Op5EM1()
{
   AbsoluteIndexedX(false);
   LSR8();
}

static void Op5EM0()
{
   AbsoluteIndexedX(false);
   LSR16();
}

/**********************************************************************************************/

/* ORA *************************************************************************************** */
static void Op09M1()
{
   ICPU.Registers.AL |= *CPU.PC++;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed;
#endif
   SetZN8(ICPU.Registers.AL);
}

static void Op09M0()
{
#ifdef FAST_LSB_WORD_ACCESS
   ICPU.Registers.A.W |= *(uint16_t*) CPU.PC;
#else
   ICPU.Registers.A.W |= *CPU.PC + (*(CPU.PC + 1) << 8);
#endif
   CPU.PC += 2;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2;
#endif
   SetZN16(ICPU.Registers.A.W);
}

static void Op05M1()
{
   Direct(true);
   ORA8();
}

static void Op05M0()
{
   Direct(true);
   ORA16();
}

static void Op15M1()
{
   DirectIndexedX(true);
   ORA8();
}

static void Op15M0()
{
   DirectIndexedX(true);
   ORA16();
}

static void Op12M1()
{
   DirectIndirect(true);
   ORA8();
}

static void Op12M0()
{
   DirectIndirect(true);
   ORA16();
}

static void Op01M1()
{
   DirectIndexedIndirect(true);
   ORA8();
}

static void Op01M0()
{
   DirectIndexedIndirect(true);
   ORA16();
}

static void Op11M1()
{
   DirectIndirectIndexed(true);
   ORA8();
}

static void Op11M0()
{
   DirectIndirectIndexed(true);
   ORA16();
}

static void Op07M1()
{
   DirectIndirectLong(true);
   ORA8();
}

static void Op07M0()
{
   DirectIndirectLong(true);
   ORA16();
}

static void Op17M1()
{
   DirectIndirectIndexedLong(true);
   ORA8();
}

static void Op17M0()
{
   DirectIndirectIndexedLong(true);
   ORA16();
}

static void Op0DM1()
{
   Absolute(true);
   ORA8();
}

static void Op0DM0()
{
   Absolute(true);
   ORA16();
}

static void Op1DM1()
{
   AbsoluteIndexedX(true);
   ORA8();
}

static void Op1DM0()
{
   AbsoluteIndexedX(true);
   ORA16();
}

static void Op19M1()
{
   AbsoluteIndexedY(true);
   ORA8();
}

static void Op19M0()
{
   AbsoluteIndexedY(true);
   ORA16();
}

static void Op0FM1()
{
   AbsoluteLong(true);
   ORA8();
}

static void Op0FM0()
{
   AbsoluteLong(true);
   ORA16();
}

static void Op1FM1()
{
   AbsoluteLongIndexedX(true);
   ORA8();
}

static void Op1FM0()
{
   AbsoluteLongIndexedX(true);
   ORA16();
}

static void Op03M1()
{
   StackRelative(true);
   ORA8();
}

static void Op03M0()
{
   StackRelative(true);
   ORA16();
}

static void Op13M1()
{
   StackRelativeIndirectIndexed(true);
   ORA8();
}

static void Op13M0()
{
   StackRelativeIndirectIndexed(true);
   ORA16();
}

/**********************************************************************************************/

/* ROL *************************************************************************************** */
static void Op2AM1()
{
   A_ROL8();
}

static void Op2AM0()
{
   A_ROL16();
}

static void Op26M1()
{
   Direct(false);
   ROL8();
}

static void Op26M0()
{
   Direct(false);
   ROL16();
}

static void Op36M1()
{
   DirectIndexedX(false);
   ROL8();
}

static void Op36M0()
{
   DirectIndexedX(false);
   ROL16();
}

static void Op2EM1()
{
   Absolute(false);
   ROL8();
}

static void Op2EM0()
{
   Absolute(false);
   ROL16();
}

static void Op3EM1()
{
   AbsoluteIndexedX(false);
   ROL8();
}

static void Op3EM0()
{
   AbsoluteIndexedX(false);
   ROL16();
}
/**********************************************************************************************/

/* ROR *************************************************************************************** */
static void Op6AM1()
{
   A_ROR8();
}

static void Op6AM0()
{
   A_ROR16();
}

static void Op66M1()
{
   Direct(false);
   ROR8();
}

static void Op66M0()
{
   Direct(false);
   ROR16();
}

static void Op76M1()
{
   DirectIndexedX(false);
   ROR8();
}

static void Op76M0()
{
   DirectIndexedX(false);
   ROR16();
}

static void Op6EM1()
{
   Absolute(false);
   ROR8();
}

static void Op6EM0()
{
   Absolute(false);
   ROR16();
}

static void Op7EM1()
{
   AbsoluteIndexedX(false);
   ROR8();
}

static void Op7EM0()
{
   AbsoluteIndexedX(false);
   ROR16();
}
/**********************************************************************************************/

/* SBC *************************************************************************************** */
static void OpE9M1()
{
   Immediate8();
   SBC8();
}

static void OpE9M0()
{
   Immediate16();
   SBC16();
}

static void OpE5M1()
{
   Direct(true);
   SBC8();
}

static void OpE5M0()
{
   Direct(true);
   SBC16();
}

static void OpF5M1()
{
   DirectIndexedX(true);
   SBC8();
}

static void OpF5M0()
{
   DirectIndexedX(true);
   SBC16();
}

static void OpF2M1()
{
   DirectIndirect(true);
   SBC8();
}

static void OpF2M0()
{
   DirectIndirect(true);
   SBC16();
}

static void OpE1M1()
{
   DirectIndexedIndirect(true);
   SBC8();
}

static void OpE1M0()
{
   DirectIndexedIndirect(true);
   SBC16();
}

static void OpF1M1()
{
   DirectIndirectIndexed(true);
   SBC8();
}

static void OpF1M0()
{
   DirectIndirectIndexed(true);
   SBC16();
}

static void OpE7M1()
{
   DirectIndirectLong(true);
   SBC8();
}

static void OpE7M0()
{
   DirectIndirectLong(true);
   SBC16();
}

static void OpF7M1()
{
   DirectIndirectIndexedLong(true);
   SBC8();
}

static void OpF7M0()
{
   DirectIndirectIndexedLong(true);
   SBC16();
}

static void OpEDM1()
{
   Absolute(true);
   SBC8();
}

static void OpEDM0()
{
   Absolute(true);
   SBC16();
}

static void OpFDM1()
{
   AbsoluteIndexedX(true);
   SBC8();
}

static void OpFDM0()
{
   AbsoluteIndexedX(true);
   SBC16();
}

static void OpF9M1()
{
   AbsoluteIndexedY(true);
   SBC8();
}

static void OpF9M0()
{
   AbsoluteIndexedY(true);
   SBC16();
}

static void OpEFM1()
{
   AbsoluteLong(true);
   SBC8();
}

static void OpEFM0()
{
   AbsoluteLong(true);
   SBC16();
}

static void OpFFM1()
{
   AbsoluteLongIndexedX(true);
   SBC8();
}

static void OpFFM0()
{
   AbsoluteLongIndexedX(true);
   SBC16();
}

static void OpE3M1()
{
   StackRelative(true);
   SBC8();
}

static void OpE3M0()
{
   StackRelative(true);
   SBC16();
}

static void OpF3M1()
{
   StackRelativeIndirectIndexed(true);
   SBC8();
}

static void OpF3M0()
{
   StackRelativeIndirectIndexed(true);
   SBC16();
}
/**********************************************************************************************/

/* STA *************************************************************************************** */
static void Op85M1()
{
   Direct(false);
   STA8();
}

static void Op85M0()
{
   Direct(false);
   STA16();
}

static void Op95M1()
{
   DirectIndexedX(false);
   STA8();
}

static void Op95M0()
{
   DirectIndexedX(false);
   STA16();
}

static void Op92M1()
{
   DirectIndirect(false);
   STA8();
}

static void Op92M0()
{
   DirectIndirect(false);
   STA16();
}

static void Op81M1()
{
   DirectIndexedIndirect(false);
   STA8();
#ifdef noVAR_CYCLES
   if (CheckIndex())
      CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op81M0()
{
   DirectIndexedIndirect(false);
   STA16();
#ifdef noVAR_CYCLES
   if (CheckIndex())
      CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op91M1()
{
   DirectIndirectIndexed(false);
   STA8();
}

static void Op91M0()
{
   DirectIndirectIndexed(false);
   STA16();
}

static void Op87M1()
{
   DirectIndirectLong(false);
   STA8();
}

static void Op87M0()
{
   DirectIndirectLong(false);
   STA16();
}

static void Op97M1()
{
   DirectIndirectIndexedLong(false);
   STA8();
}

static void Op97M0()
{
   DirectIndirectIndexedLong(false);
   STA16();
}

static void Op8DM1()
{
   Absolute(false);
   STA8();
}

static void Op8DM0()
{
   Absolute(false);
   STA16();
}

static void Op9DM1()
{
   AbsoluteIndexedX(false);
   STA8();
}

static void Op9DM0()
{
   AbsoluteIndexedX(false);
   STA16();
}

static void Op99M1()
{
   AbsoluteIndexedY(false);
   STA8();
}

static void Op99M0()
{
   AbsoluteIndexedY(false);
   STA16();
}

static void Op8FM1()
{
   AbsoluteLong(false);
   STA8();
}

static void Op8FM0()
{
   AbsoluteLong(false);
   STA16();
}

static void Op9FM1()
{
   AbsoluteLongIndexedX(false);
   STA8();
}

static void Op9FM0()
{
   AbsoluteLongIndexedX(false);
   STA16();
}

static void Op83M1()
{
   StackRelative(false);
   STA8();
}

static void Op83M0()
{
   StackRelative(false);
   STA16();
}

static void Op93M1()
{
   StackRelativeIndirectIndexed(false);
   STA8();
}

static void Op93M0()
{
   StackRelativeIndirectIndexed(false);
   STA16();
}
/**********************************************************************************************/

/* STX *************************************************************************************** */
static void Op86X1()
{
   Direct(false);
   STX8();
}

static void Op86X0()
{
   Direct(false);
   STX16();
}

static void Op96X1()
{
   DirectIndexedY(false);
   STX8();
}

static void Op96X0()
{
   DirectIndexedY(false);
   STX16();
}

static void Op8EX1()
{
   Absolute(false);
   STX8();
}

static void Op8EX0()
{
   Absolute(false);
   STX16();
}
/**********************************************************************************************/

/* STY *************************************************************************************** */
static void Op84X1()
{
   Direct(false);
   STY8();
}

static void Op84X0()
{
   Direct(false);
   STY16();
}

static void Op94X1()
{
   DirectIndexedX(false);
   STY8();
}

static void Op94X0()
{
   DirectIndexedX(false);
   STY16();
}

static void Op8CX1()
{
   Absolute(false);
   STY8();
}

static void Op8CX0()
{
   Absolute(false);
   STY16();
}
/**********************************************************************************************/

/* STZ *************************************************************************************** */
static void Op64M1()
{
   Direct(false);
   STZ8();
}

static void Op64M0()
{
   Direct(false);
   STZ16();
}

static void Op74M1()
{
   DirectIndexedX(false);
   STZ8();
}

static void Op74M0()
{
   DirectIndexedX(false);
   STZ16();
}

static void Op9CM1()
{
   Absolute(false);
   STZ8();
}

static void Op9CM0()
{
   Absolute(false);
   STZ16();
}

static void Op9EM1()
{
   AbsoluteIndexedX(false);
   STZ8();
}

static void Op9EM0()
{
   AbsoluteIndexedX(false);
   STZ16();
}

/**********************************************************************************************/

/* TRB *************************************************************************************** */
static void Op14M1()
{
   Direct(false);
   TRB8();
}

static void Op14M0()
{
   Direct(false);
   TRB16();
}

static void Op1CM1()
{
   Absolute(false);
   TRB8();
}

static void Op1CM0()
{
   Absolute(false);
   TRB16();
}
/**********************************************************************************************/

/* TSB *************************************************************************************** */
static void Op04M1()
{
   Direct(false);
   TSB8();
}

static void Op04M0()
{
   Direct(false);
   TSB16();
}

static void Op0CM1()
{
   Absolute(false);
   TSB8();
}

static void Op0CM0()
{
   Absolute(false);
   TSB16();
}

/**********************************************************************************************/

/* Branch Instructions *********************************************************************** */
#ifndef SA1_OPCODES
#define BranchCheck0()\
   if(CPU.BranchSkip)\
   {\
      CPU.BranchSkip = false;\
      if (!Settings.SoundSkipMethod)\
         if (CPU.PC - CPU.PCBase > OpAddress)\
            return;\
   }

#define BranchCheck1()\
   if(CPU.BranchSkip)\
   {\
      CPU.BranchSkip = false;\
      if (!Settings.SoundSkipMethod)\
      {\
         if( CPU.PC - CPU.PCBase > OpAddress)\
            return;\
      }\
      else if (Settings.SoundSkipMethod == 1)\
         return;\
      if (Settings.SoundSkipMethod == 3)\
      {\
         if (CPU.PC - CPU.PCBase > OpAddress)\
            return;\
         else\
            CPU.PC = CPU.PCBase + OpAddress;\
      }\
   }

#define BranchCheck2()\
   if(CPU.BranchSkip)\
   {\
      CPU.BranchSkip = false;\
      if (!Settings.SoundSkipMethod)\
      {\
         if( CPU.PC - CPU.PCBase > OpAddress)\
            return;\
      }\
      else if (Settings.SoundSkipMethod == 1)\
         CPU.PC = CPU.PCBase + OpAddress;\
      if (Settings.SoundSkipMethod == 3)\
      {\
         if (CPU.PC - CPU.PCBase > OpAddress)\
            return;\
         else\
            CPU.PC = CPU.PCBase + OpAddress;\
      }\
   }
#else
#define BranchCheck0()
#define BranchCheck1()
#define BranchCheck2()
#endif

#ifdef CPU_SHUTDOWN
#ifndef SA1_OPCODES
static inline void CPUShutdown()
{
   if (Settings.Shutdown && CPU.PC == CPU.WaitAddress)
   {
      // Don't skip cycles with a pending NMI or IRQ - could cause delayed
      // interrupt. Interrupts are delayed for a few cycles already, but
      // the delay could allow the shutdown code to cycle skip again.
      // Was causing screen flashing on Top Gear 3000.

      if (CPU.WaitCounter == 0 && !(CPU.Flags & (IRQ_PENDING_FLAG | NMI_FLAG)))
      {
         CPU.WaitAddress = NULL;
#ifndef USE_BLARGG_APU
         CPU.Cycles = CPU.NextEvent;
         if (IAPU.APUExecuting)
         {
            ICPU.CPUExecuting = false;
            do
            {
               APU_EXECUTE1();
            }
            while (APU.Cycles < CPU.NextEvent);
            ICPU.CPUExecuting = true;
         }
#endif
      }
      else if (CPU.WaitCounter >= 2)
         CPU.WaitCounter = 1;
      else
         CPU.WaitCounter--;
   }
}
#else
static inline void CPUShutdown()
{
   if (Settings.Shutdown && CPU.PC == CPU.WaitAddress)
   {
      if (CPU.WaitCounter >= 1)
      {
         SA1.Executing = false;
         SA1.CPUExecuting = false;
      }
      else
         CPU.WaitCounter++;
   }
}
#endif
#else
#define CPUShutdown()
#endif

// From the speed-hacks branch of CatSFC
static inline void ForceShutdown()
{
#ifdef CPU_SHUTDOWN
#ifdef VAR_CYCLES
   CPU.WaitAddress = NULL;
#ifndef USE_BLARGG_APU
   CPU.Cycles = CPU.NextEvent;
   if (IAPU.APUExecuting)
   {
      ICPU.CPUExecuting = false;
      do
      {
         APU_EXECUTE1();
      }
      while (APU.Cycles < CPU.NextEvent);
      ICPU.CPUExecuting = true;
   }
#endif
#else
   SA1.Executing = false;
   SA1.CPUExecuting = false;
#endif
#endif
}

/* BCC */
static void Op90()
{
   Relative();
   BranchCheck0();
   if (!CheckCarry())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BCS */
static void OpB0()
{
   Relative();
   BranchCheck0();
   if (CheckCarry())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BEQ */
static void OpF0()
{
   Relative();
   BranchCheck2();
   if (CheckZero())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BMI */
static void Op30()
{
   Relative();
   BranchCheck1();
   if (CheckNegative())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BNE */
static void OpD0()
{
   Relative();
   BranchCheck1();
   if (!CheckZero())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BPL */
static void Op10()
{
   Relative();
   BranchCheck1();
   if (!CheckNegative())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BRA */
static void Op80()
{
   Relative();
   CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   CPUShutdown();
}

/* BVC */
static void Op50()
{
   Relative();
   BranchCheck0();
   if (!CheckOverflow())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}

/* BVS */
static void Op70()
{
   Relative();
   BranchCheck0();
   if (CheckOverflow())
   {
      CPU.PC = CPU.PCBase + OpAddress;
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
      CPUShutdown();
   }
}
/**********************************************************************************************/

/* ClearFlag Instructions ******************************************************************** */
/* CLC */
static void Op18()
{
   ClearCarry();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

/* CLD */
static void OpD8()
{
   ClearDecimal();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

/* CLI */
static void Op58()
{
   ClearIRQ();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   /*    CHECK_FOR_IRQ(); */
}

/* CLV */
static void OpB8()
{
   ClearOverflow();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}
/**********************************************************************************************/

/* DEX/DEY *********************************************************************************** */
static void OpCAX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.XL--;
   SetZN8(ICPU.Registers.XL);
}

static void OpCAX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.X.W--;
   SetZN16(ICPU.Registers.X.W);
}

static void Op88X1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.YL--;
   SetZN8(ICPU.Registers.YL);
}

static void Op88X0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.Y.W--;
   SetZN16(ICPU.Registers.Y.W);
}
/**********************************************************************************************/

/* INX/INY *********************************************************************************** */
static void OpE8X1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.XL++;
   SetZN8(ICPU.Registers.XL);
}

static void OpE8X0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.X.W++;
   SetZN16(ICPU.Registers.X.W);
}

static void OpC8X1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.YL++;
   SetZN8(ICPU.Registers.YL);
}

static void OpC8X0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
   CPU.WaitAddress = NULL;
#endif
   ICPU.Registers.Y.W++;
   SetZN16(ICPU.Registers.Y.W);
}

/**********************************************************************************************/

/* NOP *************************************************************************************** */
static void OpEA()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}
/**********************************************************************************************/

/* PUSH Instructions ************************************************************************* */
#define PushB(b)\
   S9xSetByte(b, ICPU.Registers.S.W--);

#define PushBE(b)\
   PushB(b);\
   ICPU.Registers.SH = 0x01;

#define PushW(w)\
   S9xSetByte((w) >> 8, ICPU.Registers.S.W);\
   S9xSetByte((w) & 0xff, (ICPU.Registers.S.W - 1) & 0xffff);\
   ICPU.Registers.S.W -= 2;

#define PushWE(w)\
   PushW(w); \
   ICPU.Registers.SH = 0x01;

//PEA NL
static void OpF4E1()
{
   Absolute(false);
   PushWE((uint16_t)OpAddress);
}

static void OpF4()
{
   Absolute(false);
   PushW((uint16_t)OpAddress);
}

//PEI NL
static void OpD4E1()
{
   DirectIndirect(false);
   PushWE((uint16_t)OpAddress);
}

static void OpD4()
{
   DirectIndirect(false);
   PushW((uint16_t)OpAddress);
}

//PER NL
static void Op62E1()
{
   RelativeLong(false);
   PushWE((uint16_t)OpAddress);
}

static void Op62()
{
   RelativeLong(false);
   PushW((uint16_t)OpAddress);
}

//PHA
static void Op48E1()
{
   PushBE(ICPU.Registers.AL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op48M1()
{
   PushB(ICPU.Registers.AL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op48M0()
{
   PushW(ICPU.Registers.A.W);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//PHB
static void Op8BE1()
{
   PushBE(ICPU.Registers.DB);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op8B()
{
   PushB(ICPU.Registers.DB);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//PHD NL
static void Op0BE1()
{
   PushWE(ICPU.Registers.D.W);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op0B()
{
   PushW(ICPU.Registers.D.W);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//PHK
static void Op4BE1()
{
   PushBE(ICPU.Registers.PB);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op4B()
{
   PushB(ICPU.Registers.PB);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//PHP
static void Op08E1()
{
   S9xPackStatus();
   PushBE(ICPU.Registers.PL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op08()
{
   S9xPackStatus();
   PushB(ICPU.Registers.PL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//PHX
static void OpDAE1()
{
   PushBE(ICPU.Registers.XL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void OpDAX1()
{
   PushB(ICPU.Registers.XL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void OpDAX0()
{
   PushW(ICPU.Registers.X.W);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//PHY
static void Op5AE1()
{
   PushBE(ICPU.Registers.YL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op5AX1()
{
   PushB(ICPU.Registers.YL);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op5AX0()
{
   PushW(ICPU.Registers.Y.W);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}
/**********************************************************************************************/

/* PULL Instructions ************************************************************************* */
#define PullB(b)\
   b = S9xGetByte (++ICPU.Registers.S.W);

#define PullBE(b)\
   PullB(b);\
   ICPU.Registers.SH = 0x01;

#define PullW(w)\
   w = S9xGetByte(++ICPU.Registers.S.W);\
   w |= (S9xGetByte(++ICPU.Registers.S.W) << 8);

#define PullWE(w)\
   PullW(w);\
   ICPU.Registers.SH = 0x01;

//PLA
static void Op68E1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullBE(ICPU.Registers.AL);
   SetZN8(ICPU.Registers.AL);
}

static void Op68M1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullB(ICPU.Registers.AL);
   SetZN8(ICPU.Registers.AL);
}

static void Op68M0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullW(ICPU.Registers.A.W);
   SetZN16(ICPU.Registers.A.W);
}

//PLB
static void OpABE1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullBE(ICPU.Registers.DB);
   SetZN8(ICPU.Registers.DB);
   ICPU.ShiftedDB = ICPU.Registers.DB << 16;
}

static void OpAB()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullB(ICPU.Registers.DB);
   SetZN8(ICPU.Registers.DB);
   ICPU.ShiftedDB = ICPU.Registers.DB << 16;
}

/* PHP */
//PLD NL
static void Op2BE1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullWE(ICPU.Registers.D.W);
   SetZN16(ICPU.Registers.D.W);
}

static void Op2B()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullW(ICPU.Registers.D.W);
   SetZN16(ICPU.Registers.D.W);
}

/* PLP */
static void Op28E1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullBE(ICPU.Registers.PL);
   S9xUnpackStatus();

   if (CheckIndex())
   {
      ICPU.Registers.XH = 0;
      ICPU.Registers.YH = 0;
   }
   S9xFixCycles();
}

static void Op28()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullB(ICPU.Registers.PL);
   S9xUnpackStatus();

   if (CheckIndex())
   {
      ICPU.Registers.XH = 0;
      ICPU.Registers.YH = 0;
   }
   S9xFixCycles();
}

//PLX
static void OpFAE1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullBE(ICPU.Registers.XL);
   SetZN8(ICPU.Registers.XL);
}

static void OpFAX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullB(ICPU.Registers.XL);
   SetZN8(ICPU.Registers.XL);
}

static void OpFAX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullW(ICPU.Registers.X.W);
   SetZN16(ICPU.Registers.X.W);
}

//PLY
static void Op7AE1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullBE(ICPU.Registers.YL);
   SetZN8(ICPU.Registers.YL);
}

static void Op7AX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullB(ICPU.Registers.YL);
   SetZN8(ICPU.Registers.YL);
}

static void Op7AX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   PullW(ICPU.Registers.Y.W);
   SetZN16(ICPU.Registers.Y.W);
}

/**********************************************************************************************/

/* SetFlag Instructions ********************************************************************** */
/* SEC */
static void Op38()
{
   SetCarry();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

/* SED */
static void OpF8()
{
   SetDecimal();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   missing.decimal_mode = 1;
}

/* SEI */
static void Op78()
{
   SetIRQ();
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}
/**********************************************************************************************/

/* Transfer Instructions ********************************************************************* */
/* TAX8 */
static void OpAAX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.XL = ICPU.Registers.AL;
   SetZN8(ICPU.Registers.XL);
}

/* TAX16 */
static void OpAAX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.X.W = ICPU.Registers.A.W;
   SetZN16(ICPU.Registers.X.W);
}

/* TAY8 */
static void OpA8X1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.YL = ICPU.Registers.AL;
   SetZN8(ICPU.Registers.YL);
}

/* TAY16 */
static void OpA8X0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.Y.W = ICPU.Registers.A.W;
   SetZN16(ICPU.Registers.Y.W);
}

static void Op5B()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.D.W = ICPU.Registers.A.W;
   SetZN16(ICPU.Registers.D.W);
}

static void Op1B()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.S.W = ICPU.Registers.A.W;
   if (CheckEmulation())
      ICPU.Registers.SH = 1;
}

static void Op7B()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.A.W = ICPU.Registers.D.W;
   SetZN16(ICPU.Registers.A.W);
}

static void Op3B()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.A.W = ICPU.Registers.S.W;
   SetZN16(ICPU.Registers.A.W);
}

static void OpBAX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.XL = ICPU.Registers.SL;
   SetZN8(ICPU.Registers.XL);
}

static void OpBAX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.X.W = ICPU.Registers.S.W;
   SetZN16(ICPU.Registers.X.W);
}

static void Op8AM1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.AL = ICPU.Registers.XL;
   SetZN8(ICPU.Registers.AL);
}

static void Op8AM0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.A.W = ICPU.Registers.X.W;
   SetZN16(ICPU.Registers.A.W);
}

static void Op9A()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.S.W = ICPU.Registers.X.W;
   if (CheckEmulation())
      ICPU.Registers.SH = 1;
}

static void Op9BX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.YL = ICPU.Registers.XL;
   SetZN8(ICPU.Registers.YL);
}

static void Op9BX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.Y.W = ICPU.Registers.X.W;
   SetZN16(ICPU.Registers.Y.W);
}

static void Op98M1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.AL = ICPU.Registers.YL;
   SetZN8(ICPU.Registers.AL);
}

static void Op98M0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.A.W = ICPU.Registers.Y.W;
   SetZN16(ICPU.Registers.A.W);
}

static void OpBBX1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.XL = ICPU.Registers.YL;
   SetZN8(ICPU.Registers.XL);
}

static void OpBBX0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   ICPU.Registers.X.W = ICPU.Registers.Y.W;
   SetZN16(ICPU.Registers.X.W);
}

/**********************************************************************************************/

/* XCE *************************************************************************************** */
static void OpFB()
{
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
   uint8_t A1 = ICPU._Carry;
   uint8_t A2 = ICPU.Registers.PH;
   ICPU._Carry = A2 & 1;
   ICPU.Registers.PH = A1;

   if (CheckEmulation())
   {
      SetFlags(MemoryFlag | IndexFlag);
      ICPU.Registers.SH = 1;
      missing.emulate6502 = 1;
   }
   if (CheckIndex())
   {
      ICPU.Registers.XH = 0;
      ICPU.Registers.YH = 0;
   }
   S9xFixCycles();
}
/**********************************************************************************************/

/* BRK *************************************************************************************** */
static void Op00()
{
#ifndef SA1_OPCODES
   CPU.BRKTriggered = true;
#endif
   if (!CheckEmulation())
   {
      PushB(ICPU.Registers.PB);
      PushW(CPU.PC - CPU.PCBase + 1);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
      S9xSetPCBase(S9xGetWord(0xFFE6));
#ifndef SA1_OPCODES
      CPU.Cycles += TWO_CYCLES;
#endif
   }
   else
   {
      PushW(CPU.PC - CPU.PCBase);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
      S9xSetPCBase(S9xGetWord(0xFFFE));
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
   }
}
/**********************************************************************************************/

/* BRL ************************************************************************************** */
static void Op82()
{
   RelativeLong();
   S9xSetPCBase(ICPU.ShiftedPB + OpAddress);
}
/**********************************************************************************************/

/* IRQ *************************************************************************************** */
void S9xOpcode_IRQ()
{
   if (!CheckEmulation())
   {
      PushB(ICPU.Registers.PB);
      PushW(CPU.PC - CPU.PCBase);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
      S9xSA1SetPCBase(Memory.FillRAM [0x2207] |
                      (Memory.FillRAM [0x2208] << 8));
#else
      if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x40))
         S9xSetPCBase(Memory.FillRAM [0x220e] |
                      (Memory.FillRAM [0x220f] << 8));
      else
         S9xSetPCBase(S9xGetWord(0xFFEE));
#endif
#ifndef SA1_OPCODES
      CPU.Cycles += TWO_CYCLES;
#endif
   }
   else
   {
      PushW(CPU.PC - CPU.PCBase);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
      S9xSA1SetPCBase(Memory.FillRAM [0x2207] |
                      (Memory.FillRAM [0x2208] << 8));
#else
      if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x40))
         S9xSetPCBase(Memory.FillRAM [0x220e] |
                      (Memory.FillRAM [0x220f] << 8));
      else
         S9xSetPCBase(S9xGetWord(0xFFFE));
#endif
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
   }
}

/**********************************************************************************************/

/* NMI *************************************************************************************** */
void S9xOpcode_NMI()
{
   if (!CheckEmulation())
   {
      PushB(ICPU.Registers.PB);
      PushW(CPU.PC - CPU.PCBase);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
      S9xSA1SetPCBase(Memory.FillRAM [0x2205] |
                      (Memory.FillRAM [0x2206] << 8));
#else
      if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x20))
         S9xSetPCBase(Memory.FillRAM [0x220c] |
                      (Memory.FillRAM [0x220d] << 8));
      else
         S9xSetPCBase(S9xGetWord(0xFFEA));
#endif
#ifndef SA1_OPCODES
      CPU.Cycles += TWO_CYCLES;
#endif
   }
   else
   {
      PushW(CPU.PC - CPU.PCBase);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
#ifdef SA1_OPCODES
      S9xSA1SetPCBase(Memory.FillRAM [0x2205] |
                      (Memory.FillRAM [0x2206] << 8));
#else
      if (Settings.SA1 && (Memory.FillRAM [0x2209] & 0x20))
         S9xSetPCBase(Memory.FillRAM [0x220c] |
                      (Memory.FillRAM [0x220d] << 8));
      else
         S9xSetPCBase(S9xGetWord(0xFFFA));
#endif
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
   }
}
/**********************************************************************************************/

/* COP *************************************************************************************** */
static void Op02()
{
   if (!CheckEmulation())
   {
      PushB(ICPU.Registers.PB);
      PushW(CPU.PC - CPU.PCBase + 1);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
      S9xSetPCBase(S9xGetWord(0xFFE4));
#ifndef SA1_OPCODES
      CPU.Cycles += TWO_CYCLES;
#endif
   }
   else
   {
      PushW(CPU.PC - CPU.PCBase);
      S9xPackStatus();
      PushB(ICPU.Registers.PL);
      OpenBus = ICPU.Registers.PL;
      ClearDecimal();
      SetIRQ();

      ICPU.Registers.PB = 0;
      ICPU.ShiftedPB = 0;
      S9xSetPCBase(S9xGetWord(0xFFF4));
#ifndef SA1_OPCODES
      CPU.Cycles += ONE_CYCLE;
#endif
   }
}
/**********************************************************************************************/

/* JML *************************************************************************************** */
static void OpDC()
{
   AbsoluteIndirectLong(false);
   ICPU.Registers.PB = (uint8_t)(OpAddress >> 16);
   ICPU.ShiftedPB = OpAddress & 0xff0000;
   S9xSetPCBase(OpAddress);
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
}

static void Op5C()
{
   AbsoluteLong(false);
   ICPU.Registers.PB = (uint8_t)(OpAddress >> 16);
   ICPU.ShiftedPB = OpAddress & 0xff0000;
   S9xSetPCBase(OpAddress);
}
/**********************************************************************************************/

/* JMP *************************************************************************************** */
static void Op4C()
{
   Absolute(false);
   S9xSetPCBase(ICPU.ShiftedPB + (OpAddress & 0xffff));
#if defined(CPU_SHUTDOWN) && defined(SA1_OPCODES)
   CPUShutdown();
#endif
}

static void Op6C()
{
   AbsoluteIndirect(false);
   S9xSetPCBase(ICPU.ShiftedPB + (OpAddress & 0xffff));
}

static void Op7C()
{
   AbsoluteIndexedIndirect(false);
   S9xSetPCBase(ICPU.ShiftedPB + OpAddress);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}
/**********************************************************************************************/

/* JSL/RTL *********************************************************************************** */
static void Op22E1()
{
   AbsoluteLong(false);
   PushB(ICPU.Registers.PB);
   PushWE(CPU.PC - CPU.PCBase - 1);
   ICPU.Registers.PB = (uint8_t)(OpAddress >> 16);
   ICPU.ShiftedPB = OpAddress & 0xff0000;
   S9xSetPCBase(OpAddress);
}

static void Op22()
{
   AbsoluteLong(false);
   PushB(ICPU.Registers.PB);
   PushW(CPU.PC - CPU.PCBase - 1);
   ICPU.Registers.PB = (uint8_t)(OpAddress >> 16);
   ICPU.ShiftedPB = OpAddress & 0xff0000;
   S9xSetPCBase(OpAddress);
}

static void Op6BE1()
{
   PullWE(ICPU.Registers.PC);
   PullB(ICPU.Registers.PB);
   ICPU.ShiftedPB = ICPU.Registers.PB << 16;
   S9xSetPCBase(ICPU.ShiftedPB + ((ICPU.Registers.PC + 1) & 0xffff));
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
}

static void Op6B()
{
   PullW(ICPU.Registers.PC);
   PullB(ICPU.Registers.PB);
   ICPU.ShiftedPB = ICPU.Registers.PB << 16;
   S9xSetPCBase(ICPU.ShiftedPB + ((ICPU.Registers.PC + 1) & 0xffff));
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
}
/**********************************************************************************************/

/* JSR/RTS *********************************************************************************** */
static void Op20()
{
   Absolute(false);
   PushW(CPU.PC - CPU.PCBase - 1);
   S9xSetPCBase(ICPU.ShiftedPB + (OpAddress & 0xffff));
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

//JSR a,x
static void OpFCE1()
{
   AbsoluteIndexedIndirect(false);
   PushWE(CPU.PC - CPU.PCBase - 1);
   S9xSetPCBase(ICPU.ShiftedPB + OpAddress);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void OpFC()
{
   AbsoluteIndexedIndirect(false);
   PushW(CPU.PC - CPU.PCBase - 1);
   S9xSetPCBase(ICPU.ShiftedPB + OpAddress);
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE;
#endif
}

static void Op60()
{
   PullW(ICPU.Registers.PC);
   S9xSetPCBase(ICPU.ShiftedPB + ((ICPU.Registers.PC + 1) & 0xffff));
#ifndef SA1_OPCODES
   CPU.Cycles += ONE_CYCLE * 3;
#endif
}

/**********************************************************************************************/

/* MVN/MVP *********************************************************************************** */
static void Op54X1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2 + TWO_CYCLES;
#endif

   ICPU.Registers.DB = *CPU.PC++;
   ICPU.ShiftedDB = ICPU.Registers.DB << 16;
   OpenBus = *CPU.PC++;

   S9xSetByte(S9xGetByte((OpenBus << 16) + ICPU.Registers.X.W), ICPU.ShiftedDB + ICPU.Registers.Y.W);

   ICPU.Registers.XL++;
   ICPU.Registers.YL++;
   ICPU.Registers.A.W--;
   if (ICPU.Registers.A.W != 0xffff)
      CPU.PC -= 3;
}

static void Op54X0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2 + TWO_CYCLES;
#endif

   ICPU.Registers.DB = *CPU.PC++;
   ICPU.ShiftedDB = ICPU.Registers.DB << 16;
   OpenBus = *CPU.PC++;

   S9xSetByte(S9xGetByte((OpenBus << 16) + ICPU.Registers.X.W), ICPU.ShiftedDB + ICPU.Registers.Y.W);

   ICPU.Registers.X.W++;
   ICPU.Registers.Y.W++;
   ICPU.Registers.A.W--;
   if (ICPU.Registers.A.W != 0xffff)
      CPU.PC -= 3;
}

static void Op44X1()
{
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2 + TWO_CYCLES;
#endif
   ICPU.Registers.DB = *CPU.PC++;
   ICPU.ShiftedDB = ICPU.Registers.DB << 16;
   OpenBus = *CPU.PC++;
   S9xSetByte(S9xGetByte((OpenBus << 16) + ICPU.Registers.X.W), ICPU.ShiftedDB + ICPU.Registers.Y.W);

   ICPU.Registers.XL--;
   ICPU.Registers.YL--;
   ICPU.Registers.A.W--;
   if (ICPU.Registers.A.W != 0xffff)
      CPU.PC -= 3;
}

static void Op44X0()
{
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeedx2 + TWO_CYCLES;
#endif
   ICPU.Registers.DB = *CPU.PC++;
   ICPU.ShiftedDB = ICPU.Registers.DB << 16;
   OpenBus = *CPU.PC++;
   S9xSetByte(S9xGetByte((OpenBus << 16) + ICPU.Registers.X.W), ICPU.ShiftedDB + ICPU.Registers.Y.W);

   ICPU.Registers.X.W--;
   ICPU.Registers.Y.W--;
   ICPU.Registers.A.W--;
   if (ICPU.Registers.A.W != 0xffff)
      CPU.PC -= 3;
}

/**********************************************************************************************/

/* REP/SEP *********************************************************************************** */
static void OpC2()
{
   uint8_t Work8 = ~*CPU.PC++;
   ICPU.Registers.PL &= Work8;
   ICPU._Carry &= Work8;
   ICPU._Overflow &= (Work8 >> 6);
   ICPU._Negative &= Work8;
   ICPU._Zero |= ~Work8 & Zero;

#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed + ONE_CYCLE;
#endif
   if (CheckEmulation())
   {
      SetFlags(MemoryFlag | IndexFlag);
      missing.emulate6502 = 1;
   }
   if (CheckIndex())
   {
      ICPU.Registers.XH = 0;
      ICPU.Registers.YH = 0;
   }
   S9xFixCycles();
}

static void OpE2()
{
   uint8_t Work8 = *CPU.PC++;
   ICPU.Registers.PL |= Work8;
   ICPU._Carry |= Work8 & 1;
   ICPU._Overflow |= (Work8 >> 6) & 1;
   ICPU._Negative |= Work8;
   if (Work8 & Zero)
      ICPU._Zero = 0;
#ifndef SA1_OPCODES
   CPU.Cycles += CPU.MemSpeed + ONE_CYCLE;
#endif
   if (CheckEmulation())
   {
      SetFlags(MemoryFlag | IndexFlag);
      missing.emulate6502 = 1;
   }
   if (CheckIndex())
   {
      ICPU.Registers.XH = 0;
      ICPU.Registers.YH = 0;
   }
   S9xFixCycles();
}
/**********************************************************************************************/

/* XBA *************************************************************************************** */
static void OpEB()
{
   uint8_t Work8 = ICPU.Registers.AL;
   ICPU.Registers.AL = ICPU.Registers.AH;
   ICPU.Registers.AH = Work8;
   SetZN8(ICPU.Registers.AL);
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
}
/**********************************************************************************************/

/* RTI *************************************************************************************** */
static void Op40()
{
   PullB(ICPU.Registers.PL);
   S9xUnpackStatus();
   PullW(ICPU.Registers.PC);
   if (!CheckEmulation())
   {
      PullB(ICPU.Registers.PB);
      ICPU.ShiftedPB = ICPU.Registers.PB << 16;
   }
   else
   {
      SetFlags(MemoryFlag | IndexFlag);
      missing.emulate6502 = 1;
   }
   S9xSetPCBase(ICPU.ShiftedPB + ICPU.Registers.PC);
   if (CheckIndex())
   {
      ICPU.Registers.XH = 0;
      ICPU.Registers.YH = 0;
   }
#ifndef SA1_OPCODES
   CPU.Cycles += TWO_CYCLES;
#endif
   S9xFixCycles();
}

/**********************************************************************************************/

/* STP/WAI/DB ******************************************************************************** */
// WAI
static void OpCB()
{
#ifdef SA1_OPCODES
   SA1.WaitingForInterrupt = true;
   SA1.PC--;
#else // SA_OPCODES
   CPU.WaitingForInterrupt = true;
   CPU.PC--;
#ifdef CPU_SHUTDOWN
   if (Settings.Shutdown)
   {
      CPU.Cycles = CPU.NextEvent;
#ifndef USE_BLARGG_APU
      if (IAPU.APUExecuting)
      {
         ICPU.CPUExecuting = false;
         do
         {
            APU_EXECUTE1();
         }
         while (APU.Cycles < CPU.NextEvent);
         ICPU.CPUExecuting = true;
      }
#endif
   }
#endif
#endif
}

// Usually an STP opcode
// SNESAdvance speed hack, not implemented in Snes9xTYL | Snes9x-Euphoria (from the speed-hacks branch of CatSFC)
static void OpDB()
{
#if !defined NO_SPEEDHACKS && defined CPU_SHUTDOWN
   uint8_t NextByte = *CPU.PC++;

   ForceShutdown();

   int8_t BranchOffset = (NextByte & 0x7F) | ((NextByte & 0x40) << 1);
   // ^ -64 .. +63, sign extend bit 6 into 7 for unpacking
   OpAddress = ((int32_t) (CPU.PC - CPU.PCBase) + BranchOffset) & 0xffff;

   switch (NextByte & 0x80)
   {
   case 0x00: // BNE
      BranchCheck1 ();
      if (!CheckZero ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0x80: // BEQ
      BranchCheck2 ();
      if (CheckZero ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   }
#else
     CPU.PC--;
     CPU.Flags |= DEBUG_MODE_FLAG;
#endif
}

// SNESAdvance speed hack, as implemented in Snes9xTYL / Snes9x-Euphoria (from the speed-hacks branch of CatSFC)
static void Op42()
{
#if !defined NO_SPEEDHACKS && defined CPU_SHUTDOWN
   uint8_t NextByte = *CPU.PC++;

   ForceShutdown();

   int8_t BranchOffset = 0xF0 | (NextByte & 0xF); // always negative
   OpAddress = ((int32_t) (CPU.PC - CPU.PCBase) + BranchOffset) & 0xffff;

   switch (NextByte & 0xF0)
   {
   case 0x10: // BPL
      BranchCheck1 ();
      if (!CheckNegative ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0x30: // BMI
      BranchCheck1 ();
      if (CheckNegative ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0x50: // BVC
      BranchCheck0 ();
      if (!CheckOverflow ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0x70: // BVS
      BranchCheck0 ();
      if (CheckOverflow ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0x80: // BRA
      CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
      CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
      CPU.Cycles++;
#endif
#endif
      CPUShutdown ();
      return;
   case 0x90: // BCC
      BranchCheck0 ();
      if (!CheckCarry ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0xB0: // BCS
      BranchCheck0 ();
      if (CheckCarry ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0xD0: // BNE
      BranchCheck1 ();
      if (!CheckZero ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   case 0xF0: // BEQ
      BranchCheck2 ();
      if (CheckZero ())
      {
         CPU.PC = CPU.PCBase + OpAddress;
#ifdef VAR_CYCLES
         CPU.Cycles += ONE_CYCLE;
#else
#ifndef SA1_OPCODES
         CPU.Cycles++;
#endif
#endif
         CPUShutdown ();
      }
      return;
   }
#endif
}

/*****************************************************************************/

/*****************************************************************************/
/* CPU-S9xOpcodes Definitions                                                */
/*****************************************************************************/
SOpcodes S9xOpcodesM1X1[256] =
{
   {Op00},      {Op01M1},    {Op02},      {Op03M1},    {Op04M1},
   {Op05M1},    {Op06M1},    {Op07M1},    {Op08},      {Op09M1},
   {Op0AM1},    {Op0B},      {Op0CM1},    {Op0DM1},    {Op0EM1},
   {Op0FM1},    {Op10},      {Op11M1},    {Op12M1},    {Op13M1},
   {Op14M1},    {Op15M1},    {Op16M1},    {Op17M1},    {Op18},
   {Op19M1},    {Op1AM1},    {Op1B},      {Op1CM1},    {Op1DM1},
   {Op1EM1},    {Op1FM1},    {Op20},      {Op21M1},    {Op22},
   {Op23M1},    {Op24M1},    {Op25M1},    {Op26M1},    {Op27M1},
   {Op28},      {Op29M1},    {Op2AM1},    {Op2B},      {Op2CM1},
   {Op2DM1},    {Op2EM1},    {Op2FM1},    {Op30},      {Op31M1},
   {Op32M1},    {Op33M1},    {Op34M1},    {Op35M1},    {Op36M1},
   {Op37M1},    {Op38},      {Op39M1},    {Op3AM1},    {Op3B},
   {Op3CM1},    {Op3DM1},    {Op3EM1},    {Op3FM1},    {Op40},
   {Op41M1},    {Op42},      {Op43M1},    {Op44X1},    {Op45M1},
   {Op46M1},    {Op47M1},    {Op48M1},    {Op49M1},    {Op4AM1},
   {Op4B},      {Op4C},      {Op4DM1},    {Op4EM1},    {Op4FM1},
   {Op50},      {Op51M1},    {Op52M1},    {Op53M1},    {Op54X1},
   {Op55M1},    {Op56M1},    {Op57M1},    {Op58},      {Op59M1},
   {Op5AX1},    {Op5B},      {Op5C},      {Op5DM1},    {Op5EM1},
   {Op5FM1},    {Op60},      {Op61M1},    {Op62},      {Op63M1},
   {Op64M1},    {Op65M1},    {Op66M1},    {Op67M1},    {Op68M1},
   {Op69M1},    {Op6AM1},    {Op6B},      {Op6C},      {Op6DM1},
   {Op6EM1},    {Op6FM1},    {Op70},      {Op71M1},    {Op72M1},
   {Op73M1},    {Op74M1},    {Op75M1},    {Op76M1},    {Op77M1},
   {Op78},      {Op79M1},    {Op7AX1},    {Op7B},      {Op7C},
   {Op7DM1},    {Op7EM1},    {Op7FM1},    {Op80},      {Op81M1},
   {Op82},      {Op83M1},    {Op84X1},    {Op85M1},    {Op86X1},
   {Op87M1},    {Op88X1},    {Op89M1},    {Op8AM1},    {Op8B},
   {Op8CX1},    {Op8DM1},    {Op8EX1},    {Op8FM1},    {Op90},
   {Op91M1},    {Op92M1},    {Op93M1},    {Op94X1},    {Op95M1},
   {Op96X1},    {Op97M1},    {Op98M1},    {Op99M1},    {Op9A},
   {Op9BX1},    {Op9CM1},    {Op9DM1},    {Op9EM1},    {Op9FM1},
   {OpA0X1},    {OpA1M1},    {OpA2X1},    {OpA3M1},    {OpA4X1},
   {OpA5M1},    {OpA6X1},    {OpA7M1},    {OpA8X1},    {OpA9M1},
   {OpAAX1},    {OpAB},      {OpACX1},    {OpADM1},    {OpAEX1},
   {OpAFM1},    {OpB0},      {OpB1M1},    {OpB2M1},    {OpB3M1},
   {OpB4X1},    {OpB5M1},    {OpB6X1},    {OpB7M1},    {OpB8},
   {OpB9M1},    {OpBAX1},    {OpBBX1},    {OpBCX1},    {OpBDM1},
   {OpBEX1},    {OpBFM1},    {OpC0X1},    {OpC1M1},    {OpC2},
   {OpC3M1},    {OpC4X1},    {OpC5M1},    {OpC6M1},    {OpC7M1},
   {OpC8X1},    {OpC9M1},    {OpCAX1},    {OpCB},      {OpCCX1},
   {OpCDM1},    {OpCEM1},    {OpCFM1},    {OpD0},      {OpD1M1},
   {OpD2M1},    {OpD3M1},    {OpD4},      {OpD5M1},    {OpD6M1},
   {OpD7M1},    {OpD8},      {OpD9M1},    {OpDAX1},    {OpDB},
   {OpDC},      {OpDDM1},    {OpDEM1},    {OpDFM1},    {OpE0X1},
   {OpE1M1},    {OpE2},      {OpE3M1},    {OpE4X1},    {OpE5M1},
   {OpE6M1},    {OpE7M1},    {OpE8X1},    {OpE9M1},    {OpEA},
   {OpEB},      {OpECX1},    {OpEDM1},    {OpEEM1},    {OpEFM1},
   {OpF0},      {OpF1M1},    {OpF2M1},    {OpF3M1},    {OpF4},
   {OpF5M1},    {OpF6M1},    {OpF7M1},    {OpF8},      {OpF9M1},
   {OpFAX1},    {OpFB},      {OpFC},      {OpFDM1},    {OpFEM1},
   {OpFFM1}
};

SOpcodes S9xOpcodesE1[256] =
{
    {Op00},	     {Op01M1},    {Op02},      {Op03M1},    {Op04M1},
    {Op05M1},    {Op06M1},    {Op07M1},    {Op08E1},    {Op09M1},
    {Op0AM1},    {Op0BE1},    {Op0CM1},    {Op0DM1},    {Op0EM1},
    {Op0FM1},    {Op10},      {Op11M1},    {Op12M1},    {Op13M1},
    {Op14M1},    {Op15M1},    {Op16M1},    {Op17M1},    {Op18},
    {Op19M1},    {Op1AM1},    {Op1B},      {Op1CM1},    {Op1DM1},
    {Op1EM1},    {Op1FM1},    {Op20},      {Op21M1},    {Op22E1},
    {Op23M1},    {Op24M1},    {Op25M1},    {Op26M1},    {Op27M1},
    {Op28},      {Op29M1},    {Op2AM1},    {Op2BE1},    {Op2CM1},
    {Op2DM1},    {Op2EM1},    {Op2FM1},    {Op30},      {Op31M1},
    {Op32M1},    {Op33M1},    {Op34M1},    {Op35M1},    {Op36M1},
    {Op37M1},    {Op38},      {Op39M1},    {Op3AM1},    {Op3B},
    {Op3CM1},    {Op3DM1},    {Op3EM1},    {Op3FM1},    {Op40},
    {Op41M1},    {Op42},      {Op43M1},    {Op44X1},    {Op45M1},
    {Op46M1},    {Op47M1},    {Op48E1},    {Op49M1},    {Op4AM1},
    {Op4BE1},    {Op4C},      {Op4DM1},    {Op4EM1},    {Op4FM1},
    {Op50},      {Op51M1},    {Op52M1},    {Op53M1},    {Op54X1},
    {Op55M1},    {Op56M1},    {Op57M1},    {Op58},      {Op59M1},
    {Op5AE1},    {Op5B},      {Op5C},      {Op5DM1},    {Op5EM1},
    {Op5FM1},    {Op60},      {Op61M1},    {Op62E1},    {Op63M1},
    {Op64M1},    {Op65M1},    {Op66M1},    {Op67M1},    {Op68E1},
    {Op69M1},    {Op6AM1},    {Op6BE1},    {Op6C},      {Op6DM1},
    {Op6EM1},    {Op6FM1},    {Op70},      {Op71M1},    {Op72M1},
    {Op73M1},    {Op74M1},    {Op75M1},    {Op76M1},    {Op77M1},
    {Op78},      {Op79M1},    {Op7AE1},    {Op7B},      {Op7C},
    {Op7DM1},    {Op7EM1},    {Op7FM1},    {Op80},      {Op81M1},
    {Op82},      {Op83M1},    {Op84X1},    {Op85M1},    {Op86X1},
    {Op87M1},    {Op88X1},    {Op89M1},    {Op8AM1},    {Op8BE1},
    {Op8CX1},    {Op8DM1},    {Op8EX1},    {Op8FM1},    {Op90},
    {Op91M1},    {Op92M1},    {Op93M1},    {Op94X1},    {Op95M1},
    {Op96X1},    {Op97M1},    {Op98M1},    {Op99M1},    {Op9A},
    {Op9BX1},    {Op9CM1},    {Op9DM1},    {Op9EM1},    {Op9FM1},
    {OpA0X1},    {OpA1M1},    {OpA2X1},    {OpA3M1},    {OpA4X1},
    {OpA5M1},    {OpA6X1},    {OpA7M1},    {OpA8X1},    {OpA9M1},
    {OpAAX1},    {OpABE1},    {OpACX1},    {OpADM1},    {OpAEX1},
    {OpAFM1},    {OpB0},      {OpB1M1},    {OpB2M1},    {OpB3M1},
    {OpB4X1},    {OpB5M1},    {OpB6X1},    {OpB7M1},    {OpB8},
    {OpB9M1},    {OpBAX1},    {OpBBX1},    {OpBCX1},    {OpBDM1},
    {OpBEX1},    {OpBFM1},    {OpC0X1},    {OpC1M1},    {OpC2},
    {OpC3M1},    {OpC4X1},    {OpC5M1},    {OpC6M1},    {OpC7M1},
    {OpC8X1},    {OpC9M1},    {OpCAX1},    {OpCB},      {OpCCX1},
    {OpCDM1},    {OpCEM1},    {OpCFM1},    {OpD0},      {OpD1M1},
    {OpD2M1},    {OpD3M1},    {OpD4E1},    {OpD5M1},    {OpD6M1},
    {OpD7M1},    {OpD8},      {OpD9M1},    {OpDAE1},    {OpDB},
    {OpDC},      {OpDDM1},    {OpDEM1},    {OpDFM1},    {OpE0X1},
    {OpE1M1},    {OpE2},      {OpE3M1},    {OpE4X1},    {OpE5M1},
    {OpE6M1},    {OpE7M1},    {OpE8X1},    {OpE9M1},    {OpEA},
    {OpEB},      {OpECX1},    {OpEDM1},    {OpEEM1},    {OpEFM1},
    {OpF0},      {OpF1M1},    {OpF2M1},    {OpF3M1},    {OpF4E1},
    {OpF5M1},    {OpF6M1},    {OpF7M1},    {OpF8},      {OpF9M1},
    {OpFAE1},    {OpFB},      {OpFCE1},    {OpFDM1},    {OpFEM1},
    {OpFFM1}
};

SOpcodes S9xOpcodesM1X0[256] =
{
   {Op00},  {Op01M1},    {Op02},      {Op03M1},    {Op04M1},
   {Op05M1},    {Op06M1},    {Op07M1},    {Op08},      {Op09M1},
   {Op0AM1},    {Op0B},      {Op0CM1},    {Op0DM1},    {Op0EM1},
   {Op0FM1},    {Op10},      {Op11M1},    {Op12M1},    {Op13M1},
   {Op14M1},    {Op15M1},    {Op16M1},    {Op17M1},    {Op18},
   {Op19M1},    {Op1AM1},    {Op1B},      {Op1CM1},    {Op1DM1},
   {Op1EM1},    {Op1FM1},    {Op20},      {Op21M1},    {Op22},
   {Op23M1},    {Op24M1},    {Op25M1},    {Op26M1},    {Op27M1},
   {Op28},      {Op29M1},    {Op2AM1},    {Op2B},      {Op2CM1},
   {Op2DM1},    {Op2EM1},    {Op2FM1},    {Op30},      {Op31M1},
   {Op32M1},    {Op33M1},    {Op34M1},    {Op35M1},    {Op36M1},
   {Op37M1},    {Op38},      {Op39M1},    {Op3AM1},    {Op3B},
   {Op3CM1},    {Op3DM1},    {Op3EM1},    {Op3FM1},    {Op40},
   {Op41M1},    {Op42},      {Op43M1},    {Op44X0},    {Op45M1},
   {Op46M1},    {Op47M1},    {Op48M1},    {Op49M1},    {Op4AM1},
   {Op4B},      {Op4C},      {Op4DM1},    {Op4EM1},    {Op4FM1},
   {Op50},      {Op51M1},    {Op52M1},    {Op53M1},    {Op54X0},
   {Op55M1},    {Op56M1},    {Op57M1},    {Op58},      {Op59M1},
   {Op5AX0},    {Op5B},      {Op5C},      {Op5DM1},    {Op5EM1},
   {Op5FM1},    {Op60},      {Op61M1},    {Op62},      {Op63M1},
   {Op64M1},    {Op65M1},    {Op66M1},    {Op67M1},    {Op68M1},
   {Op69M1},    {Op6AM1},    {Op6B},      {Op6C},      {Op6DM1},
   {Op6EM1},    {Op6FM1},    {Op70},      {Op71M1},    {Op72M1},
   {Op73M1},    {Op74M1},    {Op75M1},    {Op76M1},    {Op77M1},
   {Op78},      {Op79M1},    {Op7AX0},    {Op7B},      {Op7C},
   {Op7DM1},    {Op7EM1},    {Op7FM1},    {Op80},      {Op81M1},
   {Op82},      {Op83M1},    {Op84X0},    {Op85M1},    {Op86X0},
   {Op87M1},    {Op88X0},    {Op89M1},    {Op8AM1},    {Op8B},
   {Op8CX0},    {Op8DM1},    {Op8EX0},    {Op8FM1},    {Op90},
   {Op91M1},    {Op92M1},    {Op93M1},    {Op94X0},    {Op95M1},
   {Op96X0},    {Op97M1},    {Op98M1},    {Op99M1},    {Op9A},
   {Op9BX0},    {Op9CM1},    {Op9DM1},    {Op9EM1},    {Op9FM1},
   {OpA0X0},    {OpA1M1},    {OpA2X0},    {OpA3M1},    {OpA4X0},
   {OpA5M1},    {OpA6X0},    {OpA7M1},    {OpA8X0},    {OpA9M1},
   {OpAAX0},    {OpAB},      {OpACX0},    {OpADM1},    {OpAEX0},
   {OpAFM1},    {OpB0},      {OpB1M1},    {OpB2M1},    {OpB3M1},
   {OpB4X0},    {OpB5M1},    {OpB6X0},    {OpB7M1},    {OpB8},
   {OpB9M1},    {OpBAX0},    {OpBBX0},    {OpBCX0},    {OpBDM1},
   {OpBEX0},    {OpBFM1},    {OpC0X0},    {OpC1M1},    {OpC2},
   {OpC3M1},    {OpC4X0},    {OpC5M1},    {OpC6M1},    {OpC7M1},
   {OpC8X0},    {OpC9M1},    {OpCAX0},    {OpCB},      {OpCCX0},
   {OpCDM1},    {OpCEM1},    {OpCFM1},    {OpD0},      {OpD1M1},
   {OpD2M1},    {OpD3M1},    {OpD4},      {OpD5M1},    {OpD6M1},
   {OpD7M1},    {OpD8},      {OpD9M1},    {OpDAX0},    {OpDB},
   {OpDC},      {OpDDM1},    {OpDEM1},    {OpDFM1},    {OpE0X0},
   {OpE1M1},    {OpE2},      {OpE3M1},    {OpE4X0},    {OpE5M1},
   {OpE6M1},    {OpE7M1},    {OpE8X0},    {OpE9M1},    {OpEA},
   {OpEB},      {OpECX0},    {OpEDM1},    {OpEEM1},    {OpEFM1},
   {OpF0},      {OpF1M1},    {OpF2M1},    {OpF3M1},    {OpF4},
   {OpF5M1},    {OpF6M1},    {OpF7M1},    {OpF8},      {OpF9M1},
   {OpFAX0},    {OpFB},      {OpFC},      {OpFDM1},    {OpFEM1},
   {OpFFM1}
};

SOpcodes S9xOpcodesM0X0[256] =
{
   {Op00},  {Op01M0},    {Op02},      {Op03M0},    {Op04M0},
   {Op05M0},    {Op06M0},    {Op07M0},    {Op08},      {Op09M0},
   {Op0AM0},    {Op0B},      {Op0CM0},    {Op0DM0},    {Op0EM0},
   {Op0FM0},    {Op10},      {Op11M0},    {Op12M0},    {Op13M0},
   {Op14M0},    {Op15M0},    {Op16M0},    {Op17M0},    {Op18},
   {Op19M0},    {Op1AM0},    {Op1B},      {Op1CM0},    {Op1DM0},
   {Op1EM0},    {Op1FM0},    {Op20},      {Op21M0},    {Op22},
   {Op23M0},    {Op24M0},    {Op25M0},    {Op26M0},    {Op27M0},
   {Op28},      {Op29M0},    {Op2AM0},    {Op2B},      {Op2CM0},
   {Op2DM0},    {Op2EM0},    {Op2FM0},    {Op30},      {Op31M0},
   {Op32M0},    {Op33M0},    {Op34M0},    {Op35M0},    {Op36M0},
   {Op37M0},    {Op38},      {Op39M0},    {Op3AM0},    {Op3B},
   {Op3CM0},    {Op3DM0},    {Op3EM0},    {Op3FM0},    {Op40},
   {Op41M0},    {Op42},      {Op43M0},    {Op44X0},    {Op45M0},
   {Op46M0},    {Op47M0},    {Op48M0},    {Op49M0},    {Op4AM0},
   {Op4B},      {Op4C},      {Op4DM0},    {Op4EM0},    {Op4FM0},
   {Op50},      {Op51M0},    {Op52M0},    {Op53M0},    {Op54X0},
   {Op55M0},    {Op56M0},    {Op57M0},    {Op58},      {Op59M0},
   {Op5AX0},    {Op5B},      {Op5C},      {Op5DM0},    {Op5EM0},
   {Op5FM0},    {Op60},      {Op61M0},    {Op62},      {Op63M0},
   {Op64M0},    {Op65M0},    {Op66M0},    {Op67M0},    {Op68M0},
   {Op69M0},    {Op6AM0},    {Op6B},      {Op6C},      {Op6DM0},
   {Op6EM0},    {Op6FM0},    {Op70},      {Op71M0},    {Op72M0},
   {Op73M0},    {Op74M0},    {Op75M0},    {Op76M0},    {Op77M0},
   {Op78},      {Op79M0},    {Op7AX0},    {Op7B},      {Op7C},
   {Op7DM0},    {Op7EM0},    {Op7FM0},    {Op80},      {Op81M0},
   {Op82},      {Op83M0},    {Op84X0},    {Op85M0},    {Op86X0},
   {Op87M0},    {Op88X0},    {Op89M0},    {Op8AM0},    {Op8B},
   {Op8CX0},    {Op8DM0},    {Op8EX0},    {Op8FM0},    {Op90},
   {Op91M0},    {Op92M0},    {Op93M0},    {Op94X0},    {Op95M0},
   {Op96X0},    {Op97M0},    {Op98M0},    {Op99M0},    {Op9A},
   {Op9BX0},    {Op9CM0},    {Op9DM0},    {Op9EM0},    {Op9FM0},
   {OpA0X0},    {OpA1M0},    {OpA2X0},    {OpA3M0},    {OpA4X0},
   {OpA5M0},    {OpA6X0},    {OpA7M0},    {OpA8X0},    {OpA9M0},
   {OpAAX0},    {OpAB},      {OpACX0},    {OpADM0},    {OpAEX0},
   {OpAFM0},    {OpB0},      {OpB1M0},    {OpB2M0},    {OpB3M0},
   {OpB4X0},    {OpB5M0},    {OpB6X0},    {OpB7M0},    {OpB8},
   {OpB9M0},    {OpBAX0},    {OpBBX0},    {OpBCX0},    {OpBDM0},
   {OpBEX0},    {OpBFM0},    {OpC0X0},    {OpC1M0},    {OpC2},
   {OpC3M0},    {OpC4X0},    {OpC5M0},    {OpC6M0},    {OpC7M0},
   {OpC8X0},    {OpC9M0},    {OpCAX0},    {OpCB},      {OpCCX0},
   {OpCDM0},    {OpCEM0},    {OpCFM0},    {OpD0},      {OpD1M0},
   {OpD2M0},    {OpD3M0},    {OpD4},      {OpD5M0},    {OpD6M0},
   {OpD7M0},    {OpD8},      {OpD9M0},    {OpDAX0},    {OpDB},
   {OpDC},      {OpDDM0},    {OpDEM0},    {OpDFM0},    {OpE0X0},
   {OpE1M0},    {OpE2},      {OpE3M0},    {OpE4X0},    {OpE5M0},
   {OpE6M0},    {OpE7M0},    {OpE8X0},    {OpE9M0},    {OpEA},
   {OpEB},      {OpECX0},    {OpEDM0},    {OpEEM0},    {OpEFM0},
   {OpF0},      {OpF1M0},    {OpF2M0},    {OpF3M0},    {OpF4},
   {OpF5M0},    {OpF6M0},    {OpF7M0},    {OpF8},      {OpF9M0},
   {OpFAX0},    {OpFB},      {OpFC},      {OpFDM0},    {OpFEM0},
   {OpFFM0}
};

SOpcodes S9xOpcodesM0X1[256] =
{
   {Op00},  {Op01M0},    {Op02},      {Op03M0},    {Op04M0},
   {Op05M0},    {Op06M0},    {Op07M0},    {Op08},      {Op09M0},
   {Op0AM0},    {Op0B},      {Op0CM0},    {Op0DM0},    {Op0EM0},
   {Op0FM0},    {Op10},      {Op11M0},    {Op12M0},    {Op13M0},
   {Op14M0},    {Op15M0},    {Op16M0},    {Op17M0},    {Op18},
   {Op19M0},    {Op1AM0},    {Op1B},      {Op1CM0},    {Op1DM0},
   {Op1EM0},    {Op1FM0},    {Op20},      {Op21M0},    {Op22},
   {Op23M0},    {Op24M0},    {Op25M0},    {Op26M0},    {Op27M0},
   {Op28},      {Op29M0},    {Op2AM0},    {Op2B},      {Op2CM0},
   {Op2DM0},    {Op2EM0},    {Op2FM0},    {Op30},      {Op31M0},
   {Op32M0},    {Op33M0},    {Op34M0},    {Op35M0},    {Op36M0},
   {Op37M0},    {Op38},      {Op39M0},    {Op3AM0},    {Op3B},
   {Op3CM0},    {Op3DM0},    {Op3EM0},    {Op3FM0},    {Op40},
   {Op41M0},    {Op42},      {Op43M0},    {Op44X1},    {Op45M0},
   {Op46M0},    {Op47M0},    {Op48M0},    {Op49M0},    {Op4AM0},
   {Op4B},      {Op4C},      {Op4DM0},    {Op4EM0},    {Op4FM0},
   {Op50},      {Op51M0},    {Op52M0},    {Op53M0},    {Op54X1},
   {Op55M0},    {Op56M0},    {Op57M0},    {Op58},      {Op59M0},
   {Op5AX1},    {Op5B},      {Op5C},      {Op5DM0},    {Op5EM0},
   {Op5FM0},    {Op60},      {Op61M0},    {Op62},      {Op63M0},
   {Op64M0},    {Op65M0},    {Op66M0},    {Op67M0},    {Op68M0},
   {Op69M0},    {Op6AM0},    {Op6B},      {Op6C},      {Op6DM0},
   {Op6EM0},    {Op6FM0},    {Op70},      {Op71M0},    {Op72M0},
   {Op73M0},    {Op74M0},    {Op75M0},    {Op76M0},    {Op77M0},
   {Op78},      {Op79M0},    {Op7AX1},    {Op7B},      {Op7C},
   {Op7DM0},    {Op7EM0},    {Op7FM0},    {Op80},      {Op81M0},
   {Op82},      {Op83M0},    {Op84X1},    {Op85M0},    {Op86X1},
   {Op87M0},    {Op88X1},    {Op89M0},    {Op8AM0},    {Op8B},
   {Op8CX1},    {Op8DM0},    {Op8EX1},    {Op8FM0},    {Op90},
   {Op91M0},    {Op92M0},    {Op93M0},    {Op94X1},    {Op95M0},
   {Op96X1},    {Op97M0},    {Op98M0},    {Op99M0},    {Op9A},
   {Op9BX1},    {Op9CM0},    {Op9DM0},    {Op9EM0},    {Op9FM0},
   {OpA0X1},    {OpA1M0},    {OpA2X1},    {OpA3M0},    {OpA4X1},
   {OpA5M0},    {OpA6X1},    {OpA7M0},    {OpA8X1},    {OpA9M0},
   {OpAAX1},    {OpAB},      {OpACX1},    {OpADM0},    {OpAEX1},
   {OpAFM0},    {OpB0},      {OpB1M0},    {OpB2M0},    {OpB3M0},
   {OpB4X1},    {OpB5M0},    {OpB6X1},    {OpB7M0},    {OpB8},
   {OpB9M0},    {OpBAX1},    {OpBBX1},    {OpBCX1},    {OpBDM0},
   {OpBEX1},    {OpBFM0},    {OpC0X1},    {OpC1M0},    {OpC2},
   {OpC3M0},    {OpC4X1},    {OpC5M0},    {OpC6M0},    {OpC7M0},
   {OpC8X1},    {OpC9M0},    {OpCAX1},    {OpCB},      {OpCCX1},
   {OpCDM0},    {OpCEM0},    {OpCFM0},    {OpD0},      {OpD1M0},
   {OpD2M0},    {OpD3M0},    {OpD4},      {OpD5M0},    {OpD6M0},
   {OpD7M0},    {OpD8},      {OpD9M0},    {OpDAX1},    {OpDB},
   {OpDC},      {OpDDM0},    {OpDEM0},    {OpDFM0},    {OpE0X1},
   {OpE1M0},    {OpE2},      {OpE3M0},    {OpE4X1},    {OpE5M0},
   {OpE6M0},    {OpE7M0},    {OpE8X1},    {OpE9M0},    {OpEA},
   {OpEB},      {OpECX1},    {OpEDM0},    {OpEEM0},    {OpEFM0},
   {OpF0},      {OpF1M0},    {OpF2M0},    {OpF3M0},    {OpF4},
   {OpF5M0},    {OpF6M0},    {OpF7M0},    {OpF8},      {OpF9M0},
   {OpFAX1},    {OpFB},      {OpFC},      {OpFDM0},    {OpFEM0},
   {OpFFM0}
};
