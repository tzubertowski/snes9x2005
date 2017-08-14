#include "../copyright"

#include "fxemu.h"
#include "fxinst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* The FxChip Emulator's internal variables */
FxRegs_s GSU; /* This will be initialized when loading a ROM */

void FxFlushCache(void)
{
   GSU.vCacheBaseReg = 0;
   GSU.bCacheActive = false;
}

void fx_flushCache(void)
{
   GSU.bCacheActive = false;
}

void fx_updateRamBank(uint8_t Byte)
{
   // Update BankReg and Bank pointer
   GSU.vRamBankReg = (uint32_t) Byte & (FX_RAM_BANKS - 1);
   GSU.pvRamBank = GSU.apvRamBank[Byte & 0x3];
}

static void fx_readRegisterSpaceForCheck(void)
{
   R15 = GSU.pvRegisters[30];
   R15 |= ((uint32_t) GSU.pvRegisters[31]) << 8;
   GSU.vStatusReg = (uint32_t) GSU.pvRegisters[GSU_SFR];
   GSU.vStatusReg |= ((uint32_t) GSU.pvRegisters[GSU_SFR + 1]) << 8;
   GSU.vPrgBankReg = (uint32_t) GSU.pvRegisters[GSU_PBR];
}

static void fx_readRegisterSpaceForUse(void)
{
   static uint32_t avHeight[] = { 128, 160, 192, 256 };
   static uint32_t avMult[] = { 16, 32, 32, 64 };
   int32_t i;
   uint8_t* p = GSU.pvRegisters;

   /* Update R0 - R14 */
   for (i = 0; i < 15; i++)
   {
      GSU.avReg[i] = *p++;
      GSU.avReg[i] += ((uint32_t)(*p++)) << 8;
   }

   /* Update other registers */
   p = GSU.pvRegisters;
   GSU.vRomBankReg = (uint32_t)p[GSU_ROMBR];
   GSU.vRamBankReg = ((uint32_t)p[GSU_RAMBR]) & (FX_RAM_BANKS - 1);
   GSU.vCacheBaseReg = (uint32_t)p[GSU_CBR];
   GSU.vCacheBaseReg |= ((uint32_t)p[GSU_CBR + 1]) << 8;

   /* Update status register variables */
   GSU.vZero = !(GSU.vStatusReg & FLG_Z);
   GSU.vSign = (GSU.vStatusReg & FLG_S) << 12;
   GSU.vOverflow = (GSU.vStatusReg & FLG_OV) << 16;
   GSU.vCarry = (GSU.vStatusReg & FLG_CY) >> 2;

   /* Set bank pointers */
   GSU.pvRamBank = GSU.apvRamBank[GSU.vRamBankReg & 0x3];
   GSU.pvRomBank = GSU.apvRomBank[GSU.vRomBankReg];
   GSU.pvPrgBank = GSU.apvRomBank[GSU.vPrgBankReg];

   /* Set screen pointers */
   GSU.pvScreenBase = &GSU.pvRam[ USEX8(p[GSU_SCBR]) << 10 ];
   i  =  (int32_t)(!!(p[GSU_SCMR] & 0x04));
   i |= ((int32_t)(!!(p[GSU_SCMR] & 0x20))) << 1;
   GSU.vScreenHeight = GSU.vScreenRealHeight = avHeight[i];
   GSU.vMode = p[GSU_SCMR] & 0x03;
   if (i == 3)
      GSU.vScreenSize = (256 / 8) * (256 / 8) * 32;
   else
      GSU.vScreenSize = (GSU.vScreenHeight / 8) * (256 / 8) * avMult[GSU.vMode];
   if (GSU.vPlotOptionReg & 0x10)
      GSU.vScreenHeight = 256; /* OBJ Mode (for drawing into sprites) */
   if (GSU.pvScreenBase + GSU.vScreenSize > GSU.pvRam + (GSU.nRamBanks * 65536))
      GSU.pvScreenBase =  GSU.pvRam + (GSU.nRamBanks * 65536) - GSU.vScreenSize;
   GSU.pfPlot = fx_apfPlotTable[GSU.vMode];
   GSU.pfRpix = fx_apfPlotTable[GSU.vMode + 5];

   fx_apfOpcodeTable[0x04c] = GSU.pfPlot;
   fx_apfOpcodeTable[0x14c] = GSU.pfRpix;
   fx_apfOpcodeTable[0x24c] = GSU.pfPlot;
   fx_apfOpcodeTable[0x34c] = GSU.pfRpix;

   fx_computeScreenPointers();
}

void fx_dirtySCBR(void)
{
   GSU.vSCBRDirty = true;
}

void fx_computeScreenPointers(void)
{
   if (GSU.vMode != GSU.vPrevMode || GSU.vPrevScreenHeight != GSU.vScreenHeight || GSU.vSCBRDirty)
   {
      int32_t i;
      GSU.vSCBRDirty = false;

      /* Make a list of pointers to the start of each screen column */
      switch (GSU.vScreenHeight)
      {
      case 128:
         switch (GSU.vMode)
         {
         case 0:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 4);
               GSU.x[i] = i << 8;
            }
            break;
         case 1:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 5);
               GSU.x[i] = i << 9;
            }
            break;
         case 2:
         case 3:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 6);
               GSU.x[i] = i << 10;
            }
            break;
         }
         break;
      case 160:
         switch (GSU.vMode)
         {
         case 0:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 4);
               GSU.x[i] = (i << 8) + (i << 6);
            }
            break;
         case 1:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 5);
               GSU.x[i] = (i << 9) + (i << 7);
            }
            break;
         case 2:
         case 3:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 6);
               GSU.x[i] = (i << 10) + (i << 8);
            }
            break;
         }
         break;
      case 192:
         switch (GSU.vMode)
         {
         case 0:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 4);
               GSU.x[i] = (i << 8) + (i << 7);
            }
            break;
         case 1:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 5);
               GSU.x[i] = (i << 9) + (i << 8);
            }
            break;
         case 2:
         case 3:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + (i << 6);
               GSU.x[i] = (i << 10) + (i << 9);
            }
            break;
         }
         break;
      case 256:
         switch (GSU.vMode)
         {
         case 0:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + ((i & 0x10) << 9) + ((i & 0xf) << 8);
               GSU.x[i] = ((i & 0x10) << 8) + ((i & 0xf) << 4);
            }
            break;
         case 1:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + ((i & 0x10) << 10) + ((i & 0xf) << 9);
               GSU.x[i] = ((i & 0x10) << 9) + ((i & 0xf) << 5);
            }
            break;
         case 2:
         case 3:
            for (i = 0; i < 32; i++)
            {
               GSU.apvScreen[i] = GSU.pvScreenBase + ((i & 0x10) << 11) + ((i & 0xf) << 10);
               GSU.x[i] = ((i & 0x10) << 10) + ((i & 0xf) << 6);
            }
            break;
         }
         break;
      }
      GSU.vPrevMode = GSU.vMode;
      GSU.vPrevScreenHeight = GSU.vScreenHeight;
   }
}

static void fx_writeRegisterSpaceAfterCheck(void)
{
   GSU.pvRegisters[30] = (uint8_t) R15;
   GSU.pvRegisters[31] = (uint8_t) (R15 >> 8);
   GSU.pvRegisters[GSU_SFR] = (uint8_t) GSU.vStatusReg;
   GSU.pvRegisters[GSU_SFR + 1] = (uint8_t) (GSU.vStatusReg >> 8);
   GSU.pvRegisters[GSU_PBR] = (uint8_t) GSU.vPrgBankReg;
}

static void fx_writeRegisterSpaceAfterUse(void)
{
   int32_t i;
   uint8_t* p = GSU.pvRegisters;
   for (i = 0; i < 15; i++)
   {
      *p++ = (uint8_t)GSU.avReg[i];
      *p++ = (uint8_t)(GSU.avReg[i] >> 8);
   }

   /* Update status register */
   if (USEX16(GSU.vZero) == 0)
      SF(Z);
   else
      CF(Z);
   if (GSU.vSign & 0x8000)
      SF(S);
   else
      CF(S);
   if (GSU.vOverflow >= 0x8000 || GSU.vOverflow < -0x8000)
      SF(OV);
   else
      CF(OV);
   if (GSU.vCarry)
      SF(CY);
   else
      CF(CY);

   p = GSU.pvRegisters;
   p[GSU_ROMBR] = (uint8_t)GSU.vRomBankReg;
   p[GSU_RAMBR] = (uint8_t)GSU.vRamBankReg;
   p[GSU_CBR] = (uint8_t)GSU.vCacheBaseReg;
   p[GSU_CBR + 1] = (uint8_t)(GSU.vCacheBaseReg >> 8);
}

/* Reset the FxChip */
void FxReset(FxInit_s* psFxInfo)
{
   int32_t i;
   /* Clear all internal variables */
   memset(&GSU, 0, sizeof(FxRegs_s));

   /* Set default registers */
   GSU.pvSreg = GSU.pvDreg = &R0;

   /* Set RAM and ROM pointers */
   GSU.pvRegisters = psFxInfo->pvRegisters;
   GSU.nRamBanks = psFxInfo->nRamBanks;
   GSU.pvRam = psFxInfo->pvRam;
   GSU.nRomBanks = psFxInfo->nRomBanks;
   GSU.pvRom = psFxInfo->pvRom;
   GSU.vPrevScreenHeight = ~0;
   GSU.vPrevMode = ~0;

   /* The GSU can't access more than 2mb (16mbits) */
   if (GSU.nRomBanks > 0x20)
      GSU.nRomBanks = 0x20;

   /* Clear FxChip register space */
   memset(GSU.pvRegisters, 0, 0x300);

   /* Set FxChip version Number */
   GSU.pvRegisters[0x3b] = 0;

   /* Make ROM bank table */
   for (i = 0; i < 256; i++)
   {
      uint32_t b = i & 0x7f;
      if (b >= 0x40)
      {
         if (GSU.nRomBanks > 2)
            b %= GSU.nRomBanks;
         else
            b &= 1;

         GSU.apvRomBank[i] = &GSU.pvRom[b << 16];
      }
      else
      {
         b %= GSU.nRomBanks * 2;
         GSU.apvRomBank[i] = &GSU.pvRom[(b << 16) + 0x200000];
      }
   }

   /* Make RAM bank table */
   for (i = 0; i < 4; i++)
   {
      GSU.apvRamBank[i] = &GSU.pvRam[(i % GSU.nRamBanks) << 16];
      GSU.apvRomBank[0x70 + i] = GSU.apvRamBank[i];
   }

   /* Start with a nop in the pipe */
   GSU.vPipe = 0x01;

   fx_readRegisterSpaceForCheck();
   fx_readRegisterSpaceForUse();
}

static bool fx_checkStartAddress(void)
{
   /* Check if we start inside the cache */
   if (GSU.bCacheActive && R15 >= GSU.vCacheBaseReg && R15 < (GSU.vCacheBaseReg + 512))
      return true;

   /*  Check if we're in an unused area */
   if (GSU.vPrgBankReg >= 0x60 && GSU.vPrgBankReg <= 0x6f)
      return false;
   if (GSU.vPrgBankReg >= 0x74)
      return false;

   /* Check if we're in RAM and the RAN flag is not set */
   if (GSU.vPrgBankReg >= 0x70 && GSU.vPrgBankReg <= 0x73 && !(SCMR & (1 << 3)))
      return false;

   /* If not, we're in ROM, so check if the RON flag is set */
   if (!(SCMR & (1 << 4)))
      return false;

   return true;
}

/* Execute until the next stop instruction */
int32_t FxEmulate(uint32_t nInstructions)
{
   uint32_t vCount;

   /* Read registers and initialize GSU session */
   fx_readRegisterSpaceForCheck();

   /* Check if the start address is valid */
   if (!fx_checkStartAddress())
   {
      CF(G);
      fx_writeRegisterSpaceAfterCheck();
      return 0;
   }

   fx_readRegisterSpaceForUse();

   /* Execute GSU session */
   CF(IRQ);

   vCount = fx_run(nInstructions);

   /* Store GSU registers */
   fx_writeRegisterSpaceAfterCheck();
   fx_writeRegisterSpaceAfterUse();

   /* Check for error code */
   return vCount;
}
