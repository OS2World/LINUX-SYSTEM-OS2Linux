/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      os2.c                                                         */
/*                                                                            */
/* Description: Main source file for the OS.2 Linux module. This file         */
/*              includes the module initialization and cleanup functions.     */
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
/*----------------------------------------------------------------------------*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif  /* #ifdef HAVE_CONFIG_H

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_ERRORS
#include "os2.h"

/* include the linux headers and our local stuff */
#include "os2linux.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/

/* this is as good a place as any to put this */
DOSLINUXSTRUCT DosLinuxStruct;


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/

#define SYSMUTEXSEMNAME "/OS2LINUX_MUTEXSEM_SYSLIB"

int _init(void) __attribute__((constructor));
int _fini(void) __attribute__((destructor));


/*----------------------------------------------------------------------------*/
/* _init - Module initialization routine                                      */
/*----------------------------------------------------------------------------*/

#pragma init (my_so_init)

int my_so_init(void)
{
    int rc;

    /* initialize the DosLinuxStructure to zero */
    memset(&DosLinuxStruct, 0, sizeof(DOSLINUXSTRUCT));

    /* OS/2 processes support */
    DosLinuxStruct.pib.pib_ulpid = getpid();
    DosLinuxStruct.pib.pib_ulppid = getppid();
    DosLinuxStruct.pib.pib_hmte = 0;
    DosLinuxStruct.pib.pib_pchcmd = NULL;
    DosLinuxStruct.pib.pib_pchenv = *__environ;
    DosLinuxStruct.pib.pib_flstatus = 0;
    DosLinuxStruct.pib.pib_ultype = 2; /* ML_WINDOW_PROC_TYPE */

    /* thread specific memory support */
    rc = pthread_key_create(&DosLinuxStruct.thrdKey, NULL);
    if (rc) {
        syslog(LOG_USER | LOG_ERR,
               "OS2Linux: Error creating thread specific memory key.\n");
        exit(12);
    }

    /* thread specific TIB support */
    rc = pthread_key_create(&DosLinuxStruct.thrdTIBKey, NULL);
    if (rc) {
        syslog(LOG_USER | LOG_ERR,
               "OS2Linux: Error creating thread specific TIB key.\n");
        exit(12);
    }

    /* OS/2 process critical section mutex semaphore */
    rc = pthread_mutex_init(&DosLinuxStruct.critSecMutex, NULL );
    if (rc) {
        syslog(LOG_USER | LOG_ERR,
               "OS2Linux: Error creating thread critical section mutex semaphore.\n"
               "OS2Linux: The errno was %d.\n", errno);
        exit(12);
    }

    /* system-wide mutex semaphore for this library */
    /* do NOT use the OS/2 APIs here! */
    DosLinuxStruct.SysMutexSem = sem_open(SYSMUTEXSEMNAME, (O_CREAT | O_EXCL),
                                          (S_IRWXU | S_IRWXG), 0);
    if (DosLinuxStruct.SysMutexSem == SEM_FAILED ) {
        if (errno == EEXIST) {
            DosLinuxStruct.SysMutexSem = sem_open(SYSMUTEXSEMNAME, 0);;
            if (DosLinuxStruct.SysMutexSem == SEM_FAILED ) {
                syslog(LOG_USER | LOG_ERR,
                       "OS2Linux: Error opening the system-wide mutex semaphore.\n"
                       "OS2Linux: The errno was %d.\n", errno);
                exit(12);
            }
        }
        else {
            syslog(LOG_USER | LOG_ERR,
                   "OS2Linux: Error creating the system-wide mutex semaphore.\n"
                   "OS2Linux: The errno was %d.\n", errno);
            exit(12);
        }
    }
    sem_post(DosLinuxStruct.SysMutexSem);

    /* termination code to use for this program */
    DosLinuxStruct.termcode = TC_EXIT; /* fix this! */

    /* support for an array of OS/2 asyncronous timers */
    DosLinuxStruct.asyncTimerHandler[0] = DosAsyncTimerHandler_0;
    DosLinuxStruct.asyncTimerHandler[1] = DosAsyncTimerHandler_1;
    DosLinuxStruct.asyncTimerHandler[2] = DosAsyncTimerHandler_2;
    DosLinuxStruct.asyncTimerHandler[3] = DosAsyncTimerHandler_3;
    DosLinuxStruct.asyncTimerHandler[4] = DosAsyncTimerHandler_4;
    DosLinuxStruct.asyncTimerHandler[5] = DosAsyncTimerHandler_5;
    DosLinuxStruct.asyncTimerHandler[6] = DosAsyncTimerHandler_6;
    DosLinuxStruct.asyncTimerHandler[7] = DosAsyncTimerHandler_7;
    DosLinuxStruct.asyncTimerHandler[8] = DosAsyncTimerHandler_8;
    DosLinuxStruct.asyncTimerHandler[9] = DosAsyncTimerHandler_9;

    /* establish the program errno for the client program */
    errno = 0;

    return 0;
}


/*----------------------------------------------------------------------------*/
/* _fini - Module finialization routine                                       */
/*----------------------------------------------------------------------------*/

#pragma fini (my_so_fini)

int my_so_fini(void)
{
    int i, j;

    /* process the DosExitList of functions in LIFO order */
    for (i = 255; i >= 0; i--) { /* by ordercode */
        for (j = MAX_EXIT_LIST - 1; j >= 0; j--) { /* by list entry */
            if (DosLinuxStruct.exitList[j].ordercode == i &&
             DosLinuxStruct.exitList[j].func != NULL) {
                /* execute the function */
                (DosLinuxStruct.exitList[j].func)(DosLinuxStruct.termcode);
            }
        }
    }
    VioDeInitCurses();

    /* DO THIS LAST! */
    sem_close(DosLinuxStruct.SysMutexSem);

    return 0;
}

