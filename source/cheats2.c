#include "../copyright"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "snes9x.h"
#include "cheats.h"
#include "memmap.h"

extern SCheatData Cheat;

void S9xInitCheatData()
{
   Cheat.RAM = Memory.RAM;
   Cheat.SRAM = Memory.SRAM;
   Cheat.FillRAM = Memory.FillRAM;
}

void S9xAddCheat(bool enable, bool save_current_value, uint32_t address, uint8_t byte)
{
   if (Cheat.num_cheats < sizeof(Cheat.c) / sizeof(Cheat.c [0]))
   {
      Cheat.c [Cheat.num_cheats].address = address;
      Cheat.c [Cheat.num_cheats].byte = byte;
      Cheat.c [Cheat.num_cheats].enabled = enable;
      if (save_current_value)
      {
         Cheat.c [Cheat.num_cheats].saved_byte = S9xGetByte(address);
         Cheat.c [Cheat.num_cheats].saved = true;
      }
      Cheat.num_cheats++;
      if (enable)
         S9xApplyCheat(Cheat.num_cheats - 1);
   }
}

void S9xDeleteCheat(uint32_t which1)
{
   if (which1 < Cheat.num_cheats)
   {
      if (Cheat.c [which1].enabled)
         S9xRemoveCheat(which1);

      // memmove required: Overlapping addresses [Neb]
      memmove(&Cheat.c [which1], &Cheat.c [which1 + 1], sizeof(Cheat.c [0]) * (Cheat.num_cheats - which1 - 1));
      Cheat.num_cheats--;
   }
}

void S9xDeleteCheats()
{
   S9xRemoveCheats();
   Cheat.num_cheats = 0;
}

void S9xEnableCheat(uint32_t which1)
{
   if (which1 < Cheat.num_cheats && !Cheat.c [which1].enabled)
   {
      Cheat.c [which1].enabled = true;
      S9xApplyCheat(which1);
   }
}

void S9xDisableCheat(uint32_t which1)
{
   if (which1 < Cheat.num_cheats && Cheat.c [which1].enabled)
   {
      S9xRemoveCheat(which1);
      Cheat.c [which1].enabled = false;
   }
}

void S9xRemoveCheat(uint32_t which1)
{
   if (Cheat.c [which1].saved)
   {
      uint32_t address = Cheat.c [which1].address;

      int32_t block = (address >> MEMMAP_SHIFT) & MEMMAP_MASK;
      uint8_t* ptr = Memory.Map [block];

      if (ptr >= (uint8_t*) MAP_LAST)
         *(ptr + (address & 0xffff)) = Cheat.c [which1].saved_byte;
      else
         S9xSetByte(Cheat.c [which1].saved_byte, address);
      // Unsave the address for the next call to S9xRemoveCheat.
      Cheat.c [which1].saved = false;
   }
}

void S9xApplyCheat(uint32_t which1)
{
   uint32_t address = Cheat.c [which1].address;

   if (!Cheat.c [which1].saved)
      Cheat.c [which1].saved_byte = S9xGetByte(address);

   int32_t block = (address >> MEMMAP_SHIFT) & MEMMAP_MASK;
   uint8_t* ptr = Memory.Map [block];

   if (ptr >= (uint8_t*) MAP_LAST)
      *(ptr + (address & 0xffff)) = Cheat.c [which1].byte;
   else
      S9xSetByte(Cheat.c [which1].byte, address);
   Cheat.c [which1].saved = true;
}

void S9xApplyCheats()
{
   uint32_t i;
   if (Settings.ApplyCheats)
   {
      for (i = 0; i < Cheat.num_cheats; i++)
         if (Cheat.c [i].enabled)
            S9xApplyCheat(i);
   }
}

void S9xRemoveCheats()
{
   uint32_t i;
   for (i = 0; i < Cheat.num_cheats; i++)
      if (Cheat.c [i].enabled)
         S9xRemoveCheat(i);
}

bool S9xLoadCheatFile(const char* filename)
{
   Cheat.num_cheats = 0;

   FILE* fs = fopen(filename, "rb");
   uint8_t data [8 + MAX_SFCCHEAT_NAME];

   if (!fs)
      return (false);

   while (fread((void*) data, 1, 8 + MAX_SFCCHEAT_NAME,
                fs) == 8 + MAX_SFCCHEAT_NAME)
   {
      if (data[6] != 254 || data[7] != 252)
      {
         fclose(fs);
         return (false);
      }
      Cheat.c [Cheat.num_cheats].enabled = (data [0] & 4) == 0;
      Cheat.c [Cheat.num_cheats].byte = data [1];
      Cheat.c [Cheat.num_cheats].address = data [2] | (data [3] << 8) |
                                           (data [4] << 16);
      Cheat.c [Cheat.num_cheats].saved_byte = data [5];
      Cheat.c [Cheat.num_cheats].saved = (data [0] & 8) != 0;
      memcpy(Cheat.c [Cheat.num_cheats].name, &data [8], MAX_SFCCHEAT_NAME - 1);
      Cheat.c [Cheat.num_cheats++].name [MAX_SFCCHEAT_NAME - 1] = 0;
   }
   fclose(fs);

   return (true);
}

bool S9xSaveCheatFile(const char* filename)
{
   if (Cheat.num_cheats == 0)
   {
      (void) remove(filename);
      return (true);
   }

   FILE* fs = fopen(filename, "wb");
   uint8_t data [8 + MAX_SFCCHEAT_NAME];

   if (!fs)
      return (false);

   uint32_t i;
   for (i = 0; i < Cheat.num_cheats; i++)
   {
      memset(data, 0, 8 + MAX_SFCCHEAT_NAME);
      data [6] = 254;
      data [7] = 252;
      if (!Cheat.c [i].enabled)
         data [0] |= 4;

      if (Cheat.c [i].saved)
         data [0] |= 8;

      data [1] = Cheat.c [i].byte;
      data [2] = (uint8_t) Cheat.c [i].address;
      data [3] = (uint8_t)(Cheat.c [i].address >> 8);
      data [4] = (uint8_t)(Cheat.c [i].address >> 16);
      data [5] = Cheat.c [i].saved_byte;

      memcpy(&data [8], Cheat.c [i].name, MAX_SFCCHEAT_NAME - 1);
      if (fwrite(data, 8 + MAX_SFCCHEAT_NAME, 1, fs) != 1)
      {
         fclose(fs);
         return (false);
      }
   }

   fclose(fs);
   return (true);
}
