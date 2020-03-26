
all: list_xor

list_xor: list_xor.c
	gcc -g list_xor.c -o list_xor

memcheck: list_xor
	valgrind -q --leak-check=full ./list_xor

clean:
	rm -rf list_xor
