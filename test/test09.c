/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test09.c                                                      */
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
/* DosOpen                                                                    */
/* DosRead                                                                    */
/* DosClose                                                                   */
/* DosSleep                                                                   */
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
    HFILE hfile;
    ULONG action;
    char buf[32000];
    ULONG bytesread;
    ULONG newpos;

    printf(__FILE__ " main function invoked\n");
    printf("Test file manager functions\n");

    apiret = DosOpen("./test01.c", &hfile, &action, 0, 0, OPEN_ACTION_OPEN_IF_EXISTS,
                     OPEN_ACCESS_READONLY, NULL);
    printf("DosOpen function returned %d\n", (int)apiret);
    if (apiret != NO_ERROR) {
        DosSleep(3000);
        return 0;
    }
    printf("Action returned %d\n", (int)action);

    apiret = DosRead(hfile, buf, sizeof(buf) - 1, &bytesread);
    printf("DosRead function returned %d\n", (int)apiret);
    printf("bytesread = %d\n", (int)bytesread);
    buf[bytesread] = '\0';

    printf("\nFile contents: *****************************\n");
    printf("%s", buf);
    printf("End file contents: *****************************\n\n");

    DosSleep(1500);

    apiret = DosSetFilePtr(hfile, -200, FILE_CURRENT, &newpos);
    printf("DosSetFilePtr function returned %d\n", (int)apiret);

    apiret = DosRead(hfile, buf, sizeof(buf) - 1, &bytesread);
    printf("DosRead function returned %d\n", (int)apiret);
    printf("bytesread = %d\n", (int)bytesread);
    buf[bytesread] = '\0';

    printf("\nFile contents (last 200 bytes): *****************************\n");
    printf("%s", buf);
    printf("End file contents(last 200 bytes): *****************************\n\n");

    apiret = DosClose(hfile);
    printf("DosClose function returned %d\n", (int)apiret);

    return 0;
}

