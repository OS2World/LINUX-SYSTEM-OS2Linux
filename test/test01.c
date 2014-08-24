/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      test01.c                                                      */
/*                                                                            */
/* Description: Test the os2.so library. Only basic linking tests are         */
/*              performed.                                                    */
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
/* DosGetDateTime                                                             */
/* DosBeep                                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the standard linux stuff first */
#include <stdio.h>

/* now include the OS/2 stuff */
#define INCL_NOPMAPI
#define INCL_DOSPROCESS
#define INCL_DOSDATETIME
#include "os2.h"


/*----------------------------------------------------------------------------*/
/* displaytime - print out the date and timem                                 */
/*----------------------------------------------------------------------------*/

void displaytime(PDATETIME pdt)
{
    char * days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    printf("The current time is %s %02d-%02d-%d:%02d:%02d:%02d\n",
           days[pdt->weekday], (int)pdt->month, (int)pdt->day, (int)pdt->year,
           (int)pdt->hours, (int)pdt->minutes, (int)pdt->seconds);
    return;
}


/*----------------------------------------------------------------------------*/
/* main - the main entry point to the program                                 */
/*----------------------------------------------------------------------------*/

int main(void)
{
    APIRET apiret;
    DATETIME dt;
    ULONG sec = 5;

    printf(__FILE__ " main function invoked\n");
    printf("Test basic library functioality\n");

    printf("Three beeps should follow this message\n");
    DosBeep(0, 0);
    DosSleep(125);
    DosBeep(0, 0);
    DosSleep(125);
    DosBeep(0, 0);

    apiret = DosGetDateTime(&dt);
    displaytime(&dt);

    printf("Sleeping for %d seconds\n", (int)sec);
    DosSleep(sec * 1000);

    apiret = DosGetDateTime(&dt);
    displaytime(&dt);
    return 0;
}

