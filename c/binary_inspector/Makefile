CC = gcc
CFLAGS = -std=c11 \
		-Wall -Wextra -Wpedantic \
		-Werror \
		-Wshadow -Wcast-align -Wpointer-arith \
		-Wstrict-prototypes -Wmissing-prototypes \
		-Wformat=2 -Wformat-security \
		-Wundef \
		-Wconversion -Wsign-conversion \
		-fno-common \
		-O0 -g

binview: binview.c
	$(CC) $(CFLAGS) $< -o $@

helloworld: helloworld.c
	$(CC) $(CFLAGS) $< -o $@

all: binview helloworld

clean:
	rm -f binview helloworld
