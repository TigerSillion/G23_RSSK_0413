/*
 * Copyright (C) 2014, 2023 Renesas Electronics Corporation
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

#ifndef _BUILTIN_H
#define _BUILTIN_H

void __near __DI(void);
void __near __EI(void);
void __near __halt(void);
void __near __stop(void);
void __near __brk(void);
void __near __nop(void);

unsigned char __near __rolb(unsigned char, unsigned char);
unsigned char __near __rorb(unsigned char, unsigned char);
unsigned int  __near __rolw(unsigned int, unsigned char);
unsigned int  __near __rorw(unsigned int, unsigned char);

unsigned int  __near __mulu(unsigned char, unsigned char);
#if defined(__RL78_S3__)
unsigned long __near __mului(unsigned int, unsigned int);
signed long   __near __mulsi(signed int, signed int);
#else   /* __RL78_S3__ */
unsigned long __far     _COM_mului(unsigned int, unsigned int);
signed long __far       _COM_mulsi(signed int, signed int);
#define __mului(_x, _y)   _COM_mului((_x), (_y))
#define __mulsi(_x, _y)   _COM_mulsi((_x), (_y))
#endif  /* __RL78_S3__ */

#if !(defined(__STDC_VERSION__) && (__STDC_VERSION__ == 199409L))
unsigned long long __far    _COM_mulul(unsigned long, unsigned long);
signed long long __far      _COM_mulsl(signed long, signed long);
#define __mulul(_x, _y)     _COM_mulul((_x), (_y))
#define __mulsl(_x, _y)     _COM_mulsl((_x), (_y))
#endif  /* !__STDC__ || C99 */

unsigned int __far      _COM_divui(unsigned int, unsigned char);
unsigned long __far     _COM_divul(unsigned long, unsigned int);
#define __divui(_x, _y)   _COM_divui((_x), (_y))
#define __divul(_x, _y)   _COM_divul((_x), (_y))

unsigned char __far     _COM_remui(unsigned int, unsigned char);
unsigned int __far      _COM_remul(unsigned long, unsigned int);
#define __remui(_x, _y)   _COM_remui((_x), (_y))
#define __remul(_x, _y)   _COM_remul((_x), (_y))

unsigned long __far     _COM_macui(unsigned int, unsigned int, unsigned long);
signed long __far       _COM_macsi(signed int, signed int, signed long);
#define __macui(_x, _y, _z)    _COM_macui((_x), (_y), (_z))
#define __macsi(_x, _y, _z)    _COM_macsi((_x), (_y), (_z))

unsigned char __near    __get_psw(void);
void __near             __set_psw(unsigned char);

void __near __set1(unsigned char __near *, unsigned char);
void __near __clr1(unsigned char __near *, unsigned char);
void __near __not1(unsigned char __near *, unsigned char);

#if defined(__CNV_CA78K0R__)
#define __K0R__                 1
#define __CA78K0R__             1
#if defined(__RL78_S1__)
#define __RL78_3__              1
#elif defined(__RL78_S2__)
#define __RL78_1__              1
#elif defined(__RL78_S3__)
#define __RL78_2__              1
#endif  /* __RL78_S1__ */
#if defined(__RL78_SMALL__)
#define __K0R_SMALL__           1
#elif defined(__RL78_MEDIUM__)
#define __K0R_MEDIUM__          1
#endif  /* __RL78_SMALL__ */
#if defined(__UCHAR)
#define __CHAR_UNSIGNED__       1
#endif  /* __UCHAR */
#endif  /* __CNV_CA78K0R__ */

#if defined(__CNV_NC30__)
#define NC30
#define M16C
#define __R8C__
#endif  /* __CNV_NC30__ */

#if defined(__CNV_IAR__)
#define __break()               __brk()
#define __disable_interrupt()   __DI()
#define __enable_interrupt()    __EI()
#define __no_operation()        __nop()
#define __RL78_0__              1
#define __RL78_1__              2
#define __RL78_2__              3
#define __CODE_MODEL_NEAR__     1
#define __CODE_MODEL_FAR__      2
#define __DATA_MODEL_NEAR__     1
#define __DATA_MODEL_FAR__      2
#if defined(__RL78_S1__)
#define __CORE__                __RL78_0__
#elif defined(__RL78_S2__)
#define __CORE__                __RL78_1__
#elif defined(__RL78_S3__)
#define __CORE__                __RL78_2__
#endif  /* __RL78_S1__ */
#if defined(__RL78_SMALL__)
#define __CODE_MODEL__          __CODE_MODEL_NEAR__
#define __DATA_MODEL__          __DATA_MODEL_NEAR__
#elif defined(__RL78_MEDIUM__)
#define __CODE_MODEL__          __CODE_MODEL_FAR__
#define __DATA_MODEL__          __DATA_MODEL_NEAR__
#endif  /* __RL78_SMALL__ */
#define __IAR_SYSTEMS_ICC__     8
#define __ICCRL78__             1
#endif  /* __CNV_IAR__ */

#endif  /* !_BUILTIN_H */
