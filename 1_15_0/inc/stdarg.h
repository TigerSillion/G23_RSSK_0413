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

#ifndef _STDARG_H
#define _STDARG_H

#if defined(__CNV_CA78K0R__)
#define va_starttop(_ap,_parmN)    va_start((_ap),(_parmN))
#endif  /* __CNV_CA78K0R__ */

typedef char __near *va_list;
void __near __builtin_va_start(va_list);
#define va_start(_ap,_parmN)  __builtin_va_start((_ap))
#define va_arg(_ap,_type)     (*(_type *)(((_ap)+=((sizeof(_type) + 1) & ~1))-((sizeof(_type) + 1) & ~1)))

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define va_copy(_dest,_src)   ((_dest)=(_src))
#endif  /* C99 */

#define va_end(_ap)           ((_ap)=(va_list)0, (void)0)

#endif  /* !_STDARG_H */
