#
# Project files
#
SRCS = $(wildcard *.cpp)
HDRS = $(wildcard *.h)
OBJS = $(SRCS:.cpp=.o)
LIB  = libcommon.a

.PHONY: all debug release gprof clean remake

#
# Default build
#
.DEFAULT_GOAL = all
all: debug release

#
# Debug targets
#
DBJOBJDIR = $(DBGDIR)/common
DBGOBJS   = $(addprefix $(DBJOBJDIR)/, $(OBJS))
DBGLIB    = $(DBGLIBDIR)/$(LIB)

#
# Release targets
#
RELOBJDIR = $(RELDIR)/common
RELOBJS   = $(addprefix $(RELOBJDIR)/, $(OBJS))
RELLIB    = $(RELLIBDIR)/$(LIB)

#
# gprof targets
#
GPROBJDIR = $(GPRDIR)/common
GPROBJS   = $(addprefix $(GPROBJDIR)/, $(OBJS))
GPRLIB    = $(GPRLIBDIR)/$(LIB)

#
# Debug rules
#
debug: $(DBGDIR)/. $(DBJOBJDIR)/. $(DBGLIBDIR)/. $(DBGLIB)

$(DBGLIB): $(DBGOBJS)
	$(AR) $(ARFLAGS) $@ $^

$(DBJOBJDIR)/%.o: %.cpp $(HDRS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -c -o $@ $<

#
# Release rules
#
release: $(RELDIR)/. $(RELOBJDIR)/. $(RELLIBDIR)/. $(RELLIB)

$(RELLIB): $(RELOBJS)
	$(AR) $(ARFLAGS) $@ $^

$(RELOBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(RELCFLAGS) -c -o $@ $<

#
# gprof rules
#
gprof: $(GPRDIR)/. $(GPROBJDIR)/. $(GPRLIBDIR)/. $(GPRLIB)

$(GPRLIB): $(GPROBJS)
	$(AR) $(ARFLAGS) $@ $^

$(GPROBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(GPRCFLAGS) -c -o $@ $<

#
# Other/common rules
#
remake: clean all

clean:
	rm -f $(RELLIB) $(RELOBJS) $(DBGLIB) $(DBGOBJS) $(GPRLIB) $(GPROBJS)

%/.:
	mkdir -p $@
