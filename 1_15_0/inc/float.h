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

#ifndef _FLOAT_H
#define _FLOAT_H

#define FLT_ROUNDS      (+1)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define FLT_EVAL_METHOD (0)
#endif  /* C99 */
#define FLT_RADIX       (+2)

#define FLT_MANT_DIG    (+24)
#define FLT_DIG         (+6)
#define FLT_MIN_EXP     (-125)
#define FLT_MIN_10_EXP  (-37)
#define FLT_MAX_EXP     (+128)
#define FLT_MAX_10_EXP  (+38)
#define FLT_MAX         (3.40282347E+38F)
#define FLT_EPSILON     (1.19209290E-07F)
#define FLT_MIN         (1.17549435E-38F)

#if defined(__DBL4)
#define DBL_MANT_DIG    (+24)
#define LDBL_MANT_DIG   (+24)

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define DECIMAL_DIG     (+9)
#endif  /* C99 */

#define DBL_DIG         (+6)
#define LDBL_DIG        (+6)

#define DBL_MIN_EXP     (-125)
#define LDBL_MIN_EXP    (-125)

#define DBL_MIN_10_EXP  (-37)
#define LDBL_MIN_10_EXP (-37)

#define DBL_MAX_EXP     (+128)
#define LDBL_MAX_EXP    (+128)

#define DBL_MAX_10_EXP  (+38)
#define LDBL_MAX_10_EXP (+38)

#define DBL_MAX         (3.40282347E+38F)
#define LDBL_MAX        (3.40282347E+38F)

#define DBL_EPSILON     (1.19209290E-07F)
#define LDBL_EPSILON    (1.19209290E-07F)

#define DBL_MIN         (1.17549435E-38F)
#define LDBL_MIN        (1.17549435E-38F)

#else   /* __DBL4 */
#define DBL_MANT_DIG    (+53)
#define LDBL_MANT_DIG   (+53)

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define DECIMAL_DIG     (+17)
#endif  /* C99 */

#define DBL_DIG         (+15)
#define LDBL_DIG        (+15)

#define DBL_MIN_EXP     (-1021)
#define LDBL_MIN_EXP    (-1021)

#define DBL_MIN_10_EXP  (-307)
#define LDBL_MIN_10_EXP (-307)

#define DBL_MAX_EXP     (+1024)
#define LDBL_MAX_EXP    (+1024)

#define DBL_MAX_10_EXP  (+308)
#define LDBL_MAX_10_EXP (+308)

#define DBL_MAX         (1.7976931348623158E+308)
#define LDBL_MAX        (1.7976931348623158E+308)

#define DBL_EPSILON     (2.2204460492503131E-016)
#define LDBL_EPSILON    (2.2204460492503131E-016)

#define DBL_MIN         (2.2250738585072014E-308)
#define LDBL_MIN        (2.2250738585072014E-308)
#endif  /* __DBL4 */

#endif  /* !_FLOAT_H */
