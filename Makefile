run: main
	./main

CFLAGS = -g -lraylib -lm -ldl -lpthread -lGL -lX11

main: main.c node.o
	gcc main.c node.o -o main $(CFLAGS)

node.o: node.c
	gcc -c node.c $(CFLAGS)

clean:
	rm *.o
	rm main