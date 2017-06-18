#include "../copyright"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "snes9x.h"
#include "cheats.h"
#include "memmap.h"

static bool S9xAllHex(const char* code, int32_t len)
{
   int32_t i;
   for (i = 0; i < len; i++)
      if ((code [i] < '0' || code [i] > '9') &&
          (code [i] < 'a' || code [i] > 'f') &&
          (code [i] < 'A' || code [i] > 'F'))
         return false;

   return true;
}

const char* S9xProActionReplayToRaw(const char* code, uint32_t* address, uint8_t* byte)
{
   uint32_t data = 0;
   if (strlen(code) != 8 || !S9xAllHex(code, 8) ||
         sscanf(code, "%x", &data) != 1)
      return "Invalid Pro Action Replay code - should be 8 hex digits in length.";

   *address = data >> 8;
   *byte = (uint8_t) data;
   return NULL;
}

const char* S9xGoldFingerToRaw(const char* code, uint32_t* address, bool* sram, uint8_t* num_bytes, uint8_t bytes[3])
{
   char tmp [15];
   if (strlen(code) != 14)
      return "Invalid Gold Finger code should be 14 hex digits in length.";

   strncpy(tmp, code, 5);
   tmp [5] = 0;
   if (sscanf(tmp, "%x", address) != 1)
      return "Invalid Gold Finger code.";

   int32_t i;
   for (i = 0; i < 3; i++)
   {
      strncpy(tmp, code + 5 + i * 2, 2);
      tmp [2] = 0;
      int32_t byte;
      if (sscanf(tmp, "%x", &byte) != 1)
         break;
      bytes [i] = (uint8_t) byte;
   }
   *num_bytes = i;
   *sram = code [13] == '1';
   return NULL;
}

const char* S9xGameGenieToRaw(const char* code, uint32_t* address, uint8_t* byte)
{
   char new_code [12];

   if (strlen(code) != 9 || *(code + 4) != '-' || !S9xAllHex(code, 4) || !S9xAllHex(code + 5, 4))
      return "Invalid Game Genie(tm) code - should be 'xxxx-xxxx'.";

   strcpy(new_code, "0x");
   strncpy(new_code + 2, code, 4);
   strcpy(new_code + 6, code + 5);

   static char* real_hex  = "0123456789ABCDEF";
   static char* genie_hex = "DF4709156BC8A23E";

   int32_t i;
   for (i = 2; i < 10; i++)
   {
      if (islower(new_code [i]))
         new_code [i] = toupper(new_code [i]);
      int32_t j;
      for (j = 0; j < 16; j++)
      {
         if (new_code [i] == genie_hex [j])
         {
            new_code [i] = real_hex [j];
            break;
         }
      }
      if (j == 16)
         return "Invalid hex-character in Game Genie(tm) code";
   }
   uint32_t data = 0;
   sscanf(new_code, "%x", &data);
   *byte = (uint8_t)(data >> 24);
   *address = ((data & 0x003c00) << 10) +
              ((data & 0x00003c) << 14) +
              ((data & 0xf00000) >>  8) +
              ((data & 0x000003) << 10) +
              ((data & 0x00c000) >>  6) +
              ((data & 0x0f0000) >> 12) +
              ((data & 0x0003c0) >>  6);

   return NULL;
}

void S9xStartCheatSearch(SCheatData* d)
{
   // memmove may be required: Source is usually a different malloc, but could be pointed to d->CWRAM [Neb]
   memmove(d->CWRAM, d->RAM, 0x20000);
   // memmove may be required: Source is usually a different malloc, but could be pointed to d->CSRAM [Neb]
   memmove(d->CSRAM, d->SRAM, 0x10000);
   // memmove may be required: Source is usually a different malloc, but could be pointed to d->CIRAM [Neb]
   memmove(d->CIRAM, &d->FillRAM [0x3000], 0x2000);
   memset(d->WRAM_BITS, 0xff, 0x20000 >> 3);
   memset(d->SRAM_BITS, 0xff, 0x10000 >> 3);
   memset(d->IRAM_BITS, 0xff, 0x2000 >> 3);
}

#define BIT_CLEAR(a,v) \
(a)[(v) >> 5] &= ~(1 << ((v) & 31))

#define BIT_SET(a,v) \
(a)[(v) >> 5] |= 1 << ((v) & 31)

#define TEST_BIT(a,v) \
((a)[(v) >> 5] & (1 << ((v) & 31)))

#define CHEATS_C(c,a,b) \
((c) == S9X_LESS_THAN ? (a) < (b) : \
 (c) == S9X_GREATER_THAN ? (a) > (b) : \
 (c) == S9X_LESS_THAN_OR_EQUAL ? (a) <= (b) : \
 (c) == S9X_GREATER_THAN_OR_EQUAL ? (a) >= (b) : \
 (c) == S9X_EQUAL ? (a) == (b) : \
 (a) != (b))

#define _D(s,m,o) \
((s) == S9X_8_BITS ? (uint8_t) (*((m) + (o))) : \
 (s) == S9X_16_BITS ? ((uint16_t) (*((m) + (o)) + (*((m) + (o) + 1) << 8))) : \
 (s) == S9X_24_BITS ? ((uint32_t) (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16))) : \
((uint32_t)  (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16) + (*((m) + (o) + 3) << 24))))

#define _DS(s,m,o) \
((s) == S9X_8_BITS ? ((int8_t) *((m) + (o))) : \
 (s) == S9X_16_BITS ? ((int16_t) (*((m) + (o)) + (*((m) + (o) + 1) << 8))) : \
 (s) == S9X_24_BITS ? (((int32_t) ((*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16)) << 8)) >> 8): \
 ((int32_t) (*((m) + (o)) + (*((m) + (o) + 1) << 8) + (*((m) + (o) + 2) << 16) + (*((m) + (o) + 3) << 24))))

void S9xSearchForChange(SCheatData* d, S9xCheatComparisonType cmp,
                        S9xCheatDataSize size, bool is_signed, bool update)
{
   int32_t l;

   switch (size)
   {
   case S9X_8_BITS:
      l = 0;
      break;
   case S9X_16_BITS:
      l = 1;
      break;
   case S9X_24_BITS:
      l = 2;
      break;
   default:
   case S9X_32_BITS:
      l = 3;
      break;
   }

   int32_t i;
   if (is_signed)
   {
      for (i = 0; i < 0x20000 - l; i++)
      {
         if (TEST_BIT(d->WRAM_BITS, i) &&
               CHEATS_C(cmp, _DS(size, d->RAM, i), _DS(size, d->CWRAM, i)))
         {
            if (update)
               d->CWRAM [i] = d->RAM [i];
         }
         else
            BIT_CLEAR(d->WRAM_BITS, i);
      }

      for (i = 0; i < 0x10000 - l; i++)
      {
         if (TEST_BIT(d->SRAM_BITS, i) &&
               CHEATS_C(cmp, _DS(size, d->SRAM, i), _DS(size, d->CSRAM, i)))
         {
            if (update)
               d->CSRAM [i] = d->SRAM [i];
         }
         else
            BIT_CLEAR(d->SRAM_BITS, i);
      }

      for (i = 0; i < 0x2000 - l; i++)
      {
         if (TEST_BIT(d->IRAM_BITS, i) &&
               CHEATS_C(cmp, _DS(size, d->FillRAM + 0x3000, i), _DS(size, d->CIRAM, i)))
         {
            if (update)
               d->CIRAM [i] = d->FillRAM [i + 0x3000];
         }
         else
            BIT_CLEAR(d->IRAM_BITS, i);
      }
   }
   else
   {
      for (i = 0; i < 0x20000 - l; i++)
      {
         if (TEST_BIT(d->WRAM_BITS, i) &&
               CHEATS_C(cmp, _D(size, d->RAM, i), _D(size, d->CWRAM, i)))
         {
            if (update)
               d->CWRAM [i] = d->RAM [i];
         }
         else
            BIT_CLEAR(d->WRAM_BITS, i);
      }

      for (i = 0; i < 0x10000 - l; i++)
      {
         if (TEST_BIT(d->SRAM_BITS, i) &&
               CHEATS_C(cmp, _D(size, d->SRAM, i), _D(size, d->CSRAM, i)))
         {
            if (update)
               d->CSRAM [i] = d->SRAM [i];
         }
         else
            BIT_CLEAR(d->SRAM_BITS, i);
      }

      for (i = 0; i < 0x2000 - l; i++)
      {
         if (TEST_BIT(d->IRAM_BITS, i) &&
               CHEATS_C(cmp, _D(size, d->FillRAM + 0x3000, i), _D(size, d->CIRAM, i)))
         {
            if (update)
               d->CIRAM [i] = d->FillRAM [i + 0x3000];
         }
         else
            BIT_CLEAR(d->IRAM_BITS, i);
      }
   }
}

void S9xSearchForValue(SCheatData* d, S9xCheatComparisonType cmp,
                       S9xCheatDataSize size, uint32_t value,
                       bool is_signed, bool update)
{
   int32_t l;

   switch (size)
   {
   case S9X_8_BITS:
      l = 0;
      break;
   case S9X_16_BITS:
      l = 1;
      break;
   case S9X_24_BITS:
      l = 2;
      break;
   default:
   case S9X_32_BITS:
      l = 3;
      break;
   }

   int32_t i;

   if (is_signed)
   {
      for (i = 0; i < 0x20000 - l; i++)
      {
         if (TEST_BIT(d->WRAM_BITS, i) &&
               CHEATS_C(cmp, _DS(size, d->RAM, i), (int32_t) value))
         {
            if (update)
               d->CWRAM [i] = d->RAM [i];
         }
         else
            BIT_CLEAR(d->WRAM_BITS, i);
      }

      for (i = 0; i < 0x10000 - l; i++)
      {
         if (TEST_BIT(d->SRAM_BITS, i) &&
               CHEATS_C(cmp, _DS(size, d->SRAM, i), (int32_t) value))
         {
            if (update)
               d->CSRAM [i] = d->SRAM [i];
         }
         else
            BIT_CLEAR(d->SRAM_BITS, i);
      }

      for (i = 0; i < 0x2000 - l; i++)
      {
         if (TEST_BIT(d->IRAM_BITS, i) &&
               CHEATS_C(cmp, _DS(size, d->FillRAM + 0x3000, i), (int32_t) value))
         {
            if (update)
               d->CIRAM [i] = d->FillRAM [i + 0x3000];
         }
         else
            BIT_CLEAR(d->IRAM_BITS, i);
      }
   }
   else
   {
      for (i = 0; i < 0x20000 - l; i++)
      {
         if (TEST_BIT(d->WRAM_BITS, i) &&
               CHEATS_C(cmp, _D(size, d->RAM, i), value))
         {
            if (update)
               d->CWRAM [i] = d->RAM [i];
         }
         else
            BIT_CLEAR(d->WRAM_BITS, i);
      }

      for (i = 0; i < 0x10000 - l; i++)
      {
         if (TEST_BIT(d->SRAM_BITS, i) &&
               CHEATS_C(cmp, _D(size, d->SRAM, i), value))
         {
            if (update)
               d->CSRAM [i] = d->SRAM [i];
         }
         else
            BIT_CLEAR(d->SRAM_BITS, i);
      }

      for (i = 0; i < 0x2000 - l; i++)
      {
         if (TEST_BIT(d->IRAM_BITS, i) &&
               CHEATS_C(cmp, _D(size, d->FillRAM + 0x3000, i), value))
         {
            if (update)
               d->CIRAM [i] = d->FillRAM [i + 0x3000];
         }
         else
            BIT_CLEAR(d->IRAM_BITS, i);
      }
   }
}

void S9xOutputCheatSearchResults(SCheatData* d)
{
   int32_t i;
   for (i = 0; i < 0x20000; i++)
   {
      if (TEST_BIT(d->WRAM_BITS, i))
         printf("WRAM: %05x: %02x\n", i, d->RAM [i]);
   }

   for (i = 0; i < 0x10000; i++)
   {
      if (TEST_BIT(d->SRAM_BITS, i))
         printf("SRAM: %04x: %02x\n", i, d->SRAM [i]);
   }

   for (i = 0; i < 0x2000; i++)
   {
      if (TEST_BIT(d->IRAM_BITS, i))
         printf("IRAM: %05x: %02x\n", i, d->FillRAM [i + 0x3000]);
   }
}
