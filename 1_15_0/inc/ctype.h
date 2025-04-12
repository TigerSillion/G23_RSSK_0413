/*
 * Copyright (C) 2014, 2018 Renesas Electronics Corporation
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

#ifndef _CTYPE_H
#define _CTYPE_H

int __far isalnum(int c);
int __far isalpha(int c);
int __far isascii(int c);
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)  /* C99 */
int __far isblank(int c);
#endif  /* C99 */
int __far iscntrl(int c);
int __far isdigit(int c);
int __far isgraph(int c);
int __far islower(int c);
int __far isprint(int c);
int __far ispunct(int c);
int __far isspace(int c);
int __far isupper(int c);
int __far isxdigit(int c);
int __far toascii(int c);
int __far tolower(int c);
int __far toupper(int c);

#endif  /* !_CTYPE_H */
