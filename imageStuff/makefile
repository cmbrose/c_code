CC = gcc
DEBUG =
EX_LIBS= -lm
CFLAGS = -Wall -c -Ilibs
LFLAGS = -Wall
APP = make_image

WINDOWS=SystemRoot

SDIR=src
ODIR=bin

__SRC = $(patsubst $(SDIR)/%.c,%,$(wildcard $(SDIR)/*.c))
_SRC = $(filter-out $(APP), $(__SRC))
SRC_H =$(wildcard $(SDIR)/*.h)
SOBJ = $(patsubst %,$(ODIR)/%.o,$(_SRC))

all: $(ODIR)/$(APP).exe

$(ODIR)/$(APP).exe: $(SOBJ) $(ODIR) $(ODIR)/$(APP).o $(SDIR)/$(APP).c
	@echo Building $(APP)
	@$(CC) $(LFLAGS) $(SOBJ) $(ODIR)/$(APP).o -o $@ $(EX_LIBS)
    
$(ODIR)/%.o: $(SDIR)/%.c $(ODIR) $(SRC_H)
	@echo Building $<
	@$(CC) $(CFLAGS) $< -o $@

$(ODIR):
	@mkdir $(ODIR)

clean:
ifdef $(WINDOWS)
	@cmd \/C RMDIR /S /Q $(ODIR)
else
	@rm -rf $(ODIR)
endif
	@echo Complete
