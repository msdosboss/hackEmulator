debug: main.c cpu.c computer.c
	gcc -g main.c cpu.c computer.c -lSDL2 -o main

all: main.o cpu.o computer.o
	gcc main.o cpu.o computer.o -lSDL2 -o main

main.o: main.c
	gcc -c main.c -lSDL2 -o main.o

computer.o: computer.c
	gcc -c computer.c -o computer.o

cpu.o: cpu.c
	gcc -c cpu.c -o cpu.o

clean:
	rm -f *.o main
