TARGET	= kodapa

CC	= gcc
CFLAGS	= -O2 -Wall
LFLAGS	= -Wall -lncurses

SDIR	= src
ODIR	= obj
BDIR 	= bin

SRC	:= $(wildcard $(SDIR)/*.c)
OBJ 	:= $(SRC:$(SDIR)/%.c=$(ODIR)/%.o)

all: $(BDIR)/$(TARGET)

$(BDIR)/$(TARGET): $(OBJ)
	@mkdir -p $(BDIR)
	@$(CC) -o $@ $(LFLAGS) $(OBJ)

$(OBJ): $(ODIR)/%.o : $(SDIR)/%.c
	@mkdir -p $(ODIR)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONEY: clean
clean:
	@rm -f $(OBJ) $(BDIR)/$(TARGET)
	@rmdir $(ODIR) $(BDIR) > /dev/null 2>&1 ||:
