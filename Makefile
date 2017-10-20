all: learn

learn: learn.c
	gcc -Wall -Werror -fsanitize=address -std=c99 learn.c -o learn

clean:
	rm -f learn
