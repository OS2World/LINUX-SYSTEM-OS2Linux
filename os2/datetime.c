/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      datetime.c                                                    */
/*                                                                            */
/* Description: This file includes the code to support date, time and         */
/*              async timers.                                                 */
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
#define INCL_DOSDATETIME
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include "os2.h"

/* include the linux headers and our local stuff */
#include "os2linux.h"


/*============================================================================*/
/* OS/2 APIs for date and time                                                */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* DosGetDateTime                                                             */
/*    Notes:                                                                  */
/*       - the timezone is always returned as unknown                         */
/*         * Fixed by Fernando Aires, LTC-IBM Brazil
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosGetDateTime(PDATETIME pdt)
{
    SAVEENV;
    struct timeval now;
    struct tm brokentime;

    /* get the current time and convert it */
    gettimeofday(&now, NULL);
    localtime_r(&now.tv_sec, &brokentime);
    tzset(); /* Set TimeZone defined by /etc/localhost */

    /* now set the return values */
    pdt->hours = (UCHAR)brokentime.tm_hour;
    pdt->minutes = (UCHAR)brokentime.tm_min;
    pdt->seconds = (UCHAR)brokentime.tm_sec;
    pdt->hundredths = now.tv_usec * 100;
    pdt->day = (UCHAR)brokentime.tm_mday;
    pdt->month = (UCHAR)brokentime.tm_mon + 1;
    pdt->year = (USHORT)brokentime.tm_year + 1900;
    pdt->timezone = timezone/(-60); /* fixed! */
    pdt->weekday = (UCHAR)brokentime.tm_wday;

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosSetDateTime                                                             */
/*    Notes:                                                                  */
/*       - this function is not supported by user mode Linux and always       */
/*         returns ERROR_ACCESS_DENIED if the process is not owned by the     */
/*         superuser (root)                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosSetDateTime(__const__ DATETIME *pdt)
{
    SAVEENV;
    int rc;
    struct tm brokentime;
    time_t now;

    /* set up the time */
    brokentime.tm_hour = (int)pdt->hours;
    brokentime.tm_min = (int)pdt->minutes;
    brokentime.tm_sec = (int)pdt->seconds;
    brokentime.tm_mday = (int)pdt->day;
    brokentime.tm_mon = (int)pdt->month - 1;
    brokentime.tm_year = (int)pdt->year - 1900;
    brokentime.tm_wday = (int)pdt->weekday;
    brokentime.tm_isdst = 0; /* fix this! */

    now = mktime(&brokentime);
    rc = stime(&now);
    if (rc == -1) {
        DosLogError("DosSetTime", "stime", errno);
        RESTOREENV_RETURN(ERROR_ACCESS_DENIED);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*============================================================================*/
/* OS/2 APIs for async timers                                                 */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* DosAsyncTimer                                                              */
/*    Notes:                                                                  */
/*       - !!!!! In order for this to work properly the DosRequestSysMutexSem */
/*         function needs to be paired with the DosReleaseSysMutex function   */
/*       - This code is the same as DosStartTimer except for one statement    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosAsyncTimer(ULONG msec, HSEM hsem, PHTIMER phtimer)
{
    SAVEENV;
    struct sigaction act;
    PDOSASYNCTIMERSTRUCT mytimer;
    int i;

    /* enter critical section */
    DosRequestSysMutexSem();

    /* find an empty slot for our timer */
    for (i = 0; i < TIMERSLOTS; i++) {
        if (DosLinuxStruct.timerslot[i] == NULL) {
            break;
        }
    }
    if (i > TIMERSLOTS) {
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* set up the handler */
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = DosLinuxStruct.asyncTimerHandler[i];
    if ((sigemptyset(&act.sa_mask) == -1) ||
     (sigaction(SIGALRM, &act, NULL) == -1)) {
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* now create the timer */
    mytimer = (PDOSASYNCTIMERSTRUCT)malloc(sizeof(DOSASYNCTIMERSTRUCT));
    if (mytimer == NULL) {
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }
    if (timer_create(CLOCK_REALTIME, NULL, &mytimer->timerid) == -1) {
        free(mytimer);
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }
    mytimer->value.it_interval.tv_sec = msec / 1000;
    mytimer->value.it_interval.tv_nsec = 0;
    mytimer->value.it_interval.tv_nsec = (msec - ((msec / 1000) * 1000)) * 100;
    mytimer->value.it_value = mytimer->value.it_interval;
    mytimer->hSem = (HEV)hsem;

    /* this is the only difference between DosAsyncTimer & DosStartTimer */
    mytimer->oneshot = TRUE;

    /* now start the timer */
    if (timer_settime(mytimer->timerid, 0, &(mytimer->value), NULL)) {
        free(mytimer);
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* now save everything back to the caller */
    DosLinuxStruct.timerslot[i] = mytimer;
    *phtimer = (HTIMER)mytimer;

    /* exit critical section */
    DosReleaseSysMutexSem();

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosStartTimer                                                              */
/*    Notes:                                                                  */
/*       - !!!!! In order for this to work properly the DosRequestSysMutexSem */
/*         function needs to be paired with the DosReleaseSysMutex function   */
/*       - This code is the same as DosAsyncTimer except for one statement    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosStartTimer(ULONG msec, HSEM hsem, PHTIMER phtimer)
{
    SAVEENV;
    struct sigaction act;
    PDOSASYNCTIMERSTRUCT mytimer;
    int i;

    /* enter critical section */
    DosRequestSysMutexSem();

    /* find an empty slot for our timer */
    for (i = 0; i < TIMERSLOTS; i++) {
        if (DosLinuxStruct.timerslot[i] == NULL) {
            break;
        }
    }
    if (i > TIMERSLOTS) {
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* set up the handler */
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = DosLinuxStruct.asyncTimerHandler[i];
    if ((sigemptyset(&act.sa_mask) == -1) ||
     (sigaction(SIGALRM, &act, NULL) == -1)) {
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* now create the timer */
    mytimer = (PDOSASYNCTIMERSTRUCT)malloc(sizeof(DOSASYNCTIMERSTRUCT));
    if (mytimer == NULL) {
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }
    if (timer_create(CLOCK_REALTIME, NULL, &mytimer->timerid) == -1) {
        free(mytimer);
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }
    mytimer->value.it_interval.tv_sec = msec / 1000;
    mytimer->value.it_interval.tv_nsec = 0;
    mytimer->value.it_interval.tv_nsec = (msec - ((msec / 1000) * 1000)) * 100;
    mytimer->value.it_value = mytimer->value.it_interval;
    mytimer->hSem = (HEV)hsem;

    /* this is the only difference between DosAsyncTimer & DosStartTimer */
    mytimer->oneshot = FALSE;

    /* now start the timer */
    if (timer_settime(mytimer->timerid, 0, &(mytimer->value), NULL)) {
        free(mytimer);
        DosReleaseSysMutexSem();
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* now save everything back to the caller */
    DosLinuxStruct.timerslot[i] = mytimer;
    *phtimer = (HTIMER)mytimer;

    /* exit critical section */
    DosReleaseSysMutexSem();

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosStopTimer                                                               */
/*    Notes:                                                                  */
/*       - none                                                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosStopTimer(HTIMER htimer)
{
    SAVEENV;
    int i;
    PDOSASYNCTIMERSTRUCT mytimer;

    mytimer = (PDOSASYNCTIMERSTRUCT)htimer;

    /* find and free the timerslot */
    for (i = 0; i < TIMERSLOTS; i++) {
        if (DosLinuxStruct.timerslot[i] == mytimer) {
            break;
        }
    }
    if (i < TIMERSLOTS) {
        DosLinuxStruct.timerslot[i] = NULL;
    }
    else {
        /* the timer has already been stopped and the struct has been freed */
        RESTOREENV_RETURN(NO_ERROR);
    }

    /* set up the values to stop the timer */
    mytimer->value.it_interval.tv_sec = 0;
    mytimer->value.it_interval.tv_nsec = 0;
    mytimer->value.it_value = mytimer->value.it_interval;

    /* now stop the timer */
    if (timer_settime(mytimer->timerid, 0, &(mytimer->value), NULL)) {
        RESTOREENV_RETURN(ERROR_TS_NOTIMER);
    }

    /* free this because DosStartTimer and DosAsyncTimer always return */
    /* new timer structures                                            */
    free(mytimer);

    RESTOREENV_RETURN(NO_ERROR);
}

