CC=gcc
CFLAGS=-g -lcrypto -lm
SRC= src/main.c 

visual_ss: $(SRC)
	$(CC) -o ss $(SRC) $(CFLAGS)