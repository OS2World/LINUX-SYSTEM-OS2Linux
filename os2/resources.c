/*-- C -----------------------------------------------------------------------*/
/*                                                                            */
/* Module:      resources.c                                                   */
/*                                                                            */
/* Description: This file includes the code to resources.                     */
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
#define INCL_DOSRESOURCES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#include "os2.h"

/* include the linux headers and our local stuff */
#include "os2linux.h"


/*============================================================================*/
/* OS/2 APIs for resources                                                    */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* DosGetResource                                                             */
/*    Notes:                                                                  */
/*       - this function is not supported by user mode Linux and always       */
/*         returns ERROR_ACCESS_DENIED                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosGetResource(HMODULE hmod, ULONG idType, ULONG idName,
                               PPVOID ppb)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosFreeResource                                                            */
/*    Notes:                                                                  */
/*       - this function is not supported by user mode Linux and always       */
/*         returns ERROR_ACCESS_DENIED                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosFreeResource(PVOID pb)
{
    return ERROR_ACCESS_DENIED;
}


/*----------------------------------------------------------------------------*/
/* DosQueryResourceSize                                                       */
/*    Notes:                                                                  */
/*       - this function is not supported by user mode Linux and always       */
/*         returns ERROR_ACCESS_DENIED                                        */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY DosQueryResourceSize(HMODULE hmod, ULONG idt, ULONG idn,
                                     PULONG pulsize)
{
    return ERROR_ACCESS_DENIED;
}


