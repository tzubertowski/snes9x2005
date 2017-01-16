#include "../copyright"

#include "snes9x.h"
#include "memmap.h"
#include "cpuops.h"
#include "ppu.h"
#include "cpuexec.h"
#include "gfx.h"
#include "missing.h"
#include "apu.h"
#include "dma.h"
#include "fxemu.h"
#include "sa1.h"
#include "spc7110.h"

void S9xMainLoop_SA1_SFX(void);
void S9xMainLoop_SA1_NoSFX(void);
void S9xMainLoop_NoSA1_SFX(void);
void S9xMainLoop_NoSA1_NoSFX(void);

/*
 * This is a CATSFC modification inspired by a Snes9x-Euphoria modification.
 * The emulator selects a main loop based on the chips used in an entire
 * frame. This avoids the constant SA1.Executing and Settings.SuperFX checks.
 *
 * The original version of S9xMainLoop is S9xMainLoop_SA1_SFX below. Remember
 * to propagate modifications to the SA1_NoSFX, NoSA1_SFX and NoSA1_NoSFX
 * versions.
 */
void S9xMainLoop(void)
{
   if (Settings.SA1)
   {
      if (Settings.SuperFX) S9xMainLoop_SA1_SFX();
      else                  S9xMainLoop_SA1_NoSFX();
   }
   else
   {
      if (Settings.SuperFX) S9xMainLoop_NoSA1_SFX();
      else                  S9xMainLoop_NoSA1_NoSFX();
   }
}

void S9xMainLoop_SA1_SFX(void)
{
   for (;;)
   {
      APU_EXECUTE();

      if (CPU.Flags)
      {
         if (CPU.Flags & NMI_FLAG)
         {
            if (--CPU.NMICycleCount == 0)
            {
               CPU.Flags &= ~NMI_FLAG;
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               S9xOpcode_NMI();
            }
         }

         if (CPU.Flags & IRQ_PENDING_FLAG)
         {
            if (CPU.IRQCycleCount == 0)
            {
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               if (CPU.IRQActive && !Settings.DisableIRQ)
               {
                  if (!CheckFlag(IRQ))
                     S9xOpcode_IRQ();
               }
               else
                  CPU.Flags &= ~IRQ_PENDING_FLAG;
            }
            else
            {
               if (--CPU.IRQCycleCount == 0 && CheckFlag(IRQ))
                  CPU.IRQCycleCount = 1;
            }
         }

         if (CPU.Flags & SCAN_KEYS_FLAG)
            break;
      }

#ifdef CPU_SHUTDOWN
      CPU.PCAtOpcodeStart = CPU.PC;
#endif
      CPU.Cycles += CPU.MemSpeed;

      (*ICPU.S9xOpcodes [*CPU.PC++].S9xOpcode)();

      if (SA1.Executing)
         S9xSA1MainLoop();
      DO_HBLANK_CHECK_SFX();
   }

   ICPU.Registers.PC = CPU.PC - CPU.PCBase;
   S9xPackStatus();
#ifndef USE_BLARGG_APU
   IAPU.Registers.PC = IAPU.PC - IAPU.RAM;
   S9xAPUPackStatus();
#endif
   if (CPU.Flags & SCAN_KEYS_FLAG)
      CPU.Flags &= ~SCAN_KEYS_FLAG;

#ifdef DETECT_NASTY_FX_INTERLEAVE
   if (CPU.BRKTriggered && Settings.SuperFX && !CPU.TriedInterleavedMode2)
   {
      CPU.TriedInterleavedMode2 = true;
      CPU.BRKTriggered = false;
      S9xDeinterleaveMode2();
   }
#endif
}

void S9xMainLoop_SA1_NoSFX(void)
{
   for (;;)
   {
      APU_EXECUTE();

      if (CPU.Flags)
      {
         if (CPU.Flags & NMI_FLAG)
         {
            if (--CPU.NMICycleCount == 0)
            {
               CPU.Flags &= ~NMI_FLAG;
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               S9xOpcode_NMI();
            }
         }

         if (CPU.Flags & IRQ_PENDING_FLAG)
         {
            if (CPU.IRQCycleCount == 0)
            {
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               if (CPU.IRQActive && !Settings.DisableIRQ)
               {
                  if (!CheckFlag(IRQ))
                     S9xOpcode_IRQ();
               }
               else
                  CPU.Flags &= ~IRQ_PENDING_FLAG;
            }
            else
            {
               if (--CPU.IRQCycleCount == 0 && CheckFlag(IRQ))
                  CPU.IRQCycleCount = 1;
            }
         }

         if (CPU.Flags & SCAN_KEYS_FLAG)
            break;
      }

#ifdef CPU_SHUTDOWN
      CPU.PCAtOpcodeStart = CPU.PC;
#endif
      CPU.Cycles += CPU.MemSpeed;

      (*ICPU.S9xOpcodes [*CPU.PC++].S9xOpcode)();

      if (SA1.Executing)
         S9xSA1MainLoop();
      DO_HBLANK_CHECK_NoSFX();
   }

   ICPU.Registers.PC = CPU.PC - CPU.PCBase;
   S9xPackStatus();
#ifndef USE_BLARGG_APU
   IAPU.Registers.PC = IAPU.PC - IAPU.RAM;
   S9xAPUPackStatus();
#endif
   if (CPU.Flags & SCAN_KEYS_FLAG)
      CPU.Flags &= ~SCAN_KEYS_FLAG;
}

void S9xMainLoop_NoSA1_SFX(void)
{
   for (;;)
   {
      APU_EXECUTE();

      if (CPU.Flags)
      {
         if (CPU.Flags & NMI_FLAG)
         {
            if (--CPU.NMICycleCount == 0)
            {
               CPU.Flags &= ~NMI_FLAG;
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               S9xOpcode_NMI();
            }
         }

         if (CPU.Flags & IRQ_PENDING_FLAG)
         {
            if (CPU.IRQCycleCount == 0)
            {
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               if (CPU.IRQActive && !Settings.DisableIRQ)
               {
                  if (!CheckFlag(IRQ))
                     S9xOpcode_IRQ();
               }
               else
                  CPU.Flags &= ~IRQ_PENDING_FLAG;
            }
            else
            {
               if (--CPU.IRQCycleCount == 0 && CheckFlag(IRQ))
                  CPU.IRQCycleCount = 1;
            }
         }

         if (CPU.Flags & SCAN_KEYS_FLAG)
            break;
      }

#ifdef CPU_SHUTDOWN
      CPU.PCAtOpcodeStart = CPU.PC;
#endif
      CPU.Cycles += CPU.MemSpeed;

      (*ICPU.S9xOpcodes [*CPU.PC++].S9xOpcode)();

      DO_HBLANK_CHECK_SFX();
   }

   ICPU.Registers.PC = CPU.PC - CPU.PCBase;
   S9xPackStatus();
#ifndef USE_BLARGG_APU
   IAPU.Registers.PC = IAPU.PC - IAPU.RAM;
   S9xAPUPackStatus();
#endif
   if (CPU.Flags & SCAN_KEYS_FLAG)
      CPU.Flags &= ~SCAN_KEYS_FLAG;

#ifdef DETECT_NASTY_FX_INTERLEAVE
   if (CPU.BRKTriggered && Settings.SuperFX && !CPU.TriedInterleavedMode2)
   {
      CPU.TriedInterleavedMode2 = true;
      CPU.BRKTriggered = false;
      S9xDeinterleaveMode2();
   }
#endif
}

void S9xMainLoop_NoSA1_NoSFX(void)
{
   for (;;)
   {
      APU_EXECUTE();

      if (CPU.Flags)
      {
         if (CPU.Flags & NMI_FLAG)
         {
            if (--CPU.NMICycleCount == 0)
            {
               CPU.Flags &= ~NMI_FLAG;
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               S9xOpcode_NMI();
            }
         }

         if (CPU.Flags & IRQ_PENDING_FLAG)
         {
            if (CPU.IRQCycleCount == 0)
            {
               if (CPU.WaitingForInterrupt)
               {
                  CPU.WaitingForInterrupt = false;
                  CPU.PC++;
               }
               if (CPU.IRQActive && !Settings.DisableIRQ)
               {
                  if (!CheckFlag(IRQ))
                     S9xOpcode_IRQ();
               }
               else
                  CPU.Flags &= ~IRQ_PENDING_FLAG;
            }
            else
            {
               if (--CPU.IRQCycleCount == 0 && CheckFlag(IRQ))
                  CPU.IRQCycleCount = 1;
            }
         }

         if (CPU.Flags & SCAN_KEYS_FLAG)
            break;
      }

#ifdef CPU_SHUTDOWN
      CPU.PCAtOpcodeStart = CPU.PC;
#endif
      CPU.Cycles += CPU.MemSpeed;

      (*ICPU.S9xOpcodes [*CPU.PC++].S9xOpcode)();

      DO_HBLANK_CHECK_NoSFX();
   }

   ICPU.Registers.PC = CPU.PC - CPU.PCBase;
   S9xPackStatus();
#ifndef USE_BLARGG_APU
   IAPU.Registers.PC = IAPU.PC - IAPU.RAM;
   S9xAPUPackStatus();
#endif
   if (CPU.Flags & SCAN_KEYS_FLAG)
      CPU.Flags &= ~SCAN_KEYS_FLAG;
}

void S9xSetIRQ(uint32_t source)
{
   CPU.IRQActive |= source;
   CPU.Flags |= IRQ_PENDING_FLAG;
   CPU.IRQCycleCount = 3;
   if (CPU.WaitingForInterrupt)
   {
      // Force IRQ to trigger immediately after WAI -
      // Final Fantasy Mystic Quest crashes without this.
      CPU.IRQCycleCount = 0;
      CPU.WaitingForInterrupt = false;
      CPU.PC++;
   }
}

void S9xClearIRQ(uint32_t source)
{
   CLEAR_IRQ_SOURCE(source);
}

/*
 * This is a CATSFC modification inspired by a Snes9x-Euphoria modification.
 * The emulator selects an HBlank processor based on the chips used in an
 * entire frame. This avoids the constant Settings.SuperFX checks.
 *
 * The original version of S9xDoHBlankProcessing is S9xDoHBlankProcessing_SFX
 * below. Remember to propagate modifications to the NoSFX version.
 */
void S9xDoHBlankProcessing_SFX()
{
#ifdef CPU_SHUTDOWN
   CPU.WaitCounter++;
#endif
   switch (CPU.WhichEvent)
   {
   case HBLANK_START_EVENT:
      if (IPPU.HDMA && CPU.V_Counter <= PPU.ScreenHeight)
         IPPU.HDMA = S9xDoHDMA(IPPU.HDMA);

      break;

   case HBLANK_END_EVENT:
      S9xSuperFXExec();

#ifndef USE_BLARGG_APU
      CPU.Cycles -= Settings.H_Max;
      if (IAPU.APUExecuting)
      {
         APU.Cycles -= Settings.H_Max;
#ifdef MK_APU
         S9xCatchupCount();
#endif
      }
      else
         APU.Cycles = 0;
#else
      S9xAPUExecute();
      CPU.Cycles -= Settings.H_Max;
      S9xAPUSetReferenceTime(CPU.Cycles);
#endif
      CPU.NextEvent = -1;
      ICPU.Scanline++;

      if (++CPU.V_Counter >= (Settings.PAL ? SNES_MAX_PAL_VCOUNTER :
                              SNES_MAX_NTSC_VCOUNTER))
      {
         CPU.V_Counter = 0;
         Memory.FillRAM[0x213F] ^= 0x80;
         PPU.RangeTimeOver = 0;
         CPU.NMIActive = false;
         ICPU.Frame++;
         PPU.HVBeamCounterLatched = 0;
         CPU.Flags |= SCAN_KEYS_FLAG;
         S9xStartHDMA();
      }

      if (PPU.VTimerEnabled && !PPU.HTimerEnabled && CPU.V_Counter == PPU.IRQVBeamPos)
         S9xSetIRQ(PPU_V_BEAM_IRQ_SOURCE);

      if (CPU.V_Counter == PPU.ScreenHeight + FIRST_VISIBLE_LINE)
      {
         // Start of V-blank
         S9xEndScreenRefresh();
         IPPU.HDMA = 0;
         // Bits 7 and 6 of $4212 are computed when read in S9xGetPPU.
         missing.dma_this_frame = 0;
         IPPU.MaxBrightness = PPU.Brightness;
         PPU.ForcedBlanking = (Memory.FillRAM [0x2100] >> 7) & 1;

         if (!PPU.ForcedBlanking)
         {
            PPU.OAMAddr = PPU.SavedOAMAddr;

            uint8_t tmp = 0;
            if (PPU.OAMPriorityRotation)
               tmp = (PPU.OAMAddr & 0xFE) >> 1;
            if ((PPU.OAMFlip & 1) || PPU.FirstSprite != tmp)
            {
               PPU.FirstSprite = tmp;
               IPPU.OBJChanged = true;
            }

            PPU.OAMFlip = 0;
         }

         Memory.FillRAM[0x4210] = 0x80 | Model->_5A22;
         if (Memory.FillRAM[0x4200] & 0x80)
         {
            CPU.NMIActive = true;
            CPU.Flags |= NMI_FLAG;
            CPU.NMICycleCount = CPU.NMITriggerPoint;
         }

#ifdef OLD_SNAPSHOT_CODE
         if (CPU.Flags & SAVE_SNAPSHOT_FLAG)
         {
            CPU.Flags &= ~SAVE_SNAPSHOT_FLAG;
            Registers.PC = CPU.PC - CPU.PCBase;
            S9xPackStatus();
            S9xAPUPackStatus();
            Snapshot(NULL);
         }
#endif
      }

      if (CPU.V_Counter == PPU.ScreenHeight + 3)
         S9xUpdateJoypads();

      if (CPU.V_Counter == FIRST_VISIBLE_LINE)
      {
         Memory.FillRAM[0x4210] = Model->_5A22;
         CPU.Flags &= ~NMI_FLAG;
         S9xStartScreenRefresh();
      }
      if (CPU.V_Counter >= FIRST_VISIBLE_LINE &&
            CPU.V_Counter < PPU.ScreenHeight + FIRST_VISIBLE_LINE)
         RenderLine(CPU.V_Counter - FIRST_VISIBLE_LINE);

#ifndef USE_BLARGG_APU
      // Use TimerErrorCounter to skip update of SPC700 timers once
      // every 128 updates. Needed because this section of code is called
      // once every emulated 63.5 microseconds, which coresponds to
      // 15.750KHz, but the SPC700 timers need to be updated at multiples
      // of 8KHz, hence the error correction.
      {
         if (APU.TimerEnabled [2])
         {
            APU.Timer [2] += 4;
            while (APU.Timer [2] >= APU.TimerTarget [2])
            {
               IAPU.RAM [0xff] = (IAPU.RAM [0xff] + 1) & 0xf;
               APU.Timer [2] -= APU.TimerTarget [2];
#ifdef SPC700_SHUTDOWN
               IAPU.WaitCounter++;
               IAPU.APUExecuting = true;
#endif
            }
         }
         if (CPU.V_Counter & 1)
         {
            if (APU.TimerEnabled [0])
            {
               APU.Timer [0]++;
               if (APU.Timer [0] >= APU.TimerTarget [0])
               {
                  IAPU.RAM [0xfd] = (IAPU.RAM [0xfd] + 1) & 0xf;
                  APU.Timer [0] = 0;
#ifdef SPC700_SHUTDOWN
                  IAPU.WaitCounter++;
                  IAPU.APUExecuting = true;
#endif
               }
            }
            if (APU.TimerEnabled [1])
            {
               APU.Timer [1]++;
               if (APU.Timer [1] >= APU.TimerTarget [1])
               {
                  IAPU.RAM [0xfe] = (IAPU.RAM [0xfe] + 1) & 0xf;
                  APU.Timer [1] = 0;
#ifdef SPC700_SHUTDOWN
                  IAPU.WaitCounter++;
                  IAPU.APUExecuting = true;
#endif
               }
            }
         }
      }
#endif // #ifndef USE_BLARGG_APU
      break;

   case HTIMER_BEFORE_EVENT:
   case HTIMER_AFTER_EVENT:
      if (PPU.HTimerEnabled && (!PPU.VTimerEnabled
                                || CPU.V_Counter == PPU.IRQVBeamPos))
         S9xSetIRQ(PPU_H_BEAM_IRQ_SOURCE);
      break;
   }

   S9xReschedule();
}
void S9xDoHBlankProcessing_NoSFX()
{
#ifdef CPU_SHUTDOWN
   CPU.WaitCounter++;
#endif
   switch (CPU.WhichEvent)
   {
   case HBLANK_START_EVENT:
      if (IPPU.HDMA && CPU.V_Counter <= PPU.ScreenHeight)
         IPPU.HDMA = S9xDoHDMA(IPPU.HDMA);

      break;

   case HBLANK_END_EVENT:

#ifndef USE_BLARGG_APU
      CPU.Cycles -= Settings.H_Max;
      if (IAPU.APUExecuting)
      {
         APU.Cycles -= Settings.H_Max;
#ifdef MK_APU
         S9xCatchupCount();
#endif
      }
      else
         APU.Cycles = 0;
#else
      S9xAPUExecute();
      CPU.Cycles -= Settings.H_Max;
      S9xAPUSetReferenceTime(CPU.Cycles);
#endif

      CPU.NextEvent = -1;
      ICPU.Scanline++;

      if (++CPU.V_Counter >= (Settings.PAL ? SNES_MAX_PAL_VCOUNTER :
                              SNES_MAX_NTSC_VCOUNTER))
      {
         CPU.V_Counter = 0;
         Memory.FillRAM[0x213F] ^= 0x80;
         PPU.RangeTimeOver = 0;
         CPU.NMIActive = false;
         ICPU.Frame++;
         PPU.HVBeamCounterLatched = 0;
         CPU.Flags |= SCAN_KEYS_FLAG;
         S9xStartHDMA();
      }

      if (PPU.VTimerEnabled && !PPU.HTimerEnabled && CPU.V_Counter == PPU.IRQVBeamPos)
         S9xSetIRQ(PPU_V_BEAM_IRQ_SOURCE);

      if (CPU.V_Counter == PPU.ScreenHeight + FIRST_VISIBLE_LINE)
      {
         // Start of V-blank
         S9xEndScreenRefresh();
         IPPU.HDMA = 0;
         // Bits 7 and 6 of $4212 are computed when read in S9xGetPPU.
         missing.dma_this_frame = 0;
         IPPU.MaxBrightness = PPU.Brightness;
         PPU.ForcedBlanking = (Memory.FillRAM [0x2100] >> 7) & 1;

         if (!PPU.ForcedBlanking)
         {
            PPU.OAMAddr = PPU.SavedOAMAddr;

            uint8_t tmp = 0;
            if (PPU.OAMPriorityRotation)
               tmp = (PPU.OAMAddr & 0xFE) >> 1;
            if ((PPU.OAMFlip & 1) || PPU.FirstSprite != tmp)
            {
               PPU.FirstSprite = tmp;
               IPPU.OBJChanged = true;
            }

            PPU.OAMFlip = 0;
         }

         Memory.FillRAM[0x4210] = 0x80 | Model->_5A22;
         if (Memory.FillRAM[0x4200] & 0x80)
         {
            CPU.NMIActive = true;
            CPU.Flags |= NMI_FLAG;
            CPU.NMICycleCount = CPU.NMITriggerPoint;
         }

#ifdef OLD_SNAPSHOT_CODE
         if (CPU.Flags & SAVE_SNAPSHOT_FLAG)
         {
            CPU.Flags &= ~SAVE_SNAPSHOT_FLAG;
            Registers.PC = CPU.PC - CPU.PCBase;
            S9xPackStatus();
            S9xAPUPackStatus();
            Snapshot(NULL);
         }
#endif
      }

      if (CPU.V_Counter == PPU.ScreenHeight + 3)
         S9xUpdateJoypads();

      if (CPU.V_Counter == FIRST_VISIBLE_LINE)
      {
         Memory.FillRAM[0x4210] = Model->_5A22;
         CPU.Flags &= ~NMI_FLAG;
         S9xStartScreenRefresh();
      }
      if (CPU.V_Counter >= FIRST_VISIBLE_LINE &&
            CPU.V_Counter < PPU.ScreenHeight + FIRST_VISIBLE_LINE)
         RenderLine(CPU.V_Counter - FIRST_VISIBLE_LINE);
      // Use TimerErrorCounter to skip update of SPC700 timers once
      // every 128 updates. Needed because this section of code is called
      // once every emulated 63.5 microseconds, which coresponds to
      // 15.750KHz, but the SPC700 timers need to be updated at multiples
      // of 8KHz, hence the error correction.
#ifndef USE_BLARGG_APU
      {
         if (APU.TimerEnabled [2])
         {
            APU.Timer [2] += 4;
            while (APU.Timer [2] >= APU.TimerTarget [2])
            {
               IAPU.RAM [0xff] = (IAPU.RAM [0xff] + 1) & 0xf;
               APU.Timer [2] -= APU.TimerTarget [2];
#ifdef SPC700_SHUTDOWN
               IAPU.WaitCounter++;
               IAPU.APUExecuting = true;
#endif
            }
         }
         if (CPU.V_Counter & 1)
         {
            if (APU.TimerEnabled [0])
            {
               APU.Timer [0]++;
               if (APU.Timer [0] >= APU.TimerTarget [0])
               {
                  IAPU.RAM [0xfd] = (IAPU.RAM [0xfd] + 1) & 0xf;
                  APU.Timer [0] = 0;
#ifdef SPC700_SHUTDOWN
                  IAPU.WaitCounter++;
                  IAPU.APUExecuting = true;
#endif
               }
            }
            if (APU.TimerEnabled [1])
            {
               APU.Timer [1]++;
               if (APU.Timer [1] >= APU.TimerTarget [1])
               {
                  IAPU.RAM [0xfe] = (IAPU.RAM [0xfe] + 1) & 0xf;
                  APU.Timer [1] = 0;
#ifdef SPC700_SHUTDOWN
                  IAPU.WaitCounter++;
                  IAPU.APUExecuting = true;
#endif
               }
            }
         }
      }
#endif
      break;

   case HTIMER_BEFORE_EVENT:
   case HTIMER_AFTER_EVENT:
      if (PPU.HTimerEnabled && (!PPU.VTimerEnabled
                                || CPU.V_Counter == PPU.IRQVBeamPos))
         S9xSetIRQ(PPU_H_BEAM_IRQ_SOURCE);
      break;
   }

   S9xReschedule();
}
