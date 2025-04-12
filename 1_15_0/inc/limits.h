/*
 * Copyright (C) 2014, 2017 Renesas Electronics Corporation
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

#ifndef _LIMITS_H
#define _LIMITS_H

#define CHAR_BIT    (8)
#define SCHAR_MIN   (-127 - 1)
#define SCHAR_MAX   (+127)
#define UCHAR_MAX   (255U)
#if defined(__SCHAR)
#define CHAR_MIN    SCHAR_MIN
#define CHAR_MAX    SCHAR_MAX
#else   /* __SCHAR */
#define CHAR_MIN    (0)
#define CHAR_MAX    UCHAR_MAX
#endif  /* __SCHAR */

#define SHRT_MIN    (-32767 - 1)
#define SHRT_MAX    (+32767)
#define USHRT_MAX   (65535U)

#define INT_MIN     SHRT_MIN
#define INT_MAX     SHRT_MAX
#define UINT_MAX    USHRT_MAX

#define LONG_MIN    (-2147483647L - 1)
#define LONG_MAX    (+2147483647L)
#define ULONG_MAX   (4294967295UL)

#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define LLONG_MIN   (-9223372036854775807LL - 1)
#define LLONG_MAX   (+9223372036854775807LL)
#define ULLONG_MAX  (18446744073709551615ULL)
#endif  /* !__STDC__ || C99 */

#endif  /* !_LIMITS_H */
