debug: main.c cpu.c
	gcc -g main.c cpu.c -o main

all: main.o cpu.o
	gcc main.o cpu.o -o main

main.o: main.c
	gcc -c main.c -o main.o

cpu.o: cpu.c
	gcc -c cpu.c -o cpu.o

clean:
	rm -f *.o main
