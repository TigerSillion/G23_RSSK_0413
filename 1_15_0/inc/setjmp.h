/*
 * Copyright (C) 2014 Renesas Electronics Corporation
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

#ifndef _SETJMP_H
#define _SETJMP_H

typedef int __near jmp_buf[3];  /* SP, return-address */
int __far setjmp(jmp_buf env);
void __far longjmp(jmp_buf env, int val);

#endif  /* !_SETJMP_H */
