#include "../copyright"

#ifndef _PORT_H_
#define _PORT_H_

#include <limits.h>

#include <string.h>
#include <sys/types.h>

#ifdef PSP
#define PIXEL_FORMAT BGR555
#else
#define PIXEL_FORMAT RGB565
#endif
// The above is used to disable the 16-bit graphics mode checks sprinkled
// throughout the code, if the pixel format is always 16-bit.

#include "pixform.h"

#ifndef __WIN32__

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#define _MAX_DIR PATH_MAX
#define _MAX_DRIVE 1
#define _MAX_FNAME PATH_MAX
#define _MAX_EXT PATH_MAX
#define _MAX_PATH PATH_MAX

void _makepath(char* path, const char* drive, const char* dir,
               const char* fname, const char* ext);
void _splitpath(const char* path, char* drive, char* dir, char* fname,
                char* ext);
#else /* __WIN32__ */
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

#define SLASH_STR "/"
#define SLASH_CHAR '/'

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || \
    defined(__WIN32__) || defined(__alpha__)
#define FAST_LSB_WORD_ACCESS
#elif defined(__MIPSEL__)
// On little-endian MIPS, a 16-bit word can be read directly from an address
// only if it's aligned.
#define FAST_ALIGNED_LSB_WORD_ACCESS
#endif

#include <libretro.h>

#define ABS(X)   ((X) <  0  ? -(X) : (X))
#define MIN(A,B) ((A) < (B) ?  (A) : (B))
#define MAX(A,B) ((A) > (B) ?  (A) : (B))

#endif
