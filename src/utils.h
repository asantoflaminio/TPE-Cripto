#ifndef __utils__
#define __utils__
#include <unistd.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
typedef unsigned char byte;

#define MAX 50
#define SET 10 


/* Gets filename extension */
const char *get_filename_ext(const char *filename);

/* Funciones provistas por la catedra */
void setSeed(int64_t s);
uint8_t nextChar(void);

/* Funcion para hacer potencias en int. https://stackoverflow.com/questions/29787310/does-pow-work-for-int-data-type-in-c */
int int_pow (int base, int exp);

#endif