#
# Project files
#
SRCS = $(wildcard *.cpp)
HDRS = $(wildcard *.h)
OBJS = $(SRCS:.cpp=.o)
EXE  = server

#
# External dependencies
#
COMMONHDRS = $(wildcard $(ROOTDIR)/common/*.h)
COMMONLIB = libcommon.a

.PHONY: all debug release gprof clean remake

#
# Default build
#
.DEFAULT_GOAL = all
all: debug release

#
# Debug targets and dependencies
#
DBJOBJDIR = $(DBGDIR)/server
DBGOBJS   = $(addprefix $(DBJOBJDIR)/, $(OBJS))
DBGEXE    = $(DBGEXEDIR)/$(EXE)
DBGLIBS   = $(DBGLIBDIR)/$(COMMONLIB)

#
# Release targets and dependencies
#
RELOBJDIR = $(RELDIR)/server
RELOBJS   = $(addprefix $(RELOBJDIR)/, $(OBJS))
RELEXE    = $(RELEXEDIR)/$(EXE)
RELLIBS   = $(RELLIBDIR)/$(COMMONLIB)

#
# gprof targets and dependencies
#
GPROBJDIR = $(GPRDIR)/server
GPROBJS   = $(addprefix $(GPROBJDIR)/, $(OBJS))
GPREXE    = $(GPREXEDIR)/$(EXE)
GPRLIBS   = $(GPRLIBDIR)/$(COMMONLIB)

#
# Debug rules
#
debug: $(DBGDIR)/. $(DBJOBJDIR)/. $(DBGEXEDIR)/. $(DBGEXE)

$(DBGEXE): $(DBGOBJS) $(DBGLIBS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $@ $^ $(LDFLAGS)

$(DBJOBJDIR)/%.o: %.cpp $(HDRS) $(COMMONHDRS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -c -o $@ $<

#
# Release rules
#
release: $(RELDIR)/. $(RELOBJDIR)/. $(RELEXEDIR)/. $(RELEXE)

$(RELEXE): $(RELOBJS)  $(RELLIBS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $@ $^ $(LDFLAGS)

$(RELOBJDIR)/%.o: %.cpp $(HDRS) $(COMMONHDRS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -c -o $@ $<

#
# Gprof rules
#
gprof: $(GPRDIR)/. $(GPROBJDIR)/. $(GPREXEDIR)/. $(GPREXE)

$(GPREXE): $(GPROBJS)  $(GPRLIBS)
	$(CC) $(CFLAGS) $(GPRCFLAGS) -o $@ $^ $(LDFLAGS)

$(GPROBJDIR)/%.o: %.cpp $(HDRS) $(COMMONHDRS)
	$(CC) $(CFLAGS) $(GPRCFLAGS) -c -o $@ $<

#
# Other/common rules
#
remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS) $(GPREXE) $(GPROBJS)

%/.:
	mkdir -p $@
