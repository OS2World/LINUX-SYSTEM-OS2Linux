/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test05.c                                                      */
/*                                                                            */
/* Description: Test thread, event semaphores and asyncronous timers.         */
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
/* DosSleep                                                                   */
/* DosCreateThread                                                            */
/* DosWaitThread                                                              */
/* DosCreateEventSem                                                          */
/* DosCloseEventSem                                                           */
/* DosWaitEventSem                                                            */
/* DosStartTimer                                                              */
/* DosStopTimer                                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the standard linux stuff first */
#include <errno.h>
#include <stdio.h>
#include <semaphore.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSDATETIME
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* thread1 - wait for event semaphore to post                                 */
/*----------------------------------------------------------------------------*/

void thread1(ULONG ulArg)
{
    int i;
    APIRET apiret;

    for (i = 0; i < 4; i++) {
        apiret = DosWaitEventSem((HEV)ulArg, SEM_INDEFINITE_WAIT);
        printf("DosWaitEventSem returned %d\n", (int)apiret);
    }
    return;
}


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    TID tid;
    HEV hev;
    HTIMER htimer;

    printf(__FILE__ " main function invoked\n");
    printf("Test timers\n");

    apiret = DosCreateEventSem("\\SEM32\\TEST5SEM", &hev, DC_SEM_SHARED, FALSE);
    printf("DosCreateEventSem function returned %d\n", (int)apiret);

    apiret = DosStartTimer(1000, (HSEM)hev, &htimer);
    printf("DosStartTimer function returned %d\n", (int)apiret);

    apiret = DosCreateThread(&tid, thread1, (ULONG)hev, 0, 8092);
    printf("DosCreateThread returned %d\n", (int)apiret);

    apiret = DosWaitThread(&tid, DCWW_WAIT);
    printf("Function DosWaitThread returned %d\n", (int)apiret);

    apiret = DosStopTimer(htimer);
    printf("DosStopTimer function returned %d\n", (int)apiret);

    apiret = DosCloseEventSem(hev);
    printf("Function DosCloseEventSem returned %d\n", (int)apiret);

    return 0;
}

