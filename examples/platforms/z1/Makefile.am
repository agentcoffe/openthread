include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

override CFLAGS                              := $(filter-out -Wconversion,$(CFLAGS))
override CXXFLAGS                            := $(filter-out -Wconversion,$(CXXFLAGS))

lib_LIBRARIES = libopenthread-z1.a

#BEARSSL_SRCDIR                            = $(top_srcdir)/third_party/bearSSL

#    -I$(BEARSSL_SRCDIR)/inc                 
libopenthread_z1_a_CPPFLAGS               = \
    -I$(top_srcdir)/include                 \
    -I$(top_srcdir)/examples/platforms      \
    -I$(top_srcdir)/examples/platforms/z1/crypto \
    -I$(top_srcdir)/src/core                \
    $(NULL)

PLATFORM_SOURCES                          = \
    alarm.c                                 \
    flash.c                                 \
    misc.c                                  \
    platform.c                              \
    radio.c                                 \
    random.c                                \
    uart.c                                  \
	crypto/aes_alt.c                        \
    $(NULL)

#SOURCES = $(PLATFORM_SOURCES)

noinst_HEADERS                            = \
    platform-z1.h                           \
    $(NULL)

libopenthread_z1_a_SOURCES                = \
    $(PLATFORM_SOURCES)                     \
    $(NULL)

Dash                                      = -
libopenthread_z1_a_LIBADD                                                                       = \
    $(shell find $(top_builddir)/examples/platforms/utils $(Dash)type f $(Dash)name "*.o")

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
