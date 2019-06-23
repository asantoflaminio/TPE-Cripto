#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H
#include "bmp_processor.h"

/* Hides data using LSB1 */
static void lsb1_apply(uint8_t *destination, const uint8_t *source, long bytes_amount);

/* Hides data using LSB2 */
static void lsb2_apply(uint8_t *destination, const uint8_t *source, long bytes_amount);

/* Recovers data previously hidden with LSB1 */
static size_t lsb1_recover(uint8_t *destination, const uint8_t *source, long bytes_amount, int null_cutoff); 

/* Recovers data previously hidden with LSB2 */
static size_t lsb2_recover(uint8_t *destination, const uint8_t *source, long bytes_amount, int null_cutoff);

/* Recovers hidden data inside an image file */
uint8_t* recover_data(bmp_image_t24 *image, char* lsb);

/* Hides data into an image file */
int hide_data(uint8_t *image_buffer, const char *input_path, char* lsb, int current, int size);

#endif