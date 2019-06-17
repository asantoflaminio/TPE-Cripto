CC=gcc
CFLAGS=-g -lcrypto -lm
SRC= src/main.c src/utils.c src/bmp_processor.c src/steganography.c src/matrix_functions.c src/recover.c src/distribute.c

visual_ss: $(SRC)
	$(CC) -o test $(SRC) $(CFLAGS)

clean:
	rm -f *.o test