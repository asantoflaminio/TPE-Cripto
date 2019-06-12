#ifndef STEGOBMP_BMP_H
#define STEGOBMP_BMP_H

#include <stdint.h>

typedef struct bmp_image24 bmp_image_t24;
typedef struct bmp_image8 bmp_image_t8;
bmp_image_t8 *bmp_from_path8(const char *path);

int bmp_save8(const bmp_image_t8 *image, const char *path);

void bmp_free8(bmp_image_t8 *image);

uint8_t *bmp_get_data_buffer8(bmp_image_t8 *image);

uint32_t bmp_get_image_size8(bmp_image_t8 *image);

int bmp_check_size8(bmp_image_t8 *image, long size);

bmp_image_t24 *bmp_from_path24(const char *path);

int bmp_save24(const bmp_image_t24 *image, const char *path);

void bmp_free24(bmp_image_t24 *image);

uint8_t *bmp_get_data_buffer24(bmp_image_t24 *image);

uint32_t bmp_get_image_size24(bmp_image_t24 *image);

int bmp_check_size24(bmp_image_t24 *image, long size);

#endif 