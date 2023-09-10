CC = gcc
CFLAGS = -Wall -Werror -Wextra -o

run:
	$(CC) $(CFLAGS) main main.c -lncurses

clean:
	rm -rf main