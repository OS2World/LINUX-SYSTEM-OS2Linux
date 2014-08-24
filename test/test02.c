/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test02.c                                                      */
/*                                                                            */
/* Description: Test thread starting and ending.                              */
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
/* _beginthread                                                               */
/* _endthread                                                                 */
/* DosSleep                                                                   */
/* DosCreateThread                                                            */
/* DosWaitThread                                                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the standard linux stuff first */
#include <stdio.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSPROCESS
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* thread1 - thread started by DosCreateThread                                */
/*----------------------------------------------------------------------------*/

void thread1(ULONG ulArg)
{
    DosSleep(1000); /* 1 second */
    printf("Message from function thread1\n");
    printf("The thread argument was %d\n", (int)ulArg);
    DosSleep(1000); /* 1 second */
    return;
}

/*----------------------------------------------------------------------------*/
/* thread2 - thread started by _beginthread                                   */
/*----------------------------------------------------------------------------*/

void thread2(void * p)
{
    DosSleep(1000); /* 1 second */
    printf("Message from function thread2\n");
    printf("The thread argument was %p\n", p);
    DosSleep(1000); /* 1 second */
    _endthread();
    return;
}


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    TID tid;

    /* test thread1 */
    printf(__FILE__ " main function invoked\n");
    printf("Test basic thread functionality\n");

    printf("Starting first thread with DosCreateThread\n");
    apiret = DosCreateThread(&tid, thread1, 10, 0, 8092);
    printf("Function returned %d\n", (int)apiret);
    printf("The thread id was %d\n", (int)tid);
    apiret = DosWaitThread(&tid, DCWW_WAIT);
    printf("Function DosWaitThread returned %d\n", (int)apiret);

    /* test thread2 */
    printf(__FILE__ " main function invoked\n");
    printf("Starting second thread with _beginthread\n");
    tid = (TID)_beginthread(thread2, 8092, (void *)10);
    printf("The thread id was %d\n", (int)tid);
    apiret = DosWaitThread(&tid, DCWW_WAIT);
    printf("Function DosWaitThread returned %d\n", (int)apiret);

    return 0;
}

