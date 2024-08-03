CC=gcc -std=c11
CFALGS=-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -ggdb
LIBS=-lX11 -lm 

lugi: main.c luigi.h
	$(CC) $(CFALGS) $^ -o $@ $(LIBS)

example: luigi_example.c
	$(CC) $(CFALGS) -O2 $< -o $@ $(LIBS)

