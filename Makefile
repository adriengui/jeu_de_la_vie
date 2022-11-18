CC = gcc
CFLAGS = -Wall
LDFLAGS = -lsx
SRC = main.c modele.c vue.c callbacks.c
PROG = main
OBJS = $(SRC:.c=.o)
.SUFFIXES: .c .o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: donnees.h vue.h
modele.o: donnees.h
vue.o: donnees.h vue.h callbacks.h
callbacks.o: donnees.h vue.h callbacks.h

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(PROG)
