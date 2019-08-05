CC = gcc
DEBUG =
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
APP = triangles

SDIR=src
ODIR=bin

_SRC = triangulation progress arraylist hashtable hashtable_iterator iterator
SRC = $(patsubst %,$(SDIR)/%.h,$(_SRC))
OBJ = $(patsubst %,$(ODIR)/%.o,$(_SRC))

$(APP).exe : $(OBJ) $(ODIR) $(ODIR)/$(APP).o
	@echo Building $(APP)
	@$(CC) $(LFLAGS)  $(OBJ) $(ODIR)/$(APP).o -o $(APP)

$(ODIR)/%.o : $(SDIR)/%.c $(ODIR) $(SRC)
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