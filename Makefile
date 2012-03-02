# Makefile

MDKCMP_BIN	= mdkcmp

MDKCMP_SRC	= $(MDKCMP_BIN).c

CC	= gcc
DEBUG	= -Wall -Werror -g
CFLAGS	= -I.
LIBS	=

all: $(MDKCMP_BIN)
	make $(MDKCMP_BIN)

$(MDKCMP_BIN): $(MDKCMP_SRC)
	$(CC) $(DEBUG) -o $(MDKCMP_BIN) $(MDKCMP_SRC) $(CFLAGS) $(LIBS)

clean:
	rm $(MDKCMP_BIN)

rebuild:
	make clean
	make all
