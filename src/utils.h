#ifndef __utils__
#define __utils__
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
typedef unsigned char byte;

#define MAX 50
#define SET 10 


/* Funciones provistas por la catedra */
const char *get_filename_ext(const char *filename);
void setSeed(int64_t s);
uint8_t nextChar(void);
#endif