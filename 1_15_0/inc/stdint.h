/*
 * Copyright (C) 2014, 2019 Renesas Electronics Corporation
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

#ifndef _STDINT_H
#define _STDINT_H

typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed long         int32_t;
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
typedef signed long long    int64_t;
#endif  /* !__STDC__ || C99 */

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long       uint32_t;
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
typedef unsigned long long  uint64_t;
#endif  /* !__STDC__ || C99 */

typedef int8_t              int_least8_t;
typedef int16_t             int_least16_t;
typedef int32_t             int_least32_t;
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
typedef int64_t             int_least64_t;
#endif  /* !__STDC__ || C99 */

typedef uint8_t             uint_least8_t;
typedef uint16_t            uint_least16_t;
typedef uint32_t            uint_least32_t;
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
typedef uint64_t            uint_least64_t;
#endif  /* !__STDC__ || C99 */

typedef int8_t              int_fast8_t;
typedef int16_t             int_fast16_t;
typedef int32_t             int_fast32_t;
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
typedef int64_t             int_fast64_t;
#endif  /* !__STDC__ || C99 */

typedef uint8_t             uint_fast8_t;
typedef uint16_t            uint_fast16_t;
typedef uint32_t            uint_fast32_t;
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
typedef uint64_t            uint_fast64_t;
#endif  /* !__STDC__ || C99 */

typedef int32_t             intptr_t;
typedef uint32_t            uintptr_t;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L)
typedef int32_t             intmax_t;
typedef uint32_t            uintmax_t;
#else   /* __STDC__ && C90 */
typedef int64_t             intmax_t;
typedef uint64_t            uintmax_t;
#endif  /* __STDC__ && C90 */

#define INT8_MIN            (-0x7F - 1)
#define INT16_MIN           (-0x7FFF - 1)
#define INT32_MIN           (-0x7FFFFFFF - 1)
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT64_MIN           (-0x7FFFFFFFFFFFFFFF - 1)
#endif  /* !__STDC__ || C99 */

#define INT8_MAX            (0x7F)
#define INT16_MAX           (0x7FFF)
#define INT32_MAX           (0x7FFFFFFF)
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT64_MAX           (0x7FFFFFFFFFFFFFFF)
#endif  /* !__STDC__ || C99 */

#define UINT8_MAX           (0xFFU)
#define UINT16_MAX          (0xFFFFU)
#define UINT32_MAX          (0xFFFFFFFFU)
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define UINT64_MAX          (0xFFFFFFFFFFFFFFFFU)
#endif  /* !__STDC__ || C99 */

#define INT_LEAST8_MIN      INT8_MIN
#define INT_LEAST16_MIN     INT16_MIN
#define INT_LEAST32_MIN     INT32_MIN
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT_LEAST64_MIN     INT64_MIN
#endif  /* !__STDC__ || C99 */

#define INT_LEAST8_MAX      INT8_MAX
#define INT_LEAST16_MAX     INT16_MAX
#define INT_LEAST32_MAX     INT32_MAX
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT_LEAST64_MAX     INT64_MAX
#endif  /* !__STDC__ || C99 */

#define UINT_LEAST8_MAX     UINT8_MAX
#define UINT_LEAST16_MAX    UINT16_MAX
#define UINT_LEAST32_MAX    UINT32_MAX
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define UINT_LEAST64_MAX    UINT64_MAX
#endif  /* !__STDC__ || C99 */

#define INT_FAST8_MIN       INT_LEAST8_MIN
#define INT_FAST16_MIN      INT_LEAST16_MIN
#define INT_FAST32_MIN      INT_LEAST32_MIN
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT_FAST64_MIN      INT_LEAST64_MIN
#endif  /* !__STDC__ || C99 */

#define INT_FAST8_MAX       INT_LEAST8_MAX
#define INT_FAST16_MAX      INT_LEAST16_MAX
#define INT_FAST32_MAX      INT_LEAST32_MAX
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT_FAST64_MAX      INT_LEAST64_MAX
#endif  /* !__STDC__ || C99 */

#define UINT_FAST8_MAX      UINT_LEAST8_MAX
#define UINT_FAST16_MAX     UINT_LEAST16_MAX
#define UINT_FAST32_MAX     UINT_LEAST32_MAX
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define UINT_FAST64_MAX     UINT_LEAST64_MAX
#endif  /* !__STDC__ || C99 */

#define INTPTR_MIN          INT32_MIN
#define INTPTR_MAX          INT32_MAX
#define UINTPTR_MAX         UINT32_MAX

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L)
#define INTMAX_MIN          INT32_MIN
#define INTMAX_MAX          INT32_MAX
#define UINTMAX_MAX         UINT32_MAX
#else   /* __STDC__ && C90 */
#define INTMAX_MIN          INT64_MIN
#define INTMAX_MAX          INT64_MAX
#define UINTMAX_MAX         UINT64_MAX
#endif  /* __STDC__ && C90 */

#define PTRDIFF_MIN         INT16_MIN
#define PTRDIFF_MAX         INT16_MAX

#define SIZE_MAX            UINT16_MAX

#define INT8_C(_x)           (_x)
#define INT16_C(_x)          (_x)
#define INT32_C(_x)          (_x ## L)
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define INT64_C(_x)          (_x ## LL)
#endif  /* !__STDC__ || C99 */

#define UINT8_C(_x)          (_x)
#define UINT16_C(_x)         (_x ## U)
#define UINT32_C(_x)         (_x ## UL)
#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
#define UINT64_C(_x)         (_x ## ULL)
#endif  /* !__STDC__ || C99 */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L)
#define INTMAX_C(_x)         INT32_C(_x)
#define UINTMAX_C(_x)        UINT32_C(_x)
#else   /* __STDC__ && C90 */
#define INTMAX_C(_x)         INT64_C(_x)
#define UINTMAX_C(_x)        UINT64_C(_x)
#endif  /* __STDC__ && C90 */

#endif  /* !_STDINT_H */
