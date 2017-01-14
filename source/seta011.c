#include "../copyright"

#include <stdio.h>
#include "seta.h"
#include "memmap.h"

ST011_Regs ST011;

// shougi playboard
uint8_t board[9][9];

// debug
static int line = 0;

uint8_t S9xGetST011(uint32_t Address)
{
   uint8_t t;
   uint16_t address = (uint16_t) Address & 0xFFFF;

   // line counter
   line++;

   // status check
   if (address == 0x01)
      t = 0xFF;
   // read directly from s-ram
   else
      t = Memory.SRAM[address];

   return t;
}

void S9xSetST011(uint32_t Address, uint8_t Byte)
{
   uint16_t address = (uint16_t) Address & 0xFFFF;
   static bool reset = false;

   // debug
   line++;

   if (!reset)
   {
      // bootup values
      ST011.waiting4command = true;
      reset = true;
   }

   Memory.SRAM[address] = Byte;

   // op commands/data goes through this address
   if (address == 0x00)
   {
      // check for new commands
      if (ST011.waiting4command)
      {
         ST011.waiting4command = false;
         ST011.command = Byte;
         ST011.in_index = 0;
         ST011.out_index = 0;
         switch (ST011.command)
         {
         case 0x01:
            ST011.in_count = 12 * 10 + 8;
            break;
         case 0x02:
            ST011.in_count = 4;
            break;
         case 0x04:
            ST011.in_count = 0;
            break;
         case 0x05:
            ST011.in_count = 0;
            break;
         case 0x06:
            ST011.in_count = 0;
            break;
         case 0x07:
            ST011.in_count = 0;
            break;
         case 0x0E:
            ST011.in_count = 0;
            break;
         default:
            ST011.waiting4command = true;
            break;
         }
      }
      else
      {
         ST011.parameters [ST011.in_index] = Byte;
         ST011.in_index++;
      }
   }

   if (ST011.in_count == ST011.in_index)
   {
      // Actually execute the command
      ST011.waiting4command = true;
      ST011.out_index = 0;
      switch (ST011.command)
      {
      // unknown: download playboard
      case 0x01:
      {
         // 9x9 board data: top to bottom, left to right
         // Values represent piece types and ownership
         int lcv;
         for (lcv = 0; lcv < 9; lcv++)
            memcpy(board[lcv], ST011.parameters + lcv * 10, 9 * 1);
      }
      break;

      // unknown
      case 0x02:
         break;

      // unknown
      case 0x04:
      case 0x05:
      {
         // outputs
         Memory.SRAM[0x12C] = 0x00;
         Memory.SRAM[0x12E] = 0x00;
      }
      break;

      // unknown
      case 0x06:
      case 0x07:
         break;

      // unknown
      case 0x0E:
      {
         // outputs
         Memory.SRAM[0x12C] = 0x00;
         Memory.SRAM[0x12D] = 0x00;
      }
      break;
      }
   }
}

