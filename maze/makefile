CC = g++
CFLAGS = -Wall -c -std=c++11
LFLAGS = -Wall
APP = bg_maker

SDIR=src
ODIR=bin

_SRC = prim_maze shapes progress debug
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

$(ODIR)/%.o : $(SDIR)/%.cpp $(ODIR)
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