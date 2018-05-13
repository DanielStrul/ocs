#
# Project subdirectories, build directory...
#
SUBDIRS = common/. server/. client/.
export ROOTDIR = $(CURDIR)
export BUILDIR = $(ROOTDIR)/build


#
# Make flags
#
export MAKEFLAGS += --warn-undefined-variables

#
# Compiler, archiver and linker flags
#
export CC = g++
export CFLAGS = --std=c++11 -pthread -Wall -Wextra -pedantic -I$(ROOTDIR)/common
export LDFLAGS = -lboost_program_options -lboost_system
export AR = ar
export ARFLAGS = rcs

#
# Cygwin-specific flags
#
ifeq ($(OS),Windows_NT)
	export CC = x86_64-w64-mingw32-g++
	export LDFLAGS += -lws2_32
endif

#
# Debug build settings
#
export DBGDIR = $(BUILDIR)/debug
export DBGLIBDIR = $(DBGDIR)/lib
export DBGEXEDIR = $(DBGDIR)/bin
export DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
export RELDIR = $(BUILDIR)/release
export RELLIBDIR = $(RELDIR)/lib
export RELEXEDIR = $(RELDIR)/bin
export RELCFLAGS = -O3 -DNDEBUG

#
# gprof build settings
#
export GPRDIR = $(BUILDIR)/gprof
export GPRLIBDIR = $(GPRDIR)/lib
export GPREXEDIR = $(GPRDIR)/bin
export GPRCFLAGS = $(RELCFLAGS) -pg

#
# Recursive rules
#
.DEFAULT_GOAL = all
all debug release gprof clean remake:
	@for dir in $(SUBDIRS) ; do \
		$(MAKE) -C $$dir $@ ; \
	done
