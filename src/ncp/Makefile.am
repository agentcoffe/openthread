#
#  Copyright (c) 2016, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

EXTRA_DIST                                        = \
    PROTOCOL.md                                     \
    $(NULL)

# Pull in the sources that comprise the OpenThread NCP library.

lib_LIBRARIES                                     = \
    libopenthread-ncp-mtd.a                         \
    libopenthread-ncp-ftd.a                         \
    $(NULL)


COMMON_CPPFLAGS                                   = \
    -I$(top_srcdir)/include                         \
    -I$(top_srcdir)/src                             \
    -I$(top_srcdir)/src/core                        \
    -I$(top_srcdir)/third_party                     \
    -D_GNU_SOURCE                                   \
    -DSPINEL_PLATFORM_HEADER=\"spinel_platform.h\"  \
    $(OPENTHREAD_TARGET_DEFINES)                    \
    $(NULL)

libopenthread_ncp_mtd_a_CPPFLAGS                  = \
    -DOPENTHREAD_MTD=1                              \
    $(COMMON_CPPFLAGS)                              \
    $(NULL)

libopenthread_ncp_ftd_a_CPPFLAGS                  = \
    -DOPENTHREAD_FTD=1                              \
    $(COMMON_CPPFLAGS)                              \
    $(NULL)

COMMON_SOURCES                                    = \
    ncp_base.cpp                                    \
    ncp_base.hpp                                    \
    ncp_buffer.cpp                                  \
    ncp_buffer.hpp                                  \
    spinel.cpp                                      \
    spinel.hpp                                      \
    spinel_platform.h                               \
    ncp_spi.cpp                                     \
    ncp_spi.hpp                                     \
    hdlc.cpp                                        \
    hdlc.hpp                                        \
    ncp_uart.cpp                                    \
    ncp_uart.hpp                                    \
    $(NULL)

libopenthread_ncp_mtd_a_SOURCES                   = \
   $(COMMON_SOURCES)                                \
   $(NULL)

libopenthread_ncp_ftd_a_SOURCES                   = \
   $(COMMON_SOURCES)                                \
   $(NULL)

include_HEADERS                                   = \
    $(NULL)

if OPENTHREAD_BUILD_TESTS

noinst_PROGRAMS                                   = spinel-test
spinel_test_SOURCES                               = spinel.cpp
spinel_test_CFLAGS                                = \
    $(COMMON_CPPFLAGS)                              \
    -DSPINEL_SELF_TEST=1                            \
    -D_GNU_SOURCE                                   \
    -I$(top_srcdir)/src/core                        \
    -I$(top_srcdir)/include                         \
    $(NULL)

TESTS                                             = spinel-test

install-headers: install-includeHEADERS

if OPENTHREAD_BUILD_COVERAGE
CLEANFILES                                        = $(wildcard *.gcda *.gcno)
endif # OPENTHREAD_BUILD_COVERAGE
endif # OPENTHREAD_BUILD_TESTS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
