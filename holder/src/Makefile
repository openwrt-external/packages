CC ?= gcc
CFLAGS ?= -O2 -Wall -pedantic
CFLAGS += -std=gnu99

.PHONY: clean

build:
	$(CC) $(CFLAGS) main.c -o holder -lcurl -lssl -lcrypto

clean:
	rm -f holder
