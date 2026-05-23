SRCDIR = src
CFLAGS = -Wall -Wextra -O2 -std=c11 -I$(SRCDIR)
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lX11

OBJS = main.o node.o

main: $(OBJS)
	gcc $(OBJS) -o main $(LDFLAGS)

main.o: $(SRCDIR)/main.c $(SRCDIR)/node.h $(SRCDIR)/config.h
	gcc -c $(SRCDIR)/main.c $(CFLAGS) -o main.o

node.o: $(SRCDIR)/node.c $(SRCDIR)/node.h $(SRCDIR)/config.h
	gcc -c $(SRCDIR)/node.c $(CFLAGS) -o node.o


.PHONY: run clean

run: main
	./main

clean:
	rm -f *.o main
