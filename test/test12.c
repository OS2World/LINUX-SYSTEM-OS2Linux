/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test12.c                                                      */
/*                                                                            */
/* Description: Test Vio functions.                                           */
/*                                                                            */
/* Copyright (C) W. David Ashley 2004-2010. All Rights Reserved.              */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* The following OS/2 APIs are tested here:                                   */
/*                                                                            */
/* VioGetCurPos                                                               */
/* VioSetCurPos                                                               */
/* DosSleep                                                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/



/* include the standard linux stuff first */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSPROCESS
#define INCL_VIO
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET16 apiret;
    char *str1 = "First string";
    char *str2 = "Second string";

    printf(__FILE__ " main function invoked\n");
    printf("Test VIO functions\n");
    DosSleep(2000);

    /* move the cursor around some */
    apiret = VioSetCurPos(0, 0, 0);
    DosSleep(1000);
    apiret = VioSetCurPos(1, 0, 0);
    DosSleep(1000);
    apiret = VioSetCurPos(2, 0, 0);
    DosSleep(1000);
    apiret = VioSetCurPos(3, 0, 0);
    DosSleep(1000);
    apiret = VioSetCurPos(4, 0, 0);
    DosSleep(1000);
    apiret = VioSetCurPos(5, 0, 0);
    DosSleep(1000);

    /* write some strings */
    VioWrtTTY(str1, strlen(str1), 0);
    DosSleep(2000);
    apiret = VioSetCurPos(6, 0, 0);
    VioWrtTTY(str2, strlen(str2), 0);
    DosSleep(2000);

    return 0;
}

