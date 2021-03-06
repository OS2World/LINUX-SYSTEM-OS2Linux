/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test08.c                                                      */
/*                                                                            */
/* Description: Test some miscellaneous functions.                            */
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
/* DosPutMessage                                                              */
/* DosQuerySysInfo                                                            */
/* DosScanEnv                                                                 */
/* DosSetExtLIBPATH                                                           */
/* DosQueryExtLIBPATH                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the standard linux stuff first */
#include <errno.h>
#include <stdio.h>
#include <string.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSMISC
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    char * msg = "Message from DosPutMessage";
    ULONG val;
    PSZ env_val;
    CHAR newpath[128];

    printf(__FILE__ " main function invoked\n");
    printf("Test miscellaneous functions\n");

    apiret = DosPutMessage((HFILE)1, strlen(msg), msg);
    printf("DosPutMessage function returned %d\n", (int)apiret);

    apiret = DosQuerySysInfo(QSV_MAX_TEXT_SESSIONS, QSV_MAX_TEXT_SESSIONS, &val,
                             sizeof(val));
    printf("DosQuerySysInfo function returned %d\n", (int)apiret);
    printf("QSV_MAX_TEXT_SESSIONS = %d\n", (int)val);

    apiret = DosQuerySysInfo(QSV_MAX_PATH_LENGTH, QSV_MAX_PATH_LENGTH, &val,
                             sizeof(val));
    printf("DosQuerySysInfo function returned %d\n", (int)apiret);
    printf("QSV_MAX_PATH_LENGTH = %d\n", (int)val);

    apiret = DosScanEnv("LD_LIBRARY_PATH", &env_val);
    printf("DosScanEnv function returned %d\n", (int)apiret);
    printf("Env var LD_LIBRARY_PATH = %s\n", env_val);

    apiret = DosSetExtLIBPATH("./", 0);
    printf("DosSetExtLIBPATH function returned %d\n", (int)apiret);

    apiret = DosQueryExtLIBPATH(newpath, 0);
    printf("DosQueryExtLIBPATH function returned %d\n", (int)apiret);
    printf("The new LIBPATH is %s\n", newpath);

    return 0;
}

