/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      modulemgr.c                                                   */
/*                                                                            */
/* Description: This file includes the code to support modules.               */
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
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include "os2.h"

/* include the linux headers and our local stuff */
#include "os2linux.h"


/*============================================================================*/
/* OS/2 APIs for modules                                                      */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* DosLoadModule                                                              */
/*    Notes:                                                                  */
/*       - only a limited number of error codes are returned                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosLoadModule(PSZ pszName, ULONG cbName, PCSZ pszModname,
                              PHMODULE phmod)
{
    SAVEENV;

    *phmod = (HMODULE)dlopen(pszModname, RTLD_NOW | RTLD_GLOBAL);
    if (*phmod == 0) {
        if (pszName != NULL) {
            strncpy(pszName, dlerror(), cbName - 1);
        }
        RESTOREENV_RETURN(ERROR_FILE_NOT_FOUND);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosFreeModule                                                              */
/*    Notes:                                                                  */
/*       - only a limited number of error codes are returned                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosFreeModule(HMODULE hmod)
{
    SAVEENV;
    int rc;

    rc = dlclose((void *)hmod);
    if (rc) {
        RESTOREENV_RETURN(ERROR_INVALID_HANDLE);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosQueryProcAddr                                                           */
/*    Notes:                                                                  */
/*       - ordinal is ignored                                                 */
/*       - pszName must be non-null                                           */
/*       - only a limited number of error codes are returned                  */
/*       - external symbols (not just procedure names) are returned           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryProcAddr(HMODULE hmod, ULONG ordinal, PCSZ pszName,
                                 PFN* ppfn)
{
    SAVEENV;
    char * rc;

    /* check param */
    if (pszName == NULL) {
        RESTOREENV_RETURN(ERROR_INVALID_PARAMETER);
    }

    *ppfn = dlsym((void *)hmod, pszName);
    rc = dlerror();
    if (rc) {
        RESTOREENV_RETURN(ERROR_INVALID_NAME);
    }

    RESTOREENV_RETURN(NO_ERROR);
}


/*----------------------------------------------------------------------------*/
/* DosQueryModuleHandle                                                       */
/*    Notes:                                                                  */
/*       - unsupported                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryModuleHandle(PCSZ pszModname, PHMODULE phmod)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosQueryModuleName                                                         */
/*    Notes:                                                                  */
/*       - unsupported                                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryModuleName(HMODULE hmod, ULONG cbName, PCHAR pch)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosQueryProcType                                                           */
/*    Notes:                                                                  */
/*       - see notes for DosQueryProcAddr function                            */
/*       - always claims a procedure is 32 bit                                */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryProcType(HMODULE hmod, ULONG ordinal, PCSZ pszName,
                                 PULONG pulproctype)
{
    SAVEENV;
    APIRET rc;
    PFN pfn;

    rc = DosQueryProcAddr(hmod, ordinal, pszName, &pfn);
    if (rc == NO_ERROR) {
        *pulproctype = PT_32BIT;
    }

    RESTOREENV_RETURN(rc);
}

