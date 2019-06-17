#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bmp_processor.h"





bmp_image_t8 *bmp_from_path8(const char *path) {
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("Could not open file %s: ", path);
        printf("%s\n", strerror(errno));
        return NULL;
    }

    bmp_image_t8 *image = malloc(sizeof(bmp_image_t8));

    fread(&image->header, sizeof(struct bmp_header), 1, fp);
    fread(&image->info, sizeof(struct bmp_info), 1, fp);
    for(int i=0; i < 256; i++){
        fread(&image->bmiColors[i], sizeof(RGBQUAD), 1, fp);
    }
    
    const size_t header_size = sizeof(struct bmp_header) + sizeof(struct bmp_info) + sizeof(RGBQUAD)*256;
    // printf("header size es %d\n", header_size);
    // printf("imageDataOffset es %d\n", image->header.imageDataOffset);
    if (header_size != image->header.imageDataOffset) {
        printf("Image contains extra data!\n");
        goto _ABORT;
    }

    if (image->info.bitsPerPixel != 24 && image->info.bitsPerPixel != 8) {
        printf("Image does not contains 24 or 8 bits per pixel.\n");
        goto _ABORT;
    }

    if (image->info.compressionMethod != 0) {
        printf("Image is compressed\n");
        goto _ABORT;
    }

    if (image->header.imageDataOffset - header_size != 0) {
        printf("Image contains extra data between header and bitmap!\n");
        goto _ABORT;
    }

    image->data = malloc(image->info.imageSize);
    fread(image->data, image->info.imageSize, 1, fp);

    fclose(fp);

    return image;

    _ABORT:
    free(image->data);
    free(image);
    fclose(fp);

    return NULL;
}

bmp_image_t24 *bmp_from_path24(const char *path) {
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("Could not open file %s: ", path);
        printf("%s\n", strerror(errno));
        return NULL;
    }

    bmp_image_t24 *image = malloc(sizeof(bmp_image_t24));

    fread(&image->header, sizeof(struct bmp_header), 1, fp);
    fread(&image->info, sizeof(struct bmp_info), 1, fp);

    const size_t header_size = sizeof(struct bmp_header) + sizeof(struct bmp_info);
    // printf("header size es %d\n", header_size);
    // printf("imageDataOffset es %d\n", image->header.imageDataOffset);
    if (header_size != image->header.imageDataOffset) {
        printf("Image contains extra data!\n");
        goto _ABORT;
    }

    if (image->info.bitsPerPixel != 24 && image->info.bitsPerPixel != 8) {
        printf("Image does not contains 24 or 8 bits per pixel.\n");
        goto _ABORT;
    }

    if (image->info.compressionMethod != 0) {
        printf("Image is compressed\n");
        goto _ABORT;
    }

    if (image->header.imageDataOffset - header_size != 0) {
        printf("Image contains extra data between header and bitmap!\n");
        goto _ABORT;
    }

    image->data = malloc(image->info.imageSize);
    fread(image->data, image->info.imageSize, 1, fp);

    fclose(fp);

    return image;

    _ABORT:
    free(image->data);
    free(image);
    fclose(fp);

    return NULL;
}


int bmp_save8(const bmp_image_t8 *image, const char *path) {
    FILE *fp = fopen(path, "wb+");

    if (fp == NULL) {
        printf("Could not open file %s: ", path);
        printf("%s\n", strerror(errno));
        return 0;
    }

    fwrite(&image->header, sizeof(struct bmp_header), 1, fp);
    fwrite(&image->info, sizeof(struct bmp_info), 1, fp);
    for(int i=0; i<256; i++){
        fwrite(&image->bmiColors[i], sizeof(RGBQUAD), 1, fp);
    }
    fwrite(image->data, image->info.imageSize, 1, fp);

    fclose(fp);

    return 1;
}

void bmp_free8(bmp_image_t8 *image) {

    free(image->data);
    free(image);
}

uint8_t *bmp_get_data_buffer8(bmp_image_t8 *image) {
    return image->data;
}

uint32_t bmp_get_image_size8(bmp_image_t8 *image) {
    return image->info.imageSize;
}

int bmp_check_size8(bmp_image_t8 *image, long size) {
    return image->info.imageSize > size * 8;
}

int bmp_save24(const bmp_image_t24 *image, const char *path) {
    FILE *fp = fopen(path, "wb+");

    if (fp == NULL) {
        printf("Could not open file %s: ", path);
        printf("%s\n", strerror(errno));
        return 0;
    }

    fwrite(&image->header, sizeof(struct bmp_header), 1, fp);
    fwrite(&image->info, sizeof(struct bmp_info), 1, fp);
    fwrite(image->data, image->info.imageSize, 1, fp);

    fclose(fp);

    return 1;
}

void bmp_free24(bmp_image_t24 *image) {

    free(image->data);
    free(image);
}

uint8_t *bmp_get_data_buffer24(bmp_image_t24 *image) {
    return image->data;
}

uint32_t bmp_get_image_size24(bmp_image_t24 *image) {
    return image->info.imageSize;
}

int bmp_check_size24(bmp_image_t24 *image, long size) {
    return image->info.imageSize > size * 8;
}