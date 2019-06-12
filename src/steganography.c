#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <stdint.h>
#include "steganography.h"


static void lsb1_crypt(uint8_t *dst, const uint8_t *src, long nbytes) {

    static uint8_t mask = (uint8_t) (~0x1);

    for (int i = 0, d = 0; i < nbytes; ++i) {

        for (int j = 7; j >= 0; --j) {
            uint8_t new_bit = (uint8_t) (src[i] & (0x1 << j));

            new_bit = new_bit >> j;


            dst[d] = (dst[d] & mask) | new_bit;
            d++;
        }
    }
}

static void lsb2_crypt(uint8_t *dst, const uint8_t *src, long nbytes) {

    static uint8_t mask = (uint8_t) (~0x3);

    for (int i = 0, d = 0; i < nbytes; ++i) {

        for (int j = 4; j >= 0; --j) {
            uint8_t new_bit = (uint8_t) (src[i] & (0x3 << (j*2)));

            new_bit = new_bit >> (j*2);


            dst[d] = (dst[d] & mask) | new_bit;
            d++;
        }
    }
}

static size_t lsb1_decrypt(uint8_t *dst, const uint8_t *src, long nbytes, int null_cutoff) {
    size_t curr_src = 0;
    size_t curr_dst = 0;
	//printf("nbytes es %d\n", nbytes);
    while (curr_dst < nbytes) {

        for (int j = 0; j < 8; ++j) {
            uint8_t new_bit = (uint8_t) (src[curr_src + j] & 0x1);

            new_bit = new_bit << (7 - j);

            dst[curr_dst] = dst[curr_dst] | new_bit;
        }

        if (null_cutoff && dst[curr_dst] == 0) {
            return curr_dst;
        }
        //printf("dst[%d] es %d\n", curr_dst,dst[curr_dst] );
        curr_src += 8;
        curr_dst++;
    }

   // printf("Ok hasta aca %d\n", curr_dst);
    return curr_dst;
}

static size_t lsb2_decrypt(uint8_t *dst, const uint8_t *src, long nbytes, int null_cutoff) {

    size_t curr_src = 0;
    size_t curr_dst = 0;

    while (curr_dst < nbytes) {

        for (int j = 0; j < 4; ++j) {
        	//printf("SoY %d\n", src[curr_src + j]);
            uint8_t new_bit = (uint8_t) (src[curr_src + j] & 0x3);
          //  printf("newbit es  %d\n", new_bit);
            new_bit = new_bit << (6 - j*2);

            dst[curr_dst] = dst[curr_dst] | new_bit;
        }

        if (null_cutoff && dst[curr_dst] == 0) {
            return curr_dst;
        }
        curr_src += 4;
        curr_dst++;
    }

    return curr_dst;
}

static void
stegobmp_extract_size(char* lsb, uint32_t *hidden_data_size, uint8_t *image_buffer, uint32_t *offset) {
	if (strcmp(lsb, "LSB1") == 0){
		lsb1_decrypt((uint8_t *) hidden_data_size, image_buffer, sizeof(*hidden_data_size), 0);
	}else{
		lsb2_decrypt((uint8_t *) hidden_data_size, image_buffer, sizeof(*hidden_data_size), 0);
	}
    *hidden_data_size = __bswap_32(*hidden_data_size);
}

static void
stegobmp_extract_data(char* lsb, void *raw_data, uint8_t *image_buffer, uint32_t hidden_data_size,
                      uint32_t *offset) {

	if (strcmp(lsb, "LSB1") == 0){
		lsb1_decrypt(raw_data, image_buffer + sizeof(uint32_t) * 8, hidden_data_size, 0);
	}else{		 
        lsb2_decrypt(raw_data, image_buffer + sizeof(uint32_t) * 4, hidden_data_size, 0);
	}
}

void* stegobmp_extract(bmp_image_t24 *image, const char *output_path, char* lsb) {

    uint8_t *image_buffer = bmp_get_data_buffer24(image);
    uint32_t hidden_data_size = 46200; //HARDCODEADO. SI ES CON LSB2 deberia ser el doble!!
   //uint8_t hidden_data_size = 0;
    char *extension;
    bmp_image_t24 * answer = image;
    uint32_t offset = 0;

    //stegobmp_extract_size(lsb, &hidden_data_size, image_buffer, &offset);

    if (hidden_data_size < bmp_get_image_size24(image)) {
        printf("[!] Embeded file possibly found.\n");
        printf("size de image era %d\n", bmp_get_image_size24(image) );
    	printf("size de hiddenimage es %d\n", hidden_data_size );
    } else {
    	printf("size de image era %d\n", bmp_get_image_size24(image) );
    	printf("size de hiddenimage es %d\n", hidden_data_size );
        //return 1;
    }
   // printf("retorno.\n");
    //return 1;

    void *raw_data = calloc(hidden_data_size, 1);

    stegobmp_extract_data(lsb, raw_data, image_buffer, hidden_data_size, &offset);

		//printf("[!] Reached1.\n");
        extension = calloc(30, 1);
        uint8_t *tmp_offset;
        size_t ext_size = 0;
        //printf("[!] Reached2.\n");
        if (strcmp(lsb, "LSB1") == 0) {
        		tmp_offset = image_buffer + sizeof(hidden_data_size) * 8 + hidden_data_size * 8;
                ext_size = lsb1_decrypt((uint8_t *) extension, tmp_offset, hidden_data_size, 1);
    	}
    	else{ //LSB2
    			tmp_offset = image_buffer + sizeof(hidden_data_size) * 4 + hidden_data_size * 4; //dudoso el 3
                ext_size = lsb2_decrypt((uint8_t *) extension, tmp_offset, hidden_data_size, 1);
        }


        extension = realloc(extension, ext_size + 1);

        extension[ext_size] = 0;
    
    //printf("[!] Reached3.\n");
    char *filename = malloc(strlen(output_path) + strlen(extension) + 1);
    strcpy(filename, output_path);
    strcat(filename, extension);

    printf("[+] Saving to: %s\n", filename);
    FILE *fp = fopen(filename, "wb+");

    if (fp == NULL) {
        printf("Could not open file %s: ", output_path);
        printf("%s\n", strerror(errno));
      //  return 3;
    }

    fwrite(raw_data, hidden_data_size, 1, fp);

    fclose(fp);
   // answer->data = raw_data;
    //return answer;
    return raw_data;

}



