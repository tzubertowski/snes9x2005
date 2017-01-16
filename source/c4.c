#include "../copyright"

#include <math.h>
#include <stdlib.h>
#include "c4.h"
#include "memmap.h"

int16_t C4WFXVal;
int16_t C4WFYVal;
int16_t C4WFZVal;
int16_t C4WFX2Val;
int16_t C4WFY2Val;
int16_t C4WFDist;
int16_t C4WFScale;

static double tanval;
static double c4x, c4y, c4z;
static double c4x2, c4y2, c4z2;

void C4TransfWireFrame()
{
   c4x = (double) C4WFXVal;
   c4y = (double) C4WFYVal;
   c4z = (double) C4WFZVal - 0x95;

   // Rotate X
   tanval = -(double) C4WFX2Val * 3.14159265 * 2 / 128;
   c4y2 = c4y * cos(tanval) - c4z * sin(tanval);
   c4z2 = c4y * sin(tanval) + c4z * cos(tanval);

   // Rotate Y
   tanval = -(double)C4WFY2Val * 3.14159265 * 2 / 128;
   c4x2 = c4x * cos(tanval) + c4z2 * sin(tanval);
   c4z = c4x * - sin(tanval) + c4z2 * cos(tanval);

   // Rotate Z
   tanval = -(double) C4WFDist * 3.14159265 * 2 / 128;
   c4x = c4x2 * cos(tanval) - c4y2 * sin(tanval);
   c4y = c4x2 * sin(tanval) + c4y2 * cos(tanval);

   // Scale
   C4WFXVal = (int16_t)(c4x * (double)C4WFScale / (0x90 * (c4z + 0x95)) * 0x95);
   C4WFYVal = (int16_t)(c4y * (double)C4WFScale / (0x90 * (c4z + 0x95)) * 0x95);
}

void C4TransfWireFrame2()
{
   c4x = (double)C4WFXVal;
   c4y = (double)C4WFYVal;
   c4z = (double)C4WFZVal;

   // Rotate X
   tanval = -(double) C4WFX2Val * 3.14159265 * 2 / 128;
   c4y2 = c4y * cos(tanval) - c4z * sin(tanval);
   c4z2 = c4y * sin(tanval) + c4z * cos(tanval);

   // Rotate Y
   tanval = -(double) C4WFY2Val * 3.14159265 * 2 / 128;
   c4x2 = c4x * cos(tanval) + c4z2 * sin(tanval);
   c4z = c4x * -sin(tanval) + c4z2 * cos(tanval);

   // Rotate Z
   tanval = -(double)C4WFDist * 3.14159265 * 2 / 128;
   c4x = c4x2 * cos(tanval) - c4y2 * sin(tanval);
   c4y = c4x2 * sin(tanval) + c4y2 * cos(tanval);

   // Scale
   C4WFXVal = (int16_t)(c4x * (double)C4WFScale / 0x100);
   C4WFYVal = (int16_t)(c4y * (double)C4WFScale / 0x100);
}

void C4CalcWireFrame()
{
   C4WFXVal = C4WFX2Val - C4WFXVal;
   C4WFYVal = C4WFY2Val - C4WFYVal;
   if (abs(C4WFXVal) > abs(C4WFYVal))
   {
      C4WFDist = abs(C4WFXVal) + 1;
      C4WFYVal = (int16_t)(256 * (double) C4WFYVal / abs(C4WFXVal));
      if (C4WFXVal < 0)
         C4WFXVal = -256;
      else
         C4WFXVal = 256;
   }
   else
   {
      if (C4WFYVal != 0)
      {
         C4WFDist = abs(C4WFYVal) + 1;
         C4WFXVal = (int16_t)(256 * (double)C4WFXVal / abs(C4WFYVal));
         if (C4WFYVal < 0)
            C4WFYVal = -256;
         else
            C4WFYVal = 256;
      }
      else
         C4WFDist = 0;
   }
}

int16_t C41FXVal;
int16_t C41FYVal;
int16_t C41FAngleRes;
int16_t C41FDist;
int16_t C41FDistVal;

void C4Op1F()
{
   if (C41FXVal == 0)
   {
      if (C41FYVal > 0)
         C41FAngleRes = 0x80;
      else
         C41FAngleRes = 0x180;
   }
   else
   {
      tanval = (double) C41FYVal / C41FXVal;
      C41FAngleRes = (int16_t)(atan(tanval) / (3.141592675 * 2) * 512);
      if (C41FXVal < 0)
         C41FAngleRes += 0x100;
      C41FAngleRes &= 0x1FF;
   }
}

void C4Op15()
{
   tanval = sqrt((double) C41FYVal * C41FYVal + (double) C41FXVal * C41FXVal);
   C41FDist = (int16_t) tanval;
}

void C4Op0D()
{
   tanval = sqrt((double) C41FYVal * C41FYVal + (double) C41FXVal * C41FXVal);
   tanval = C41FDistVal / tanval;
   C41FYVal = (int16_t)(C41FYVal * tanval * 0.99);
   C41FXVal = (int16_t)(C41FXVal * tanval * 0.98);
}
