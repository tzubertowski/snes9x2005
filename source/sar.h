#include "../copyright"

#ifndef _SAR_H_
#define _SAR_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "port.h"

#ifdef RIGHTSHIFT_IS_SAR
#define SAR8(b, n) ((b)>>(n))
#define SAR16(b, n) ((b)>>(n))
#define SAR32(b, n) ((b)>>(n))
#define SAR64(b, n) ((b)>>(n))
#else

static inline int8_t SAR8(const int8_t b, const int n)
{
#ifndef RIGHTSHIFT_INT8_IS_SAR
   if (b < 0) return (b >> n) | (-1 << (8 - n));
#endif
   return b >> n;
}

static inline int16_t SAR16(const int16_t b, const int n)
{
#ifndef RIGHTSHIFT_INT16_IS_SAR
   if (b < 0) return (b >> n) | (-1 << (16 - n));
#endif
   return b >> n;
}

static inline int32_t SAR32(const int32_t b, const int n)
{
#ifndef RIGHTSHIFT_INT32_IS_SAR
   if (b < 0) return (b >> n) | (-1 << (32 - n));
#endif
   return b >> n;
}

static inline int64_t SAR64(const int64_t b, const int n)
{
#ifndef RIGHTSHIFT_INT64_IS_SAR
   if (b < 0) return (b >> n) | (-1 << (64 - n));
#endif
   return b >> n;
}

#endif

#endif
