#include "../copyright"

#ifndef __LIBRETRO_SDK_BOOLEAN_H
#define __LIBRETRO_SDK_BOOLEAN_H

#ifndef __cplusplus

#if defined(_MSC_VER) && !defined(SN_TARGET_PS3)
/* Hack applied for MSVC when compiling in C89 mode as it isn't C99 compliant. */
#define bool unsigned char
#define true 1
#define false 0
#else
#include <stdbool.h>
#endif

#endif

#endif
