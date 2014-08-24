#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2010-2010 W. David Ashley. All rights reserved.              */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/


AUTOMAKE_OPTIONS	= foreign dist-zip

MAINTAINERCLEANFILES 	= Makefile.in aclocal.m4 configure config.guess config.sub \
			depcomp install-sh ltmain.sh missing mkinstalldirs config.h.in \
			compile stamp-h.in cscope.files cscope.out $(distdir).tar.gz

SUBDIRS =               os2 include test

# get version details
include $(top_srcdir)/os2linux.ver

VERSION = $(OS2_MAJOR).$(OS2_MINOR).$(OS2_MOD_LVL)
#driver_name = orexx-$(OS2_MAJOR).$(OS2_MINOR).$(OS2_MOD_LVL).0-$(OS2_BLD_LVL)

# Please note that the version-info has nothing to do with the release version.
# You need to know exactly what you are doing in order to get this correct.
VERSION_INFO = -version-info ${OS2_CURRENT}:${OS2_REVISION}:${OS2_AGE}

catdir = $(prefix)/bin

COMMON_CPPFLAGS = -DOS2_VER=$(OS2_MAJOR) -DOS2_REL=$(OS2_MINOR) -DOS2_MOD=$(OS2_MOD_LVL) -DOS2_FIX=0 \
                  -DOS2_SYS_STR=\"@OS2_SYS_STR@\" \
                  -DOS2_CATDIR=\"$(catdir)\" \
                  -DOS2_SHARED_LIBRARY_EXT=\"@OS2_SHARED_LIBRARY_EXT@\"











pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = os2linux.pc

