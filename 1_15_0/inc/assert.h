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

#undef  assert
#ifdef  NDEBUG
#define assert(test)    ((void)0)
#else   /* NDEBUG */
#include <stdio.h>
#include <stdlib.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define assert(_test)    ((_test) ? (void)0 : (printf("Assertion failed : %s, function %s, file %s, line %d\n", #_test, __func__, __FILE__, __LINE__), abort(), (void)0))
#else   /* C99 */
#define assert(_test)    ((_test) ? (void)0 : (printf("Assertion failed : %s, file %s, line %d\n", #_test, __FILE__, __LINE__), abort(), (void)0))
#endif  /* C99 */
#endif  /* NDEBUG */
