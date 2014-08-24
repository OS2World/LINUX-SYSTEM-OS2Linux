/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test10a.c                                                     */
/*                                                                            */
/* Description: Test DosExecPgm and queues.                                   */
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
/* DosExecPgm                                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the standard linux stuff first */
#include <errno.h>
#include <stdio.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSPROCESS
#define INCL_DOSQUEUES
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    HQUEUE hq;
    PSZ pszQueueName = "\\queue\\os2linux\\test\\queue";
    PID pid;

    printf(__FILE__ " main function invoked\n");
    printf("Test DosExecPgm and queues\n");
    ULONG bytesread, priority = 0;
    REQUESTDATA request;
    PVOID msgtxt;
    CHAR fmtspec[80];

    /* create queue */
    apiret = DosOpenQueue(&pid, &hq, pszQueueName);
    printf("DosOpenQueue function returned %d\n", (int)apiret);
    if (apiret) {
        return 0;
    }

    apiret = DosReadQueue(hq, &request, &bytesread, &msgtxt, 0, TRUE,
                             (PBYTE)&priority, NULLHANDLE);
    printf("DosReadQueue function returned %d\n", (int)apiret);
    if (apiret) {
        return 0;
    }
    sprintf(fmtspec, "The message was \"%%0.%ds\"\n", (int)bytesread);
    printf(fmtspec, (PSZ)msgtxt);

    apiret = DosReadQueue(hq, &request, &bytesread, &msgtxt, 0, TRUE,
                             (PBYTE)&priority, NULLHANDLE);
    printf("DosReadQueue function returned %d\n", (int)apiret);
    if (apiret) {
        return 0;
    }
    sprintf(fmtspec, "The message was \"%%0.%ds\"\n", (int)bytesread);
    printf(fmtspec, (PSZ)msgtxt);

    apiret = DosReadQueue(hq, &request, &bytesread, &msgtxt, 0, TRUE,
                             (PBYTE)&priority, NULLHANDLE);
    printf("DosReadQueue function returned %d\n", (int)apiret);
    if (apiret) {
        return 0;
    }
    sprintf(fmtspec, "The message was \"%%0.%ds\"\n", (int)bytesread);
    printf(fmtspec, (PSZ)msgtxt);

    apiret = DosCloseQueue(hq);
    printf("DosCloseQueue function returned %d\n", (int)apiret);

    return 0;
}

