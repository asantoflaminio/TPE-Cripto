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
    while (curr_dst < nbytes) {

        for (int j = 0; j < 8; ++j) {
            uint8_t new_bit = (uint8_t) (src[curr_src + j] & 0x1);

            new_bit = new_bit << (7 - j);

            dst[curr_dst] = dst[curr_dst] | new_bit;
        }

        if (null_cutoff && dst[curr_dst] == 0) {
            return curr_dst;
        }
        curr_src += 8;
        curr_dst++;
    }

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

static void stegobmp_extract_data(char* lsb, uint8_t *raw_data, uint8_t *image_buffer, uint32_t hidden_data_size,
                      uint32_t *offset) {

	if (strcmp(lsb, "LSB1") == 0){
		lsb1_decrypt(raw_data, image_buffer /*+ sizeof(uint32_t) * 8*/, hidden_data_size, 0);
	}else{		 
        lsb2_decrypt(raw_data, image_buffer /*+ sizeof(uint32_t) * 4*/, hidden_data_size, 0);
	}
}


uint8_t* stegobmp_extract(bmp_image_t24 *image, const char *output_path, char* lsb) {

    uint8_t *image_buffer = bmp_get_data_buffer24(image);
    uint32_t hidden_data_size = 46200; 
    if (strcmp(lsb, "LSB1") == 0){
        hidden_data_size = 46200;
    }else{
        hidden_data_size = 92400;
    }

    bmp_image_t24 * answer = image;
    uint32_t offset = 0;

    uint8_t *raw_data = calloc(hidden_data_size, 1);

    stegobmp_extract_data(lsb, raw_data, image_buffer, hidden_data_size, &offset);

    return raw_data;

}



