CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall
APP = threadimage

SDIR=src
ODIR=bin

_SRC = image_utils arraylist
SRC = $(patsubst %,$(SDIR)/%.h,$(_SRC))
OBJ = $(patsubst %,$(ODIR)/%.o,$(_SRC))

all: $(APP).exe
$(APP): $(APP).exe
release: $(APP).exe

debug: CFLAGS += -DDEBUG -g
debug: release

$(APP).exe: $(OBJ) $(ODIR) $(ODIR)/$(APP).o
	@echo Building $(APP)
	@$(CC) $(LFLAGS)  $(OBJ) $(ODIR)/$(APP).o -o $(APP)

$(ODIR)/%.o : $(SDIR)/%.c $(ODIR)
	@echo Building $@
	@$(CC) $(CFLAGS) $< -o $@

$(ODIR):
	@mkdir $(ODIR)

clean:
	@rm -rf $(ODIR)
	@if [ -a $(APP) ]; \
	then \
		rm $(APP)*; \
	fi;
	@echo Complete