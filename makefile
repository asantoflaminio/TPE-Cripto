CC=gcc
CFLAGS=-g -lcrypto -lm
SRC= src/main.c src/utils.c src/bmp_processor.c

visual_ss: $(SRC)
	$(CC) -o test $(SRC) $(CFLAGS)

clean:
	rm -f *.o test