CFLAGS	=	-c -Wall -Wextra -std=c11 -Werror -g

all:  scrambles

scrambles: main.o word.o unscramble.o
	gcc -o scrambles main.o word.o unscramble.o

unscramble.o: unscramble.c unscramble.h
	gcc ${CFLAGS} unscramble.c

word.o: word.c word.h
	gcc ${CFLAGS} word.c

main.o: main.c word.h
	gcc ${CFLAGS} main.c


clean:
	rm *.o  scrambles
