CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall
LIB = json
APP = test

SDIR=src
ODIR=bin

_SRC = json json_dict json_array json_primatives
SRC = $(patsubst %,$(SDIR)/%.h,$(_SRC))
OBJ = $(patsubst %,$(ODIR)/%.o,$(_SRC))

all: $(ODIR)/$(LIB).o
release: $(APP).exe

debug: CFLAGS += -DDEBUG -g
debug: release

$(APP).exe: $(OBJ) $(ODIR) $(ODIR)/$(APP).o
	@$(CC) $(LFLAGS)  $(OBJ) $(ODIR)/$(APP).o -o $(APP)

$(ODIR)/%.o : $(SDIR)/%.c $(ODIR)
	@echo Building $@
	@$(CC) $(CFLAGS) $< -o $@

$(ODIR):
	@mkdir $(ODIR)

clean:
	@rm -rf $(ODIR)
	@if [ -a $(LIB) ]; \
	then \
		rm $(LIB)*; \
	fi;
	@echo Complete