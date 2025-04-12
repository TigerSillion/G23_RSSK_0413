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

#ifndef _MATH_H
#define _MATH_H

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
typedef float  float_t;
typedef double double_t;
#endif  /* C99 */

#if defined(__DBL4)
#define HUGE_VAL    (__INFINITY__)
#define HUGE_VALF   (__INFINITY__)
#define HUGE_VALL   (__INFINITY__)
#else   /* __DBL4 */
#define HUGE_VAL    ((double)__INFINITY__)
#define HUGE_VALF   (__INFINITY__)
#define HUGE_VALL   ((long double)__INFINITY__)
#endif  /* __DBL4 */
#define INFINITY    (__INFINITY__)
#define NAN         (__NAN__)

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define FP_INFINITE         (+1)
#define FP_NAN              (+2)
#define FP_NORMAL           (-1)
#define FP_SUBNORMAL        (-2)
#define FP_ZERO             (0)
#define MATH_ERRNO          1
#define MATH_ERREXCEPT      2
#define math_errhandling    MATH_ERRNO

int __far __fpclassifyf(float x);
int __far __fpclassifyd(double x);
int __far __signbitf(float x);
int __far __signbitd(double x);
int __far __islessgreaterf(float x, float y);
int __far __islessgreaterd(double x, double y);

#define fpclassify(_x)       ((sizeof(_x) == sizeof(float)) ? \
                             __fpclassifyf((_x)) : __fpclassifyd((_x)))
#define isfinite(_x)         (fpclassify((_x)) <= FP_ZERO)
#define isinf(_x)            (fpclassify((_x)) == FP_INFINITE)
#define isnan(_x)            (fpclassify((_x)) == FP_NAN)
#define isnormal(_x)         (fpclassify((_x)) == FP_NORMAL)
#define signbit(_x)          ((sizeof(_x) == sizeof(float)) ? \
                             __signbitf((_x)) : __signbitd((_x)))
#define isgreater(_x, _y)        ((_x) > (_y))
#define isgreaterequal(_x, _y)   ((_x) >= (_y))
#define isless(_x, _y)           ((_x) < (_y))
#define islessequal(_x, _y)      ((_x) <= (_y))
#define islessgreater(_x, _y)    ((sizeof((_x)+(_y)) == sizeof(float)) ? \
                                 __islessgreaterf((_x), (_y)) : \
                                 __islessgreaterd((_x), (_y)))
#define isunordered(_x, _y)      ((isnan(_x) || isnan(_y)) ? 1 : 0)
#endif  /* C99 */

double __far acos(double x);
double __far asin(double x);
double __far atan(double x);
double __far atan2(double y, double x);
double __far cos(double x);
double __far sin(double x);
double __far tan(double x);
double __far cosh(double x);
double __far sinh(double x);
double __far tanh(double x);
double __far exp(double x);
double __far frexp(double value, int *exp);
double __far ldexp(double x, int exp);
double __far log(double x);
double __far log10(double x);
double __far modf(double value, double *iptr);
double __far pow(double x, double y);
double __far sqrt(double x);
double __far ceil(double x);
double __far fabs(double x);
double __far floor(double x);
double __far fmod(double x, double y);
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
double __far acosh(double x);
double __far asinh(double x);
double __far atanh(double x);
double __far log1p(double x);
double __far scalbn(double x, int n);
double __far scalbln(double x, long int n);
double __far nearbyint(double x);
double __far rint(double x);
long int __far lrint(double x);
long long int __far llrint(double x);
double __far round(double x);
long int __far lround(double x);
long long int __far llround(double x);
double __far trunc(double x);
double __far copysign(double x, double y);
double __far nan(const char __far *tagp);
double __far fdim(double x, double y);
double __far fmax(double x, double y);
double __far fmin(double x, double y);
#endif  /* C99 */

float __far acosf(float x);
float __far asinf(float x);
float __far atanf(float x);
float __far atan2f(float y, float x);
float __far cosf(float x);
float __far sinf(float x);
float __far tanf(float x);
float __far coshf(float x);
float __far sinhf(float x);
float __far tanhf(float x);
float __far expf(float x);
float __far frexpf(float value, int *exp);
float __far ldexpf(float x, int exp);
float __far logf(float x);
float __far log10f(float x);
float __far modff(float value, float *iptr);
float __far powf(float x, float y);
float __far sqrtf(float x);
float __far ceilf(float x);
float __far fabsf(float x);
float __far floorf(float x);
float __far fmodf(float x, float y);
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
float __far acoshf(float x);
float __far asinhf(float x);
float __far atanhf(float x);
float __far log1pf(float x);
float __far scalbnf(float x, int n);
float __far scalblnf(float x, long int n);
float __far nearbyintf(float x);
float __far rintf(float x);
long int __far lrintf(float x);
long long int __far llrintf(float x);
float __far roundf(float x);
long int __far lroundf(float x);
long long int __far llroundf(float x);
float __far truncf(float x);
float __far copysignf(float x, float y);
float __far nanf(const char __far *tagp);
float __far fdimf(float x, float y);
float __far fmaxf(float x, float y);
float __far fminf(float x, float y);
#endif  /* C99 */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
long double __far acosl(long double x);
long double __far asinl(long double x);
long double __far atanl(long double x);
long double __far atan2l(long double y, long double x);
long double __far cosl(long double x);
long double __far sinl(long double x);
long double __far tanl(long double x);
long double __far acoshl(long double x);
long double __far asinhl(long double x);
long double __far atanhl(long double x);
long double __far coshl(long double x);
long double __far sinhl(long double x);
long double __far tanhl(long double x);
long double __far expl(long double x);
long double __far frexpl(long double value, int *exp);
long double __far ldexpl(long double x, int exp);
long double __far logl(long double x);
long double __far log10l(long double x);
long double __far log1pl(long double x);
long double __far modfl(long double value, long double *iptr);
long double __far scalbnl(long double x, int n);
long double __far scalblnl(long double x, long int n);
long double __far fabsl(long double x);
long double __far powl(long double x, long double y);
long double __far sqrtl(long double x);
long double __far ceill(long double x);
long double __far floorl(long double x);
long double __far nearbyintl(long double x);
long double __far rintl(long double x);
long int __far lrintl(long double x);
long long int __far llrintl(long double x);
long double __far roundl(long double x);
long int __far lroundl(long double x);
long long int __far llroundl(long double x);
long double __far truncl(long double x);
long double __far fmodl(long double x, long double y);
long double __far copysignl(long double x, long double y);
long double __far nanl(const char __far *tagp);
long double __far fdiml(long double x, long double y);
long double __far fmaxl(long double x, long double y);
long double __far fminl(long double x, long double y);
#endif  /* C99 */

#if defined(__CNV_IAR__)
#define fabsl(_x)            fabs((_x))
#define acosl(_x)            acos((_x))
#define asinl(_x)            asin((_x))
#define atanl(_x)            atan((_x))
#define atan2l(_y, _x)       atan2((_y), (_x))
#define ceill(_x)            ceil((_x))
#define cosl(_x)             cos((_x))
#define coshl(_x)            cosh((_x))
#define expl(_x)             exp((_x))
#define floorl(_x)           floor((_x))
#define fmodl(_x, _y)        fmod((_x), (_y))
#define frexpl(_value, _exp) frexp((_value), (_exp))
#define ldexpl(_x, _exp)     ldexp((_x), (_exp))
#define logl(_x)             log((_x))
#define log10l(_x)           log10((_x))
#define modfl(_value, _iptr) modf((_value), (_iptr))
#define powl(_x, _y)         pow((_x), (_y))
#define sinl(_x)             sin((_x))
#define sinhl(_x)            sinh((_x))
#define sqrtl(_x)            sqrt((_x))
#define tanl(_x)             tan((_x))
#define tanhl(_x)            tanh((_x))
#endif  /* __CNV_IAR__ */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define acosl(_x)            acos((_x))
#define asinl(_x)            asin((_x))
#define atanl(_x)            atan((_x))
#define atan2l(_y, _x)       atan2((_y), (_x))
#define cosl(_x)             cos((_x))
#define sinl(_x)             sin((_x))
#define tanl(_x)             tan((_x))
#define acoshl(_x)           acosh((_x))
#define asinhl(_x)           asinh((_x))
#define atanhl(_x)           atanh((_x))
#define coshl(_x)            cosh((_x))
#define sinhl(_x)            sinh((_x))
#define tanhl(_x)            tanh((_x))
#define expl(_x)             exp((_x))
#define frexpl(_value, _exp) frexp((_value), (_exp))
#define ldexpl(_x, _exp)     ldexp((_x), (_exp))
#define logl(_x)             log((_x))
#define log10l(_x)           log10((_x))
#define log1pl(_x)           log1p((_x))
#define modfl(_value, _iptr) modf((_value), (_iptr))
#define scalbnl(_x, _n)      scalbn((_x), (_n))
#define scalblnl(_x, _n)     scalbln((_x), (_n))
#define fabsl(_x)            fabs((_x))
#define powl(_x, _y)         pow((_x), (_y))
#define sqrtl(_x)            sqrt((_x))
#define ceill(_x)            ceil((_x))
#define floorl(_x)           floor((_x))
#define nearbyintl(_x)       nearbyint((_x))
#define rintl(_x)            rint((_x))
#define lrintl(_x)           lrint((_x))
#define llrintl(_x)          llrint((_x))
#define roundl(_x)           round((_x))
#define lroundl(_x)          lround((_x))
#define llroundl(_x)         llround((_x))
#define truncl(_x)           trunc((_x))
#define fmodl(_x, _y)        fmod((_x), (_y))
#define copysignl(_x, _y)    copysign((_x), (_y))
#define nanl(_tagp)          nan((_tagp))
#define fdiml(_x, _y)        fdim((_x), (_y))
#define fmaxl(_x, _y)        fmax((_x), (_y))
#define fminl(_x, _y)        fmin((_x), (_y))
#endif  /* C99 */

#if defined(__DBL4)
#define acos(_x)             acosf((_x))
#define asin(_x)             asinf((_x))
#define atan(_x)             atanf((_x))
#define atan2(_y, _x)        atan2f((_y), (_x))
#define cos(_x)              cosf((_x))
#define sin(_x)              sinf((_x))
#define tan(_x)              tanf((_x))
#define cosh(_x)             coshf((_x))
#define sinh(_x)             sinhf((_x))
#define tanh(_x)             tanhf((_x))
#define exp(_x)              expf((_x))
#define frexp(_value, _exp)  frexpf((_value), (_exp))
#define ldexp(_x, _exp)      ldexpf((_x), (_exp))
#define log(_x)              logf((_x))
#define log10(_x)            log10f((_x))
#define modf(_value, _iptr)  modff((_value), (_iptr))
#define pow(_x, _y)          powf((_x), (_y))
#define sqrt(_x)             sqrtf((_x))
#define ceil(_x)             ceilf((_x))
#define fabs(_x)             fabsf((_x))
#define floor(_x)            floorf((_x))
#define fmod(_x, _y)         fmodf((_x), (_y))
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
#define acosh(_x)            acoshf((_x))
#define asinh(_x)            asinhf((_x))
#define atanh(_x)            atanhf((_x))
#define log1p(_x)            log1pf((_x))
#define scalbn(_x, _n)       scalbnf((_x), (_n))
#define scalbln(_x, _n)      scalblnf((_x), (_n))
#define nearbyint(_x)        nearbyintf((_x))
#define rint(_x)             rintf((_x))
#define lrint(_x)            lrintf((_x))
#define llrint(_x)           llrintf((_x))
#define round(_x)            roundf((_x))
#define lround(_x)           lroundf((_x))
#define llround(_x)          llroundf((_x))
#define trunc(_x)            truncf((_x))
#define copysign(_x, _y)     copysignf((_x), (_y))
#define nan(_tagp)           nanf((_tagp))
#define fdim(_x, _y)         fdimf((_x), (_y))
#define fmax(_x, _y)         fmaxf((_x), (_y))
#define fmin(_x, _y)         fminf((_x), (_y))
#endif  /* C99 */
#endif  /* __DBL4 */

#endif  /* !_MATH_H */
