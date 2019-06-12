#ifndef STEGOBMP_BMP_H
#define STEGOBMP_BMP_H

#include <stdint.h>
struct __attribute__ ((packed)) bmp_header {
    char fileIdentifier[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t imageDataOffset;
};

struct __attribute__ ((packed)) bmp_info {
    uint32_t headerSize;
    int32_t imageWidth;
    int32_t imageHeight;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t imageSize;
    int32_t horizontalRes;
    int32_t verticalRes;
    uint32_t colorsInPalette;
    uint32_t numImportantColors;
};


typedef struct                       /**** Colormap entry structure ****/
    {
    unsigned char  rgbBlue;          /* Blue value */
    unsigned char  rgbGreen;         /* Green value */
    unsigned char  rgbRed;           /* Red value */
    unsigned char  rgbReserved;      /* Reserved */
    } RGBQUAD;

struct bmp_image24 {
    struct bmp_header header;
    struct bmp_info info;
    uint8_t *data;
};

struct bmp_image8 {
    struct bmp_header header;
    struct bmp_info info;
    RGBQUAD  bmiColors[256];
    uint8_t *data;
};

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