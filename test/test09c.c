/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test09c.c                                                     */
/*                                                                            */
/* Description: Test file manager functions.                                  */
/*                                                                            */
/* Copyright (C) IBM Corporation 2003. All Rights Reserved.                   */
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
/* DosFindFirst                                                               */
/* DosFindNext                                                                */
/* DosFindClose                                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the standard linux stuff first */
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_ERRORS
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    PSZ filespec = "./*.c";
    HDIR hdir = HDIR_CREATE;
    ULONG entries = 3;
    FILEFINDBUF3 findbuf[3];
    int i;

    printf(__FILE__ " main function invoked\n");
    printf("Test file manager functions\n");

    apiret = DosFindFirst(filespec, &hdir, 0, findbuf, sizeof(findbuf),
                          &entries, FIL_STANDARD);
    printf("DosFindFirst function returned %d\n", (int)apiret);
    printf("%d entries returned\n", (int)entries);
    for (i = 0; i < entries; i ++) {
        printf("%s\n", findbuf[i].achName);
    }

    while (apiret == NO_ERROR) {
        apiret = DosFindNext(hdir, findbuf, sizeof(findbuf), &entries);
        printf("DosFindNext function returned %d\n", (int)apiret);
        printf("%d entries returned\n", (int)entries);
        for (i = 0; i < entries; i ++) {
            printf("%s\n", findbuf[i].achName);
        }
    }
    printf("The last DosFindNext function should return 18\n");

    apiret = DosFindClose(hdir);
    printf("DosFindClose function returned %d\n", (int)apiret);

    return 0;
}

