all: main.o cpu.o
	gcc main.o cpu.o -o main

main.o: main.c
	gcc -c main.c -o main.o

cpu.o: cpu.c
	gcc -c cpu.c -o cpu.o

clean:
	rm -f *.o main
