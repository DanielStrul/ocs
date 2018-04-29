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
export CFLAGS = --std=c++11 -Wall -Wextra -pedantic -I$(ROOTDIR)/common
export LDFLAGS =  
export AR = ar
export ARFLAGS = rcs

#
# Cygwin-specific flags
#
ifeq ($(OS),Windows_NT)
    export CC = x86_64-w64-mingw32-g++
	export LDFLAGS += -lboost_filesystem -lboost_program_options -lboost_system -lws2_32
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
# Recursive rules
#
.DEFAULT_GOAL = all
all debug release clean remake:
	@for dir in $(SUBDIRS) ; do \
		$(MAKE) -C $$dir $@ ; \
	done
