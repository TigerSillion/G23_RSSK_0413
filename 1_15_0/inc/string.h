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

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
void __near * __far memcpy(void __near * restrict s1, const void __near * restrict s2, size_t n);
void __far * __far _COM_memcpy_ff(void __far * restrict s1, const void __far * restrict s2, size_t n);
#else   /* C99 */
void __near * __far memcpy(void __near *s1, const void __near *s2, size_t n);
void __far * __far _COM_memcpy_ff(void __far *s1, const void __far *s2, size_t n);
#endif  /* C99 */

void __near * __far memmove(void __near *s1, const void __near *s2, size_t n);
void __far * __far _COM_memmove_ff(void __far *s1, const void __far *s2, size_t n);

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
char __near * __far strcpy(char __near * restrict s1, const char __near * restrict s2);
char __far * __far _COM_strcpy_ff(char __far * restrict s1, const char __far * restrict s2);

char __near * __far strncpy(char __near * restrict s1, const char __near * restrict s2, size_t n);
char __far * __far _COM_strncpy_ff(char __far * restrict s1, const char __far * restrict s2, size_t n);

char __near * __far strcat(char __near * restrict s1, const char __near * restrict s2);
char __far * __far _COM_strcat_ff(char __far * restrict s1, const char __far * restrict s2);

char __near * __far strncat(char __near * restrict s1, const char __near * restrict s2, size_t n);
char __far * __far _COM_strncat_ff(char __far * restrict s1, const char __far * restrict s2, size_t n);
#else   /* C99 */
char __near * __far strcpy(char __near *s1, const char __near *s2);
char __far * __far _COM_strcpy_ff(char __far *s1, const char __far *s2);

char __near * __far strncpy(char __near *s1, const char __near *s2, size_t n);
char __far * __far _COM_strncpy_ff(char __far *s1, const char __far *s2, size_t n);

char __near * __far strcat(char __near *s1, const char __near *s2);
char __far * __far _COM_strcat_ff(char __far *s1, const char __far *s2);

char __near * __far strncat(char __near *s1, const char __near *s2, size_t n);
char __far * __far _COM_strncat_ff(char __far *s1, const char __far *s2, size_t n);
#endif  /* C99 */

int __far memcmp(const void __near *s1, const void __near *s2, size_t n);
int __far _COM_memcmp_ff(const void __far *s1, const void __far *s2, size_t n);

int __far strcmp(const char __near *s1, const char __near *s2);
int __far _COM_strcmp_ff(const char __far *s1, const char __far *s2);

int __far strncmp(const char __near *s1, const char __near *s2, size_t n);
int __far _COM_strncmp_ff(const char __far *s1, const char __far *s2, size_t n);

void __near * __far memchr(const void __near *s, int c, size_t n);
void __far * __far _COM_memchr_f(const void __far *s, int c, size_t n);

char __near * __far strchr(const char __near *s, int c);
char __far * __far _COM_strchr_f(const char __far *s, int c);

size_t __far strcspn(const char __near *s1, const char __near *s2);
size_t __far _COM_strcspn_ff(const char __far *s1, const char __far *s2);

char __near * __far strpbrk(const char __near *s1, const char __near *s2);
char __far * __far _COM_strpbrk_ff(const char __far *s1, const char __far *s2);

char __near * __far strrchr(const char __near *s, int c);
char __far * __far _COM_strrchr_f(const char __far *s, int c);

size_t __far strspn(const char __near *s1, const char __near *s2);
size_t __far _COM_strspn_ff(const char __far *s1, const char __far *s2);

char __near * __far strstr(const char __near *s1, const char __near *s2);
char __far * __far _COM_strstr_ff(const char __far *s1, const char __far *s2);

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
char __far * __far strtok(char __far * restrict s1, const char __far * restrict s2);
#else   /* C99 */
char __far * __far strtok(char __far *s1, const char __far *s2);
#endif  /* C99 */

void __near * __far memset(void __near *s, int c, size_t n);
void __far * __far _COM_memset_f(void __far *s, int c, size_t n);

char __far * __far strerror(int errnum);

size_t __far strlen(const char __near *s);
size_t __far _COM_strlen_f(const char __far *s);

#if defined(__FAR_ROM__)
#define memcpy(_s1, _s2, _n)   _COM_memcpy_ff((_s1), (_s2), (_n))
#define memmove(_s1, _s2, _n)  _COM_memmove_ff((_s1), (_s2), (_n))
#define strcpy(_s1, _s2)       _COM_strcpy_ff((_s1), (_s2))
#define strncpy(_s1, _s2, _n)  _COM_strncpy_ff((_s1), (_s2), (_n))
#define strcat(_s1, _s2)       _COM_strcat_ff((_s1), (_s2))
#define strncat(_s1, _s2, _n)  _COM_strncat_ff((_s1), (_s2), (_n))
#define memcmp(_s1, _s2, _n)   _COM_memcmp_ff((_s1), (_s2), (_n))
#define strcmp(_s1, _s2)       _COM_strcmp_ff((_s1), (_s2))
#define strncmp(_s1, _s2, _n)  _COM_strncmp_ff((_s1), (_s2), (_n))
#define memchr(_s, _c, _n)     _COM_memchr_f((_s), (_c), (_n))
#define strchr(_s, _c)         _COM_strchr_f((_s), (_c))
#define strcspn(_s1, _s2)      _COM_strcspn_ff((_s1), (_s2))
#define strpbrk(_s1, _s2)      _COM_strpbrk_ff((_s1), (_s2))
#define strrchr(_s, _c)        _COM_strrchr_f((_s), (_c))
#define strspn(_s1, _s2)       _COM_strspn_ff((_s1), (_s2))
#define strstr(_s1, _s2)       _COM_strstr_ff((_s1), (_s2))
#define memset(_s, _c, _n)     _COM_memset_f((_s), (_c), (_n))
#define strlen(_s)             _COM_strlen_f((_s))
#endif  /* __FAR_ROM__ */

#endif  /* !_STRING_H */
