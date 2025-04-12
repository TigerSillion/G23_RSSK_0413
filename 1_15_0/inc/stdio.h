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

#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>
#include <stdarg.h>
#define EOF (-1)

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
int __far printf(const char __far * restrict format, ...);
int __far printf_tiny(const char __far * restrict format, ...);
int __far scanf(const char __far * restrict format, ...);
int __far snprintf(char __far * restrict s, size_t n, const char __far * restrict format, ...);
int __far sprintf(char __far * restrict s, const char __far * restrict format, ...);
int __far sprintf_tiny(char __far * restrict s, const char __far * restrict format, ...);
int __far sscanf(const char __far * restrict s, const char __far * restrict format, ...);
int __far vprintf(const char __far * restrict format, va_list arg);
int __far vscanf(const char __far * restrict format, va_list arg);
int __far vsnprintf(char __far * restrict s, size_t n, const char __far * restrict format, va_list arg);
int __far vsprintf(char __far * restrict s, const char __far * restrict format, va_list arg);
int __far vsscanf(const char __far * restrict s, const char __far * restrict format, va_list arg);
#else   /* C99 */
int __far printf(const char __far *format, ...);
int __far printf_tiny(const char __far *format, ...);
int __far scanf(const char __far *format, ...);
int __far sprintf(char __far *s, const char __far *format, ...);
int __far sprintf_tiny(char __far *s, const char __far *format, ...);
int __far sscanf(const char __far *s, const char __far *format, ...);
int __far vprintf(const char __far *format, va_list arg);
int __far vsprintf(char __far *s, const char __far *format, va_list arg);
#endif  /* C99 */

int __far getchar(void);
char __near * __far gets(char __near *s);
char __far * __far _COM_gets_f(char __far *s);

int __far putchar(int c);
int __far puts(const char __near *s);
int __far _COM_puts_f(const char __far *s);

void __far perror(const char __near *s);
void __far _COM_perror_f(const char __far *s);

#if defined(__FAR_ROM__)
#define puts(_s)     _COM_puts_f((_s))
#define perror(_s)   _COM_perror_f((_s))
#endif  /* __FAR_ROM__ */

#if defined(__PRINTF_TINY__)
#define printf      printf_tiny
#define sprintf     sprintf_tiny
#endif  /* __PRINTF_TINY__ */

#endif  /* !_STDIO_H */
