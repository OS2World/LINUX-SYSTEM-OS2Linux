/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test11a.c                                                     */
/*                                                                            */
/* Description: Test named pipe functions.                                    */
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
/* DosOpen                                                                    */
/* DosRead                                                                    */
/* DosClose                                                                   */
/* DosSleep                                                                   */
/* DosOpenEventSem                                                                           */
/* DosPostEventSem                                                                           */
/* DosCloseEventSem                                                                           */
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
#define INCL_DOSNMPIPES
#define INCL_ERRORS
#define INCL_DOSSEMAPHORES
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    HPIPE hpipe;
    PSZ pipeName = "\\pipe\\testpipe";
    PSZ semName = "\\sem32\\pipesem";
    HEV hev;
    ULONG action, byteswritten;
    PSZ msg[3] = {"Message number one", "Message number two",
                  "Message number three"};

    printf(__FILE__ " main function invoked\n");
    printf("Test named pipe functions\n");

    apiret = DosOpen(pipeName, (PHFILE)&hpipe, &action, 0, FILE_NORMAL,
                        OPEN_ACTION_FAIL_IF_NEW, 0, NULL);
    printf("DosOpen function returned %d\n", (int)apiret);
    if (apiret != NO_ERROR) {
        return 0;
    }

    apiret = DosOpenEventSem(semName, &hev);
    printf("DosOpenEventSem function returned %d\n", (int)apiret);

    apiret = DosWrite((HFILE)hpipe, msg[0], strlen(msg[0]), &byteswritten);
    printf("DosWrite function returned %d\n", (int)apiret);

    apiret = DosPostEventSem(hev);
    DosSleep(2000);

    apiret = DosWrite((HFILE)hpipe, msg[1], strlen(msg[1]), &byteswritten);
    printf("DosWrite function returned %d\n", (int)apiret);

    apiret = DosPostEventSem(hev);
    DosSleep(2000);

    apiret = DosWrite((HFILE)hpipe, msg[2], strlen(msg[2]), &byteswritten);
    printf("DosWrite function returned %d\n", (int)apiret);

    apiret = DosPostEventSem(hev);
    DosSleep(2000);

    apiret = DosCloseEventSem(hev);
    printf("DosCloseEventSem function returned %d\n", (int)apiret);

    apiret = DosClose((HFILE)hpipe);
    printf("DosClose function returned %d\n", (int)apiret);

    return 0;
}

