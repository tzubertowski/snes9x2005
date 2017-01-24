#include "../copyright"

#ifndef _apumemory_h_
#define _apumemory_h_

extern uint8_t W4;
extern uint8_t APUROM[64];

INLINE uint8_t S9xAPUGetByteZ(uint8_t Address)
{
   if (Address >= 0xf0 && IAPU.DirectPage == IAPU.RAM)
   {
      if (Address >= 0xf4 && Address <= 0xf7)
      {
#ifdef SPC700_SHUTDOWN
         IAPU.WaitAddress2 = IAPU.WaitAddress1;
         IAPU.WaitAddress1 = IAPU.PC;
#endif
         return (IAPU.RAM [Address]);
      }
      if (Address >= 0xfd)
      {
#ifdef SPC700_SHUTDOWN
         IAPU.WaitAddress2 = IAPU.WaitAddress1;
         IAPU.WaitAddress1 = IAPU.PC;
#endif
         uint8_t t = IAPU.RAM [Address];
         IAPU.RAM [Address] = 0;
         return (t);
      }
      else if (Address == 0xf3)
         return (S9xGetAPUDSP());

      return (IAPU.RAM [Address]);
   }
   else
      return (IAPU.DirectPage [Address]);
}

INLINE void S9xAPUSetByteZ(uint8_t byte, uint8_t Address)
{
   if (Address >= 0xf0 && IAPU.DirectPage == IAPU.RAM)
   {
      if (Address == 0xf3)
         S9xSetAPUDSP(byte);
      else if (Address >= 0xf4 && Address <= 0xf7)
         APU.OutPorts [Address - 0xf4] = byte;
      else if (Address == 0xf1)
         S9xSetAPUControl(byte);
      else if (Address < 0xfd)
      {
         IAPU.RAM [Address] = byte;
         if (Address >= 0xfa)
         {
            if (byte == 0)
               APU.TimerTarget [Address - 0xfa] = 0x100;
            else
               APU.TimerTarget [Address - 0xfa] = byte;
         }
      }
   }
   else
      IAPU.DirectPage [Address] = byte;
}

INLINE uint8_t S9xAPUGetByte(uint32_t Address)
{
   Address &= 0xffff;

   if (Address == 0xf3)
      return S9xGetAPUDSP();

   bool zero = (Address >= 0xfd && Address <= 0xff);
   uint8_t t = IAPU.RAM [Address];

#ifdef SPC700_SHUTDOWN
   if (zero || (Address >= 0xf4 && Address <= 0xf7))
   {
      IAPU.WaitAddress2 = IAPU.WaitAddress1;
      IAPU.WaitAddress1 = IAPU.PC;
   }
#endif

   if(zero)
      IAPU.RAM [Address] = 0;

   return t;
}

INLINE void S9xAPUSetByte(uint8_t byte, uint32_t Address)
{
   Address &= 0xffff;

   if (Address <= 0xff && Address >= 0xf0)
   {
      if (Address == 0xf3)
         S9xSetAPUDSP(byte);
      else if (Address >= 0xf4 && Address <= 0xf7)
         APU.OutPorts [Address - 0xf4] = byte;
      else if (Address == 0xf1)
         S9xSetAPUControl(byte);
      else if (Address < 0xfd)
      {
         IAPU.RAM [Address] = byte;
         if (Address >= 0xfa)
         {
            if (byte == 0)
               APU.TimerTarget [Address - 0xfa] = 0x100;
            else
               APU.TimerTarget [Address - 0xfa] = byte;
         }
      }
   }
   else
   {
      if (Address < 0xffc0)
         IAPU.RAM [Address] = byte;
      else
      {
         APU.ExtraRAM [Address - 0xffc0] = byte;
         if (!APU.ShowROM)
            IAPU.RAM [Address] = byte;
      }
   }
}
#endif
