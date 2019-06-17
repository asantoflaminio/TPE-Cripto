#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#include "bmp_processor.h"
static void lsb1_crypt(uint8_t *dst, const uint8_t *src, long nbytes);
static void lsb2_crypt(uint8_t *dst, const uint8_t *src, long nbytes);
static size_t lsb1_decrypt(uint8_t *dst, const uint8_t *src, long nbytes, int null_cutoff);
static size_t lsb2_decrypt(uint8_t *dst, const uint8_t *src, long nbytes, int null_cutoff);
uint8_t* stegobmp_extract(bmp_image_t24 *image, char* lsb);
int hide_data(uint8_t *image_buffer, const char *input_path, char* lsb, int current);
#endif