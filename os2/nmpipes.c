/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      nmpipes.c                                                     */
/*                                                                            */
/* Description: This file includes the code to support named pipes.           */
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
#define INCL_DOSNMPIPES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include "os2.h"

/* include the linux headers and our local stuff */
#include "os2linux.h"


/*============================================================================*/
/* OS/2 APIs for named pipes                                                  */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* DosCreateNPipe                                                             */
/*    Notes:                                                                  */
/*       - cbInbuf and cbOutbuf are ignored                                   */
/*       - openmode is ignored                                                */
/*       - pipemode is ignored for function but saved for DosQueryFHState     */
/*       - msec is ignored                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosCreateNPipe(PCSZ pszName, PHPIPE pHpipe, ULONG openmode,
                               ULONG pipemode, ULONG cbInbuf, ULONG cbOutbuf,
                               ULONG msec)
{
    SAVEENV;
    PFILEHANDLESTRUCT pfh;
    PSZ pszNewName;
    PSZ pszTmpName;
    int rc;
    static char tmpdir[] = "/tmp";

    /* convert the pipe name to Linux format so we can find it */
    pszTmpName = alloca(strlen(pszName) + 1);
    if (pszTmpName == NULL) {
        RESTOREENV_RETURN(ERROR_OPEN_FAILED);
    }
    pszNewName = alloca(strlen(pszName) + strlen(tmpdir) + 1);
    if (pszTmpName == NULL) {
        RESTOREENV_RETURN(ERROR_OPEN_FAILED);
    }
    strcpy(pszTmpName, pszName);
    DosNameConversion(pszTmpName, "\\", ".", TRUE);
    if (*pszTmpName == '.') {
        *pszTmpName = '/';
    }
    strcpy(pszNewName, tmpdir);
    strcat(pszNewName, pszTmpName);

    /* try to create the Linux FIFO */
    rc = mkfifo(pszNewName, (S_IRWXG | S_IRWXU | S_IROTH));
    if (rc == -1) {
        switch (errno) {
        case ENOSPC:
        case EROFS:
            RESTOREENV_RETURN(ERROR_OUT_OF_STRUCTURES);
        case EACCES:
            RESTOREENV_RETURN(ERROR_ACCESS_DENIED);
        case ENAMETOOLONG:
            RESTOREENV_RETURN(ERROR_FILENAME_EXCED_RANGE);
        case ENOENT:
        case ENOTDIR:
            RESTOREENV_RETURN(ERROR_INVALID_PARAMETER);
        default:
            RESTOREENV_RETURN(ERROR_PIPE_BUSY);
        }
    }

    /* allocate memory for the file handle and set fields */
    pfh = malloc(sizeof(FILEHANDLESTRUCT));
    if (pfh == NULL) {
        RESTOREENV_RETURN(ERROR_NOT_ENOUGH_MEMORY);
    }
    strcpy(pfh->id, "PIP");
    pfh->openmode = openmode;
    pfh->npname = malloc(strlen(pszNewName) + 1);
    if (pfh->npname == NULL) {
        RESTOREENV_RETURN(ERROR_NOT_ENOUGH_MEMORY);
    }
    strcpy(pfh->npname, pszNewName);

    pfh->fh = open(pszNewName, O_TRUNC | O_RDWR);
    if (pfh->fh == -1) {
        switch (errno) {
        case EACCES:
        case EROFS:
        case ETXTBSY:
        case ENODEV:
        case ENXIO:
            free(pfh);
            RESTOREENV_RETURN(ERROR_ACCESS_DENIED);
        case EFAULT:
        case EINVAL:
            free(pfh);
            RESTOREENV_RETURN(ERROR_INVALID_ACCESS);
        case ENAMETOOLONG:
            free(pfh);
            RESTOREENV_RETURN(ERROR_FILENAME_EXCED_RANGE);
        case ENOSPC:
            free(pfh);
            RESTOREENV_RETURN(ERROR_DISK_FULL);
        case EMFILE:
        case ENFILE:
            free(pfh);
            RESTOREENV_RETURN(ERROR_TOO_MANY_OPEN_FILES);
        case EEXIST:
        default:
            free(pfh);
            RESTOREENV_RETURN(ERROR_OPEN_FAILED);
        }
    }

    *pHpipe = (HPIPE)pfh;
    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosConnectNPipe                                                            */
/*    Notes:                                                                  */
/*       - no need to do anything here except check for valid pipe handle     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosConnectNPipe(HPIPE hpipe)
{
    SAVEENV;
    PFILEHANDLESTRUCT pfh;

    pfh = (PFILEHANDLESTRUCT)hpipe;
    if (strcmp(pfh->id, "PIP") != 0) {
        RESTOREENV_RETURN(ERROR_INVALID_HANDLE);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosDisconnectNPipe                                                         */
/*    Notes:                                                                  */
/*       - no need to do anything here except check for valid pipe handle     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosDisConnectNPipe(HPIPE hpipe)
{
    SAVEENV;
    PFILEHANDLESTRUCT pfh;

    pfh = (PFILEHANDLESTRUCT)hpipe;
    if (strcmp(pfh->id, "PIP") != 0) {
        RESTOREENV_RETURN(ERROR_INVALID_HANDLE);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosTransactNPipe                                                           */
/*    Notes:                                                                  */
/*       - none                                                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosTransactNPipe(HPIPE hpipe, PVOID pOutbuf, ULONG cbOut,
                                 PVOID pInbuf, ULONG cbIn, PULONG pcbRead)
{
    SAVEENV;
    APIRET apiret;
    ULONG actual;

    apiret = DosWrite((HFILE)hpipe, pOutbuf, cbOut, &actual);
    if (apiret) {
        RESTOREENV_RETURN(apiret);
    }
    apiret = DosRead((HFILE)hpipe, pInbuf, cbIn, &actual);
    if (apiret) {
        RESTOREENV_RETURN(apiret);
    }
    *pcbRead = actual;

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosCallNPipe                                                               */
/*    Notes:                                                                  */
/*       - msec is ignored                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosCallNPipe(PCSZ pszName, PVOID pInbuf, ULONG cbIn,
                             PVOID pOutbuf, ULONG cbOut, PULONG pcbActual,
                             ULONG msec)
{
    SAVEENV;
    APIRET apiret;
    HFILE hFile;
    ULONG action;

    apiret = DosOpen(pszName, &hFile, &action, 0, FILE_NORMAL,
                     OPEN_ACCESS_READWRITE, 0, NULL);
    if (apiret) {
        RESTOREENV_RETURN(apiret);
    }
    apiret = DosTransactNPipe((HPIPE) hFile, pOutbuf, cbOut, pInbuf, cbIn,
                              pcbActual);
    if (apiret) {
        RESTOREENV_RETURN(apiret);
    }
    apiret = DosClose(hFile);
    if (apiret) {
        RESTOREENV_RETURN(apiret);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosQueryNPHState                                                           */
/*    Notes:                                                                  */
/*       - this function returns mostly static values                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryNPHState(HPIPE hpipe, PULONG pState)
{
    SAVEENV;
    PFILEHANDLESTRUCT pfh;
    ULONG state = (NP_WAIT | NP_TYPE_MESSAGE | NP_READMODE_MESSAGE |
                  NP_UNLIMITED_INSTANCES);

    pfh = (PFILEHANDLESTRUCT)hpipe;
    if (strcmp(pfh->id, "PIP") != 0) {
        RESTOREENV_RETURN(ERROR_INVALID_HANDLE);
    }
    if (pfh->npname != NULL) {
        state |= NP_END_SERVER;
    }
    *pState = state;

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosWaitNPipe                                                               */
/*    Notes:                                                                  */
/*       - none                                                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosWaitNPipe(PCSZ pszName, ULONG msec)
{
    SAVEENV;
    PSZ pszTmpName;
    PSZ pszNewName;
    struct stat statbuf;
    int rc;
    static char tmpdir[] = "/tmp";

    /* convert the pipe name to Linux format so we can find it */
    pszTmpName = alloca(strlen(pszName) + 1);
    if (pszTmpName == NULL) {
        RESTOREENV_RETURN(ERROR_NOT_ENOUGH_MEMORY);
    }
    pszNewName = alloca(strlen(pszName) + strlen(tmpdir) + 1);
    if (pszTmpName == NULL) {
        RESTOREENV_RETURN(ERROR_NOT_ENOUGH_MEMORY);
    }
    strcpy(pszTmpName, pszName);
    DosNameConversion(pszTmpName, "\\", ".", TRUE);
    if (*pszTmpName == '.') {
        *pszTmpName = '/';
    }
    strcpy(pszNewName, tmpdir);
    strcat(pszNewName, pszTmpName);

    rc = stat(pszNewName, &statbuf);
    if (rc) {
        RESTOREENV_RETURN(ERROR_FILE_NOT_FOUND);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosSetNPipeSem                                                             */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosSetNPipeSem(HPIPE hpipe, HSEM hsem, ULONG key)
{
    /* if I could figure out how this worked under OS/2 I would implement it */
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosPeekNPipe                                                               */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosPeekNPipe(HPIPE hpipe, PVOID pBuf, ULONG cbBuf,
                             PULONG pcbActual, PAVAILDATA pAvail,
                             PULONG pState)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosQueryNPipeInfo                                                          */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryNPipeInfo(HPIPE hpipe, ULONG infolevel, PVOID pBuf,
                                  ULONG cbBuf)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosQueryNPipeSemState                                                      */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryNPipeSemState(HSEM hsem, PPIPESEMSTATE pnpss,
                                      ULONG cbBuf)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosRawReadNPipe                                                            */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosRawReadNPipe(PCSZ pszName, ULONG cb, PULONG pLen,
                                PVOID pBuf)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosRawWriteNPipe                                                           */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosRawWriteNPipe(PCSZ pszName, ULONG cb)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosSetNPHState                                                             */
/*    Notes:                                                                  */
/*       - this function is not supported and always returns                  */
/*         ERROR_ACCESS_DENIED                                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosSetNPHState(HPIPE hpipe, ULONG state)
{
    return ERROR_ACCESS_DENIED;
}



