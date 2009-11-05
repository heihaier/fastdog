# Toolchain Setup
PP=cpp
CC=gcc
LD=$(CC)
RM=rm -f

# Compilation Configurations
DEBUG=
CFLAGS=$(DEBUG)
LDFLAGS=-l pthread -l dl

# Objectives
TARGET=fastdog
OBJECT=src/fastdog.o \
	   src/daemon/daemon.o \
	   src/list/list.o \
	   src/memory/memory.o \
	   src/server/server.o \
	   src/server/tasklist/tasklist.o \
	   src/server/tasklist/task.o \
	   src/server/config/config.o \
	   src/server/connection/connection.o \
	   src/server/connection/listener.o \
	   src/server/connection/manager.o \
	   src/server/http/http.o \
	   src/server/website/website.o \
	   src/server/website/manager.o \
	   src/server/website/interface/interface.o

DEPEND=$(OBJECT:.o=.dep)

# Procedures
all : $(OBJECT)
	$(LD) -o $(TARGET) $(OBJECT) $(LDFLAGS)

clean :
	$(RM) $(TARGET) $(OBJECT) $(DEPEND)

help :
	@echo "make [all | clean | help]"
	@echo "	all   : Build $(TARGET). (default)"
	@echo "	clean : Clean $(TARGET) *.o *.dep"
	@echo "	help  : Show help information."

# Standard Procedures
%.dep : %.c
	@$(PP) $(CFLAGS) -MM -MT $(@:.dep=.o) -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPEND)

