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

#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>
#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define RAND_MAX        0x7fff

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long int quot;
    long int rem;
} ldiv_t;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
typedef struct {
    long long int quot;
    long long int rem;
} lldiv_t;
#endif  /* C99 */

double __far atof(const char __near *nptr);
double __far _COM_atof_f(const char __far *nptr);

float __far atoff(const char __near *nptr);
float __far _COM_atoff_f(const char __far *nptr);

int __far atoi(const char __near *nptr);
int __far _COM_atoi_f(const char __far *nptr);

long int __far atol(const char __near *nptr);
long int __far _COM_atol_f(const char __far *nptr);

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
long long int __far atoll(const char __near *nptr);
long long int __far _COM_atoll_f(const char __far *nptr);

double __far strtod(const char __near * restrict nptr, char __near * __near * restrict endptr);
double __far _COM_strtod_ff(const char __far * restrict nptr, char __far * __far * restrict endptr);

float __far strtof(const char __near * restrict nptr, char __near * __near * restrict endptr);
float __far _COM_strtof_ff(const char __far * restrict nptr, char __far * __far * restrict endptr);

long double __far strtold(const char __near * restrict nptr, char __near * __near * restrict endptr);
long double __far _COM_strtold_ff(const char __far * restrict nptr, char __far * __far * restrict endptr);

long int __far strtol(const char __near * restrict nptr, char __near * __near * restrict endptr, int base);
long int __far _COM_strtol_ff(const char __far * restrict nptr, char __far * __far * restrict endptr, int base);

long long int __far strtoll(const char __near * restrict nptr, char __near * __near * restrict endptr, int base);
long long int __far _COM_strtoll_ff(const char __far * restrict nptr, char __far * __far * restrict endptr, int base);

unsigned long int __far strtoul(const char __near * restrict nptr, char __near * __near * restrict endptr, int base);
unsigned long int __far _COM_strtoul_ff(const char __far * restrict nptr, char __far * __far * restrict endptr, int base);

unsigned long long int __far strtoull(const char __near * restrict nptr, char __near * __near * restrict endptr, int base);
unsigned long long int __far _COM_strtoull_ff(const char __far * restrict nptr, char __far * __far * restrict endptr, int base);
#else   /* C99 */
double __far strtod(const char __near *nptr, char __near * __near *endptr);
double __far _COM_strtod_ff(const char __far *nptr, char __far * __far *endptr);

float __far strtof(const char __near *nptr, char __near * __near *endptr);
float __far _COM_strtof_ff(const char __far *nptr, char __far * __far *endptr);

long int __far strtol(const char __near *nptr, char __near * __near *endptr, int base);
long int __far _COM_strtol_ff(const char __far *nptr, char __far * __far *endptr, int base);

unsigned long int __far strtoul(const char __near *nptr, char __near * __near *endptr, int base);
unsigned long int __far _COM_strtoul_ff(const char __far *nptr, char __far * __far *endptr, int base);
#endif  /* C99 */

int __far rand(void);
void __far srand(unsigned int seed);

void __near * __far calloc(size_t nmemb, size_t size);
void __far free(void __near *ptr);
void __near * __far malloc(size_t size);
void __near * __far realloc(void __near *ptr, size_t size);

void __far abort(void);

void __near * __far bsearch(const void __near *key, const void __near *base, size_t nmemb, size_t size, int (__far *compar)(const void __near *, const void __near *));
void __far * __far _COM_bsearch_f(const void __far *key, const void __far *base, size_t nmemb, size_t size, int (__far *compar)(const void __far *, const void __far *));

void __far qsort(void __near *base, size_t nmemb, size_t size, int (__far *compar)(const void __near *, const void __near *));
void __far _COM_qsort_f(void __far *base, size_t nmemb, size_t size, int (__far *compar)(const void __far *, const void __far *));

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
int __far abs(int j);
long int __far labs(long int j);
long long int __far llabs(long long int j);
div_t __far div(int numer, int denom);
ldiv_t __far ldiv(long int numer, long int denom);
lldiv_t __far lldiv(long long int numer, long long int denom);
#else   /* C99 */
int __far abs(int j);
div_t __far div(int numer, int denom);
long int __far labs(long int j);
ldiv_t __far ldiv(long int numer, long int denom);
#endif  /* C99 */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define strtold(_nptr, _endptr)           strtod((_nptr), (_endptr))
#define _COM_strtold_ff(_nptr, _endptr)   _COM_strtod_ff((_nptr), (_endptr))
#elif defined(__CNV_IAR__)
#define strtold(_nptr, _endptr)           strtod((_nptr), (_endptr))
#endif  /* C99 */
#if defined(__FAR_ROM__)
#if defined(__DBL4)
#define atof(_nptr)                       _COM_atoff_f((_nptr))
#define _COM_atof_f(_nptr)                _COM_atoff_f((_nptr))
#define strtod(_nptr, _endptr)            _COM_strtof_ff((_nptr), (_endptr))
#define _COM_strtod_ff(_nptr, _endptr)    _COM_strtof_ff((_nptr), (_endptr))
#else   /* __DBL4 */
#define atof(_nptr)                       _COM_atof_f((_nptr))
#define strtod(_nptr, _endptr)            _COM_strtod_ff((_nptr), (_endptr))
#endif  /* __DBL4 */
#define atoff(_nptr)                      _COM_atoff_f((_nptr))
#define atoi(_nptr)                       _COM_atoi_f((_nptr))
#define atol(_nptr)                       _COM_atol_f((_nptr))
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define atoll(_nptr)                      _COM_atoll_f((_nptr))
#endif  /* C99 */
#define strtof(_nptr, _endptr)            _COM_strtof_ff((_nptr), (_endptr))
#define strtol(_nptr, _endptr, _base)     _COM_strtol_ff((_nptr), (_endptr), (_base))
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define strtoll(_nptr, _endptr, _base)    _COM_strtoll_ff((_nptr), (_endptr), (_base))
#endif  /* C99 */
#define strtoul(_nptr, _endptr, _base)    _COM_strtoul_ff((_nptr), (_endptr), (_base))
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define strtoull(_nptr, _endptr, _base)   _COM_strtoull_ff((_nptr), (_endptr), (_base))
#endif  /* C99 */
#define bsearch(_key, _base, _nmemb, _size, _compar) _COM_bsearch_f((_key), (_base), (_nmemb), (_size), (_compar))
#define qsort(_base, _nmemb, _size, _compar)         _COM_qsort_f((_base), (_nmemb), (_size), (_compar))
#else   /* __FAR_ROM__ */
#if defined(__DBL4)
#define atof(_nptr)                       atoff((_nptr))
#define _COM_atof_f(_nptr)                _COM_atoff_f((_nptr))
#define strtod(_nptr, _endptr)            strtof((_nptr), (_endptr))
#define _COM_strtod_ff(_nptr, _endptr)    _COM_strtof_ff((_nptr), (_endptr))
#endif  /* __DBL4 */
#endif  /* __FAR_ROM__ */

#endif  /* !_STDLIB_H */
