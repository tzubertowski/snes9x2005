#include "../copyright"

#ifndef USE_BLARGG_APU

#include "snes9x.h"
#include "spc700.h"
#include "apu.h"
#include "soundux.h"
#include "cpuexec.h"

extern int32_t NoiseFreq [32];

bool S9xInitAPU()
{
   IAPU.RAM = (uint8_t*) malloc(0x10000);

   if (!IAPU.RAM)
   {
      S9xDeinitAPU();
      return (false);
   }

   memset(IAPU.RAM, 0, 0x10000);

   return (true);
}

void S9xDeinitAPU()
{
   if (IAPU.RAM)
   {
      free((char*) IAPU.RAM);
      IAPU.RAM = NULL;
   }
}

uint8_t APUROM [64];

void S9xResetAPU()
{

   int i, j;

   Settings.APUEnabled = Settings.NextAPUEnabled;

   memset(IAPU.RAM, 0, 0x100);
   memset(IAPU.RAM + 0x20, 0xFF, 0x20);
   memset(IAPU.RAM + 0x60, 0xFF, 0x20);
   memset(IAPU.RAM + 0xA0, 0xFF, 0x20);
   memset(IAPU.RAM + 0xE0, 0xFF, 0x20);

   for (i = 1; i < 256; i++)
      memcpy(IAPU.RAM + (i << 8), IAPU.RAM, 0x100);

   memset(APU.OutPorts, 0, 4);
   IAPU.DirectPage = IAPU.RAM;
   // memmove converted: Different mallocs [Neb]
   // DS2 DMA notes: The APU ROM is not 32-byte aligned [Neb]
   memcpy(&IAPU.RAM [0xffc0], APUROM, sizeof(APUROM));
   // memmove converted: Different mallocs [Neb]
   // DS2 DMA notes: The APU ROM is not 32-byte aligned [Neb]
   memcpy(APU.ExtraRAM, APUROM, sizeof(APUROM));
   IAPU.PC = IAPU.RAM + IAPU.RAM [0xfffe] + (IAPU.RAM [0xffff] << 8);
   APU.Cycles = 0;
   IAPU.Registers.YA.W = 0;
   IAPU.Registers.X = 0;
   IAPU.Registers.S = 0xff;
   IAPU.Registers.P = 0;
   S9xAPUUnpackStatus();
   IAPU.Registers.PC = 0;
   IAPU.APUExecuting = Settings.APUEnabled;
#ifdef SPC700_SHUTDOWN
   IAPU.WaitAddress1 = NULL;
   IAPU.WaitAddress2 = NULL;
   IAPU.WaitCounter = 0;
#endif
   APU.ShowROM = true;
   IAPU.RAM [0xf1] = 0x80;

   for (i = 0; i < 3; i++)
   {
      APU.TimerEnabled [i] = false;
      APU.TimerValueWritten [i] = 0;
      APU.TimerTarget [i] = 0;
      APU.Timer [i] = 0;
   }
   for (j = 0; j < 0x80; j++)
      APU.DSP [j] = 0;

   IAPU.TwoCycles = IAPU.OneCycle * 2;

   for (i = 0; i < 256; i++)
      S9xAPUCycles [i] = S9xAPUCycleLengths [i] * IAPU.OneCycle;

   APU.DSP [APU_ENDX] = 0;
   APU.DSP [APU_KOFF] = 0;
   APU.DSP [APU_KON] = 0;
   APU.DSP [APU_FLG] = APU_MUTE | APU_ECHO_DISABLED;
   APU.KeyedChannels = 0;

   S9xResetSound(true);
   S9xSetEchoEnable(0);
}

void S9xSetAPUDSP(uint8_t byte)
{
   uint8_t reg = IAPU.RAM [0xf2];
   static uint8_t KeyOn;
   static uint8_t KeyOnPrev;
   int i;

   switch (reg)
   {
   case APU_FLG:
      if (byte & APU_SOFT_RESET)
      {
         APU.DSP [reg] = APU_MUTE | APU_ECHO_DISABLED | (byte & 0x1f);
         APU.DSP [APU_ENDX] = 0;
         APU.DSP [APU_KOFF] = 0;
         APU.DSP [APU_KON] = 0;
         S9xSetEchoWriteEnable(false);

         // Kill sound
         S9xResetSound(false);
      }
      else
      {
         S9xSetEchoWriteEnable(!(byte & APU_ECHO_DISABLED));

         SoundData.noise_hertz = NoiseFreq [byte & 0x1f];
         for (i = 0; i < 8; i++)
         {
            if (SoundData.channels [i].type == SOUND_NOISE)
               S9xSetSoundFrequency(i, SoundData.noise_hertz);
         }
      }
      break;
   case APU_NON:
      if (byte != APU.DSP [APU_NON])
      {
         int c;
         uint8_t mask = 1;
         for (c = 0; c < 8; c++, mask <<= 1)
         {
            int type;

            if (byte & mask)
               type = SOUND_NOISE;
            else
               type = SOUND_SAMPLE;

            S9xSetSoundType(c, type);
         }
      }
      break;
   case APU_MVOL_LEFT:
      if (byte != APU.DSP [APU_MVOL_LEFT])
      {
         S9xSetMasterVolume((signed char) byte,
                            (signed char) APU.DSP [APU_MVOL_RIGHT]);
      }
      break;
   case APU_MVOL_RIGHT:
      if (byte != APU.DSP [APU_MVOL_RIGHT])
      {
         S9xSetMasterVolume((signed char) APU.DSP [APU_MVOL_LEFT],
                            (signed char) byte);
      }
      break;
   case APU_EVOL_LEFT:
      if (byte != APU.DSP [APU_EVOL_LEFT])
      {
         S9xSetEchoVolume((signed char) byte,
                          (signed char) APU.DSP [APU_EVOL_RIGHT]);
      }
      break;
   case APU_EVOL_RIGHT:
      if (byte != APU.DSP [APU_EVOL_RIGHT])
      {
         S9xSetEchoVolume((signed char) APU.DSP [APU_EVOL_LEFT],
                          (signed char) byte);
      }
      break;
   case APU_ENDX:
      byte = 0;
      break;

   case APU_KOFF:
      //    if (byte)
   {
      int c;
      uint8_t mask = 1;
      for (c = 0; c < 8; c++, mask <<= 1)
      {
         if ((byte & mask) != 0)
         {
            if (APU.KeyedChannels & mask)
            {
               {
                  KeyOnPrev &= ~mask;
                  APU.KeyedChannels &= ~mask;
                  APU.DSP [APU_KON] &= ~mask;
                  //APU.DSP [APU_KOFF] |= mask;
                  S9xSetSoundKeyOff(c);
               }
            }
         }
         else if ((KeyOnPrev & mask) != 0)
         {
            KeyOnPrev &= ~mask;
            APU.KeyedChannels |= mask;
            //APU.DSP [APU_KON] |= mask;
            APU.DSP [APU_KOFF] &= ~mask;
            APU.DSP [APU_ENDX] &= ~mask;
            S9xPlaySample(c);
         }
      }
   }
      //KeyOnPrev=0;
   APU.DSP [APU_KOFF] = byte;
   return;
   case APU_KON:
      if (byte)
      {
         int c;
         uint8_t mask = 1;
         for (c = 0; c < 8; c++, mask <<= 1)
         {
            if ((byte & mask) != 0)
            {
               // Pac-In-Time requires that channels can be key-on
               // regardeless of their current state.
               if ((APU.DSP [APU_KOFF] & mask) == 0)
               {
                  KeyOnPrev &= ~mask;
                  APU.KeyedChannels |= mask;
                  //APU.DSP [APU_KON] |= mask;
                  //APU.DSP [APU_KOFF] &= ~mask;
                  APU.DSP [APU_ENDX] &= ~mask;
                  S9xPlaySample(c);
               }
               else KeyOn |= mask;
            }
         }
      }
      return;

   case APU_VOL_LEFT + 0x00:
   case APU_VOL_LEFT + 0x10:
   case APU_VOL_LEFT + 0x20:
   case APU_VOL_LEFT + 0x30:
   case APU_VOL_LEFT + 0x40:
   case APU_VOL_LEFT + 0x50:
   case APU_VOL_LEFT + 0x60:
   case APU_VOL_LEFT + 0x70:
      // At Shin Megami Tensei suggestion 6/11/00
      // if (byte != APU.DSP [reg])
   {
      S9xSetSoundVolume(reg >> 4, (signed char) byte,
                        (signed char) APU.DSP [reg + 1]);
   }
   break;
   case APU_VOL_RIGHT + 0x00:
   case APU_VOL_RIGHT + 0x10:
   case APU_VOL_RIGHT + 0x20:
   case APU_VOL_RIGHT + 0x30:
   case APU_VOL_RIGHT + 0x40:
   case APU_VOL_RIGHT + 0x50:
   case APU_VOL_RIGHT + 0x60:
   case APU_VOL_RIGHT + 0x70:
      // At Shin Megami Tensei suggestion 6/11/00
      // if (byte != APU.DSP [reg])
   {
      S9xSetSoundVolume(reg >> 4, (signed char) APU.DSP [reg - 1],
                        (signed char) byte);
   }
   break;

   case APU_P_LOW + 0x00:
   case APU_P_LOW + 0x10:
   case APU_P_LOW + 0x20:
   case APU_P_LOW + 0x30:
   case APU_P_LOW + 0x40:
   case APU_P_LOW + 0x50:
   case APU_P_LOW + 0x60:
   case APU_P_LOW + 0x70:
      S9xSetSoundHertz(reg >> 4,
                       ((byte + (APU.DSP [reg + 1] << 8)) & FREQUENCY_MASK) * 8);
      break;

   case APU_P_HIGH + 0x00:
   case APU_P_HIGH + 0x10:
   case APU_P_HIGH + 0x20:
   case APU_P_HIGH + 0x30:
   case APU_P_HIGH + 0x40:
   case APU_P_HIGH + 0x50:
   case APU_P_HIGH + 0x60:
   case APU_P_HIGH + 0x70:
      S9xSetSoundHertz(reg >> 4,
                       (((byte << 8) + APU.DSP [reg - 1]) & FREQUENCY_MASK) * 8);
      break;

   case APU_SRCN + 0x00:
   case APU_SRCN + 0x10:
   case APU_SRCN + 0x20:
   case APU_SRCN + 0x30:
   case APU_SRCN + 0x40:
   case APU_SRCN + 0x50:
   case APU_SRCN + 0x60:
   case APU_SRCN + 0x70:
      if (byte != APU.DSP [reg])
         S9xSetSoundSample(reg >> 4, byte);
      break;

   case APU_ADSR1 + 0x00:
   case APU_ADSR1 + 0x10:
   case APU_ADSR1 + 0x20:
   case APU_ADSR1 + 0x30:
   case APU_ADSR1 + 0x40:
   case APU_ADSR1 + 0x50:
   case APU_ADSR1 + 0x60:
   case APU_ADSR1 + 0x70:
      if (byte != APU.DSP [reg])
      {
         {
            S9xFixEnvelope(reg >> 4, APU.DSP [reg + 2], byte,
                           APU.DSP [reg + 1]);
         }
      }
      break;

   case APU_ADSR2 + 0x00:
   case APU_ADSR2 + 0x10:
   case APU_ADSR2 + 0x20:
   case APU_ADSR2 + 0x30:
   case APU_ADSR2 + 0x40:
   case APU_ADSR2 + 0x50:
   case APU_ADSR2 + 0x60:
   case APU_ADSR2 + 0x70:
      if (byte != APU.DSP [reg])
      {
         {
            S9xFixEnvelope(reg >> 4, APU.DSP [reg + 1], APU.DSP [reg - 1],
                           byte);
         }
      }
      break;

   case APU_GAIN + 0x00:
   case APU_GAIN + 0x10:
   case APU_GAIN + 0x20:
   case APU_GAIN + 0x30:
   case APU_GAIN + 0x40:
   case APU_GAIN + 0x50:
   case APU_GAIN + 0x60:
   case APU_GAIN + 0x70:
      if (byte != APU.DSP [reg])
      {
         {
            S9xFixEnvelope(reg >> 4, byte, APU.DSP [reg - 2],
                           APU.DSP [reg - 1]);
         }
      }
      break;

   case APU_ENVX + 0x00:
   case APU_ENVX + 0x10:
   case APU_ENVX + 0x20:
   case APU_ENVX + 0x30:
   case APU_ENVX + 0x40:
   case APU_ENVX + 0x50:
   case APU_ENVX + 0x60:
   case APU_ENVX + 0x70:
      break;

   case APU_OUTX + 0x00:
   case APU_OUTX + 0x10:
   case APU_OUTX + 0x20:
   case APU_OUTX + 0x30:
   case APU_OUTX + 0x40:
   case APU_OUTX + 0x50:
   case APU_OUTX + 0x60:
   case APU_OUTX + 0x70:
      break;

   case APU_DIR:
      break;

   case APU_PMON:
      if (byte != APU.DSP [APU_PMON])
         S9xSetFrequencyModulationEnable(byte);
      break;

   case APU_EON:
      if (byte != APU.DSP [APU_EON])
         S9xSetEchoEnable(byte);
      break;

   case APU_EFB:
      S9xSetEchoFeedback((signed char) byte);
      break;

   case APU_ESA:
      break;

   case APU_EDL:
      S9xSetEchoDelay(byte & 0xf);
      break;

   case APU_C0:
   case APU_C1:
   case APU_C2:
   case APU_C3:
   case APU_C4:
   case APU_C5:
   case APU_C6:
   case APU_C7:
      S9xSetFilterCoefficient(reg >> 4, (signed char) byte);
      break;
   default:
      // XXX
      //printf ("Write %02x to unknown APU register %02x\n", byte, reg);
      break;
   }

   KeyOnPrev |= KeyOn;
   KeyOn = 0;

   if (reg < 0x80)
      APU.DSP [reg] = byte;
}

void S9xFixEnvelope(int channel, uint8_t gain, uint8_t adsr1, uint8_t adsr2)
{
   if (adsr1 & 0x80)
   {
      // ADSR mode
      static unsigned long AttackRate [16] =
      {
         4100, 2600, 1500, 1000, 640, 380, 260, 160,
         96, 64, 40, 24, 16, 10, 6, 1
      };
      static unsigned long DecayRate [8] =
      {
         1200, 740, 440, 290, 180, 110, 74, 37
      };
      static unsigned long SustainRate [32] =
      {
         ~0, 38000, 28000, 24000, 19000, 14000, 12000, 9400,
         7100, 5900, 4700, 3500, 2900, 2400, 1800, 1500,
         1200, 880, 740, 590, 440, 370, 290, 220,
         180, 150, 110, 92, 74, 55, 37, 18
      };
      // XXX: can DSP be switched to ADSR mode directly from GAIN/INCREASE/
      // DECREASE mode? And if so, what stage of the sequence does it start
      // at?
      if (S9xSetSoundMode(channel, MODE_ADSR))
      {
         // Hack for ROMs that use a very short attack rate, key on a
         // channel, then switch to decay mode. e.g. Final Fantasy II.

         int attack = AttackRate [adsr1 & 0xf];

         if (attack == 1 && (!Settings.SoundSync))
            attack = 0;

         S9xSetSoundADSR(channel, attack,
                         DecayRate [(adsr1 >> 4) & 7],
                         SustainRate [adsr2 & 0x1f],
                         (adsr2 >> 5) & 7, 8);
      }
   }
   else
   {
      // Gain mode
      if ((gain & 0x80) == 0)
      {
         if (S9xSetSoundMode(channel, MODE_GAIN))
         {
            S9xSetEnvelopeRate(channel, 0, 0, gain & 0x7f);
            S9xSetEnvelopeHeight(channel, gain & 0x7f);
         }
      }
      else
      {
         static unsigned long IncreaseRate [32] =
         {
            ~0, 4100, 3100, 2600, 2000, 1500, 1300, 1000,
            770, 640, 510, 380, 320, 260, 190, 160,
            130, 96, 80, 64, 48, 40, 32, 24,
            20, 16, 12, 10, 8, 6, 4, 2
         };
         static unsigned long DecreaseRateExp [32] =
         {
            ~0, 38000, 28000, 24000, 19000, 14000, 12000, 9400,
            7100, 5900, 4700, 3500, 2900, 2400, 1800, 1500,
            1200, 880, 740, 590, 440, 370, 290, 220,
            180, 150, 110, 92, 74, 55, 37, 18
         };
         if (gain & 0x40)
         {
            // Increase mode
            if (S9xSetSoundMode(channel, (gain & 0x20) ?
                                MODE_INCREASE_BENT_LINE :
                                MODE_INCREASE_LINEAR))
            {
               S9xSetEnvelopeRate(channel, IncreaseRate [gain & 0x1f],
                                  1, 127);
            }
         }
         else
         {
            uint32_t rate = (gain & 0x20) ? DecreaseRateExp [gain & 0x1f] / 2 :
                          IncreaseRate [gain & 0x1f];
            int mode = (gain & 0x20) ? MODE_DECREASE_EXPONENTIAL
                       : MODE_DECREASE_LINEAR;

            if (S9xSetSoundMode(channel, mode))
               S9xSetEnvelopeRate(channel, rate, -1, 0);
         }
      }
   }
}

void S9xSetAPUControl(uint8_t byte)
{
   //if (byte & 0x40)
   //printf ("*** Special SPC700 timing enabled\n");
   if ((byte & 1) != 0 && !APU.TimerEnabled [0])
   {
      APU.Timer [0] = 0;
      IAPU.RAM [0xfd] = 0;
      if ((APU.TimerTarget [0] = IAPU.RAM [0xfa]) == 0)
         APU.TimerTarget [0] = 0x100;
   }
   if ((byte & 2) != 0 && !APU.TimerEnabled [1])
   {
      APU.Timer [1] = 0;
      IAPU.RAM [0xfe] = 0;
      if ((APU.TimerTarget [1] = IAPU.RAM [0xfb]) == 0)
         APU.TimerTarget [1] = 0x100;
   }
   if ((byte & 4) != 0 && !APU.TimerEnabled [2])
   {
      APU.Timer [2] = 0;
      IAPU.RAM [0xff] = 0;
      if ((APU.TimerTarget [2] = IAPU.RAM [0xfc]) == 0)
         APU.TimerTarget [2] = 0x100;
   }
   APU.TimerEnabled [0] = byte & 1;
   APU.TimerEnabled [1] = (byte & 2) >> 1;
   APU.TimerEnabled [2] = (byte & 4) >> 2;

   if (byte & 0x10)
      IAPU.RAM [0xF4] = IAPU.RAM [0xF5] = 0;

   if (byte & 0x20)
      IAPU.RAM [0xF6] = IAPU.RAM [0xF7] = 0;

   if (byte & 0x80)
   {
      if (!APU.ShowROM)
      {
         // memmove converted: Different mallocs [Neb]
         // DS2 DMA notes: The APU ROM is not 32-byte aligned [Neb]
         memcpy(&IAPU.RAM [0xffc0], APUROM, sizeof(APUROM));
         APU.ShowROM = true;
      }
   }
   else
   {
      if (APU.ShowROM)
      {
         APU.ShowROM = false;
         // memmove converted: Different mallocs [Neb]
         // DS2 DMA notes: The APU ROM is not 32-byte aligned [Neb]
         memcpy(&IAPU.RAM [0xffc0], APU.ExtraRAM, sizeof(APUROM));
      }
   }
   IAPU.RAM [0xf1] = byte;
}

void S9xSetAPUTimer(uint16_t Address, uint8_t byte)
{
   IAPU.RAM [Address] = byte;

   switch (Address)
   {
   case 0xfa:
      if ((APU.TimerTarget [0] = IAPU.RAM [0xfa]) == 0)
         APU.TimerTarget [0] = 0x100;
      APU.TimerValueWritten [0] = true;
      break;
   case 0xfb:
      if ((APU.TimerTarget [1] = IAPU.RAM [0xfb]) == 0)
         APU.TimerTarget [1] = 0x100;
      APU.TimerValueWritten [1] = true;
      break;
   case 0xfc:
      if ((APU.TimerTarget [2] = IAPU.RAM [0xfc]) == 0)
         APU.TimerTarget [2] = 0x100;
      APU.TimerValueWritten [2] = true;
      break;
   }
}

uint8_t S9xGetAPUDSP()
{
   uint8_t reg = IAPU.RAM [0xf2] & 0x7f;
   uint8_t byte = APU.DSP [reg];

   switch (reg)
   {
   case APU_KON:
      break;
   case APU_KOFF:
      break;
   case APU_OUTX + 0x00:
   case APU_OUTX + 0x10:
   case APU_OUTX + 0x20:
   case APU_OUTX + 0x30:
   case APU_OUTX + 0x40:
   case APU_OUTX + 0x50:
   case APU_OUTX + 0x60:
   case APU_OUTX + 0x70:
      if (SoundData.channels [reg >> 4].state == SOUND_SILENT)
         return (0);
      return ((SoundData.channels [reg >> 4].sample >> 8) |
              (SoundData.channels [reg >> 4].sample & 0xff));

   case APU_ENVX + 0x00:
   case APU_ENVX + 0x10:
   case APU_ENVX + 0x20:
   case APU_ENVX + 0x30:
   case APU_ENVX + 0x40:
   case APU_ENVX + 0x50:
   case APU_ENVX + 0x60:
   case APU_ENVX + 0x70:
      return ((uint8_t) S9xGetEnvelopeHeight(reg >> 4));

   case APU_ENDX:
      // To fix speech in Magical Drop 2 6/11/00
      // APU.DSP [APU_ENDX] = 0;
      break;
   default:
      break;
   }
   return (byte);
}

#endif
