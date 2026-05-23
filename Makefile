CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lraylib -lm -ldl -lpthread -lGL -lX11

OBJS = main.o node.o


main: $(OBJS)
	gcc $(OBJS) -o main $(LDFLAGS)

main.o: main.c node.h config.h
	gcc -c main.c $(CFLAGS)

node.o: node.c node.h config.h
	gcc -c node.c $(CFLAGS)


.PHONY: run clean

run: main
	./main

clean:
	rm -f *.o main
