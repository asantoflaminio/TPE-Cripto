#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <stdint.h>
#include "steganography.h"

static void lsb1_apply(uint8_t *destination, const uint8_t *source, long bytes_amount) {

    static uint8_t mask = (uint8_t) (~0x1);

    for (int i = 0, d = 0; i < bytes_amount; ++i) {

        for (int j = 7; j >= 0; --j) {

            uint8_t bit_value = (uint8_t) (source[i] & (0x1 << j));
            bit_value = bit_value >> j;
            destination[d] = (destination[d] & mask) | bit_value;
            d++;

        }
    }
}

static void lsb2_apply(uint8_t *destination, const uint8_t *source, long bytes_amount) {

    static uint8_t mask = (uint8_t) (~0x3);

    for (int i = 0, d = 0; i < bytes_amount; ++i) {
        
        for (int j = 3; j >= 0; --j) {

            uint8_t bit_value = (uint8_t) (source[i] & (0x3 << (j*2)));
            bit_value = bit_value >> (j*2);
            destination[d] = (destination[d] & mask) | bit_value;
            d++;

        }
    }
}

static size_t lsb1_recover(uint8_t *destination, const uint8_t *source, long bytes_amount, int null_cutoff) {

    size_t s_index = 0;
    size_t d_index = 0;

    while (d_index < bytes_amount) {

        for (int j = 0; j < 8; ++j) {

            uint8_t bit_value = (uint8_t) (source[s_index + j] & 0x1);
            bit_value = bit_value << (7 - j);
            destination[d_index] = destination[d_index] | bit_value;

        }

        if (null_cutoff && destination[d_index] == 0) {
            return d_index;
        }

        s_index += 8;
        d_index++;
    }

    return d_index;
}


static size_t lsb2_recover(uint8_t *destination, const uint8_t *source, long bytes_amount, int null_cutoff) {

    size_t s_index = 0;
    size_t d_index = 0;

    while (d_index < bytes_amount) {

        for (int j = 0; j < 4; ++j) {

            uint8_t bit_value = (uint8_t) (source[s_index + j] & 0x3);
            bit_value = bit_value << (6 - j*2);
            destination[d_index] = destination[d_index] | bit_value;

        }

        if (null_cutoff && destination[d_index] == 0) {
            return d_index;
        }

        s_index += 4;
        d_index++;
    }

    return d_index;
}

static void recover_pure_data(char* lsb, uint8_t *new_data, uint8_t *image_buffer, uint32_t hidden_data_size, uint32_t *offset) {

	if (strcmp(lsb, "LSB1") == 0) {
		lsb1_recover(new_data, image_buffer , hidden_data_size, 0);
	} else {		 
        lsb2_recover(new_data, image_buffer, hidden_data_size, 0);
	}
}


uint8_t* recover_data(bmp_image_t24 *image, char* lsb) {

    uint8_t *image_buffer = bmp_get_data_buffer24(image);
    int sharefile_size_encrypted = (image->info.imageWidth) * (image->info.imageHeight) * 3;

    uint32_t hidden_data_size = 46200; 
    int steg_aux = 8;

    if (strcmp(lsb, "LSB1") == 0) {
        steg_aux = 8;
    } else {
        steg_aux = 4;
    }

    hidden_data_size = sharefile_size_encrypted/steg_aux;

    bmp_image_t24 * answer = image;
    uint32_t offset = 0;

    uint8_t *new_data = calloc(hidden_data_size, 1);

    recover_pure_data(lsb, new_data, image_buffer, hidden_data_size, &offset);

    return new_data;

}


int hide_data(uint8_t *image_buffer, const char *input_path, char* lsb, int current, int size) {

    bmp_image_t24* porter = bmp_from_path24(input_path);
    uint8_t* porter_data = bmp_get_data_buffer24(porter);

    if (strcmp(lsb, "LSB1") == 0) {
        lsb1_apply(porter_data,image_buffer,  size);
    } else {
        //LSB2
        lsb2_apply(porter_data,image_buffer,  size);

    }

    porter->header.reserved1 = current;
    bmp_save24(porter, input_path);

    return 0;
}



