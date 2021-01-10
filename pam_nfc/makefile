# Compiler and flags
CC = gcc
CFLAGS = -lnfc -lcrypt -lpam -lpam_misc

# Directory paths
IDIR = include
ODIR = obj
SDIR = src

# Compile both programs
all: add_user authenticate

# Compilation of add_user
_DEPS = add_user.h nfc_functions.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = add_user.o nfc_functions.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

add_user: $(OBJ)	
	$(CC) -o $@ $^ $(CFLAGS)

# Compilation of authenticate
_MDEPS = authenticate.h nfc_functions.h
MDEPS = $(patsubst %,$(IDIR)/%,$(_MDEPS))

_MOBJ = authenticate.o nfc_functions.o
MOBJ = $(patsubst %,$(ODIR)/%,$(_MOBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(MDEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

authenticate: $(MOBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Clean the program by removing the object files
.PHONY: clean

clean:
		rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~