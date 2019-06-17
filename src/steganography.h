#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#include "bmp_processor.h"
static void lsb1_apply(uint8_t *destination, const uint8_t *source, long bytes_amount);
static void lsb2_apply(uint8_t *destination, const uint8_t *source, long bytes_amount);
static size_t lsb1_recover(uint8_t *destination, const uint8_t *source, long bytes_amount, int null_cutoff); 
static size_t lsb2_recover(uint8_t *destination, const uint8_t *source, long bytes_amount, int null_cutoff);
uint8_t* recover_data(bmp_image_t24 *image, char* lsb);
int hide_data(uint8_t *image_buffer, const char *input_path, char* lsb, int current);
#endif