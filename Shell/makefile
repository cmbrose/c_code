LDIR  	=lib
CC    	=gcc
CFLAGS	=-I$(LDIR) -Wall -g

ODIR  	=bin
SDIR  	=src

define PRINT_COMPILE
	@echo Building $@
	@$(CC) -c -o $@ $< $(CFLAGS)
endef

## SRC FILES ##

_SRC 	=shell.h builtin.h proc_run.h alias.h
SRCDEP  =$(patsubst %,$(SDIR)/%,$(_SRC))
SRCOBJ  =$(patsubst %.h,$(ODIR)/%.o,$(_SRC))

$(ODIR)/%.o: $(SDIR)/%.c $(SRCDEP)
	$(PRINT_COMPILE)

## LIB FILES ##
	
_LIB 	=linked_list.h input.h str_tools.h hashtable.h
LIBDEP  =$(patsubst %,$(LDIR)/%,$(_LIB))
LIBOBJ  =$(patsubst %.h,$(ODIR)/%.o,$(_LIB))
	
$(ODIR)/%.o: $(LDIR)/%.c $(LIBDEP)
	$(PRINT_COMPILE)
	
## MAKE BIN AND EXE ##

all: $(SRCOBJ) $(LIBOBJ) shell

# Ensure bin exists
$(SRCOBJ): | $(ODIR)
$(LIBOBJ): | $(ODIR)

$(ODIR):
	@mkdir -p $(ODIR)
	
shell: $(SRCOBJ) $(LIBOBJ)
	@echo Building $@
	@gcc -o $@ $^ $(CFLAGS)
	@echo Build complete

## CLEANING ##
	
.PHONY: clean

clean:
	@rm -f $(ODIR)/*.o
	@rmdir $(ODIR)
	@rm -f shell
	@echo Clean complete