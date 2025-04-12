/*
 * Copyright (C) 2018, 2019 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

/*****************************************************************************/
/* CC-RL library                                                             */
/*****************************************************************************/

#ifndef _INTTYPES_H
#define _INTTYPES_H

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    intmax_t quot;
    intmax_t rem;
} imaxdiv_t;

intmax_t __far imaxabs(intmax_t j);

imaxdiv_t __far imaxdiv(intmax_t numer, intmax_t denom);

intmax_t __far strtoimax(const char __near * restrict nptr, char __near * __near * restrict endptr, int base);
intmax_t __far _COM_strtoimax_ff(const char __far * restrict nptr, char __far * __far * restrict endptr, int base);

uintmax_t __far strtoumax(const char __near * restrict nptr, char __near * __near * restrict endptr, int base);
uintmax_t __far _COM_strtoumax_ff(const char __far * restrict nptr, char __far * __far * restrict endptr, int base);

#define imaxabs(_j)                      llabs((_j))
#define strtoimax(_nptr, _endptr, _base)   strtoll((_nptr), (_endptr), (_base))
#define _COM_strtoimax_ff(_nptr, _endptr, _base) _COM_strtoll_ff((_nptr), (_endptr), (_base))
#define strtoumax(_nptr, _endptr, _base)   strtoull((_nptr), (_endptr), (_base))
#define _COM_strtoumax_ff(_nptr, _endptr, _base) _COM_strtoull_ff((_nptr), (_endptr), (_base))

#define _PFX_8    "hh"
#define _PFX_16   "h"
#define _PFX_32   "l"
#define _PFX_64   "ll"
#define _PFX_PTR  "l"  /* #define INTPTR_MAX INT32_MAX */

#define PRId8       _PFX_8 "d"
#define PRId16      _PFX_16 "d"
#define PRId32      _PFX_32 "d"
#define PRId64      _PFX_64 "d"
#define PRIdLEAST8  _PFX_8 "d"
#define PRIdLEAST16 _PFX_16 "d"
#define PRIdLEAST32 _PFX_32 "d"
#define PRIdLEAST64 _PFX_64 "d"
#define PRIdFAST8   _PFX_8 "d"
#define PRIdFAST16  _PFX_16 "d"
#define PRIdFAST32  _PFX_32 "d"
#define PRIdFAST64  _PFX_64 "d"
#define PRIdMAX     _PFX_64 "d"
#define PRIdPTR     _PFX_PTR "d"

#define PRIi8       _PFX_8 "i"
#define PRIi16      _PFX_16 "i"
#define PRIi32      _PFX_32 "i"
#define PRIi64      _PFX_64 "i"
#define PRIiLEAST8  _PFX_8 "i"
#define PRIiLEAST16 _PFX_16 "i"
#define PRIiLEAST32 _PFX_32 "i"
#define PRIiLEAST64 _PFX_64 "i"
#define PRIiFAST8   _PFX_8 "i"
#define PRIiFAST16  _PFX_16 "i"
#define PRIiFAST32  _PFX_32 "i"
#define PRIiFAST64  _PFX_64 "i"
#define PRIiMAX     _PFX_64 "i"
#define PRIiPTR     _PFX_PTR "i"

#define PRIo8       _PFX_8 "o"
#define PRIo16      _PFX_16 "o"
#define PRIo32      _PFX_32 "o"
#define PRIo64      _PFX_64 "o"
#define PRIoLEAST8  _PFX_8 "o"
#define PRIoLEAST16 _PFX_16 "o"
#define PRIoLEAST32 _PFX_32 "o"
#define PRIoLEAST64 _PFX_64 "o"
#define PRIoFAST8   _PFX_8 "o"
#define PRIoFAST16  _PFX_16 "o"
#define PRIoFAST32  _PFX_32 "o"
#define PRIoFAST64  _PFX_64 "o"
#define PRIoMAX     _PFX_64 "o"
#define PRIoPTR     _PFX_PTR "o"

#define PRIu8       _PFX_8 "u"
#define PRIu16      _PFX_16 "u"
#define PRIu32      _PFX_32 "u"
#define PRIu64      _PFX_64 "u"
#define PRIuLEAST8  _PFX_8 "u"
#define PRIuLEAST16 _PFX_16 "u"
#define PRIuLEAST32 _PFX_32 "u"
#define PRIuLEAST64 _PFX_64 "u"
#define PRIuFAST8   _PFX_8 "u"
#define PRIuFAST16  _PFX_16 "u"
#define PRIuFAST32  _PFX_32 "u"
#define PRIuFAST64  _PFX_64 "u"
#define PRIuMAX     _PFX_64 "u"
#define PRIuPTR     _PFX_PTR "u"

#define PRIx8       _PFX_8 "x"
#define PRIx16      _PFX_16 "x"
#define PRIx32      _PFX_32 "x"
#define PRIx64      _PFX_64 "x"
#define PRIxLEAST8  _PFX_8 "x"
#define PRIxLEAST16 _PFX_16 "x"
#define PRIxLEAST32 _PFX_32 "x"
#define PRIxLEAST64 _PFX_64 "x"
#define PRIxFAST8   _PFX_8 "x"
#define PRIxFAST16  _PFX_16 "x"
#define PRIxFAST32  _PFX_32 "x"
#define PRIxFAST64  _PFX_64 "x"
#define PRIxMAX     _PFX_64 "x"
#define PRIxPTR     _PFX_PTR "x"

#define PRIX8       _PFX_8 "X"
#define PRIX16      _PFX_16 "X"
#define PRIX32      _PFX_32 "X"
#define PRIX64      _PFX_64 "X"
#define PRIXLEAST8  _PFX_8 "X"
#define PRIXLEAST16 _PFX_16 "X"
#define PRIXLEAST32 _PFX_32 "X"
#define PRIXLEAST64 _PFX_64 "X"
#define PRIXFAST8   _PFX_8 "X"
#define PRIXFAST16  _PFX_16 "X"
#define PRIXFAST32  _PFX_32 "X"
#define PRIXFAST64  _PFX_64 "X"
#define PRIXMAX     _PFX_64 "X"
#define PRIXPTR     _PFX_PTR "X"

#define SCNd8       _PFX_8 "d"
#define SCNd16      _PFX_16 "d"
#define SCNd32      _PFX_32 "d"
#define SCNd64      _PFX_64 "d"
#define SCNdLEAST8  _PFX_8 "d"
#define SCNdLEAST16 _PFX_16 "d"
#define SCNdLEAST32 _PFX_32 "d"
#define SCNdLEAST64 _PFX_64 "d"
#define SCNdFAST8   _PFX_8 "d"
#define SCNdFAST16  _PFX_16 "d"
#define SCNdFAST32  _PFX_32 "d"
#define SCNdFAST64  _PFX_64 "d"
#define SCNdMAX     _PFX_64 "d"
#define SCNdPTR     _PFX_PTR "d"

#define SCNi8       _PFX_8 "i"
#define SCNi16      _PFX_16 "i"
#define SCNi32      _PFX_32 "i"
#define SCNi64      _PFX_64 "i"
#define SCNiLEAST8  _PFX_8 "i"
#define SCNiLEAST16 _PFX_16 "i"
#define SCNiLEAST32 _PFX_32 "i"
#define SCNiLEAST64 _PFX_64 "i"
#define SCNiFAST8   _PFX_8 "i"
#define SCNiFAST16  _PFX_16 "i"
#define SCNiFAST32  _PFX_32 "i"
#define SCNiFAST64  _PFX_64 "i"
#define SCNiMAX     _PFX_64 "i"
#define SCNiPTR     _PFX_PTR "i"

#define SCNo8       _PFX_8 "o"
#define SCNo16      _PFX_16 "o"
#define SCNo32      _PFX_32 "o"
#define SCNo64      _PFX_64 "o"
#define SCNoLEAST8  _PFX_8 "o"
#define SCNoLEAST16 _PFX_16 "o"
#define SCNoLEAST32 _PFX_32 "o"
#define SCNoLEAST64 _PFX_64 "o"
#define SCNoFAST8   _PFX_8 "o"
#define SCNoFAST16  _PFX_16 "o"
#define SCNoFAST32  _PFX_32 "o"
#define SCNoFAST64  _PFX_64 "o"
#define SCNoMAX     _PFX_64 "o"
#define SCNoPTR     _PFX_PTR "o"

#define SCNu8       _PFX_8 "u"
#define SCNu16      _PFX_16 "u"
#define SCNu32      _PFX_32 "u"
#define SCNu64      _PFX_64 "u"
#define SCNuLEAST8  _PFX_8 "u"
#define SCNuLEAST16 _PFX_16 "u"
#define SCNuLEAST32 _PFX_32 "u"
#define SCNuLEAST64 _PFX_64 "u"
#define SCNuFAST8   _PFX_8 "u"
#define SCNuFAST16  _PFX_16 "u"
#define SCNuFAST32  _PFX_32 "u"
#define SCNuFAST64  _PFX_64 "u"
#define SCNuMAX     _PFX_64 "u"
#define SCNuPTR     _PFX_PTR "u"

#define SCNx8       _PFX_8 "x"
#define SCNx16      _PFX_16 "x"
#define SCNx32      _PFX_32 "x"
#define SCNx64      _PFX_64 "x"
#define SCNxLEAST8  _PFX_8 "x"
#define SCNxLEAST16 _PFX_16 "x"
#define SCNxLEAST32 _PFX_32 "x"
#define SCNxLEAST64 _PFX_64 "x"
#define SCNxFAST8   _PFX_8 "x"
#define SCNxFAST16  _PFX_16 "x"
#define SCNxFAST32  _PFX_32 "x"
#define SCNxFAST64  _PFX_64 "x"
#define SCNxMAX     _PFX_64 "x"
#define SCNxPTR     _PFX_PTR "x"

#endif  /* C99 */

#endif  /* !_INTTYPES_H */
