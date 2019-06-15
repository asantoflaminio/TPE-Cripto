#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include <limits.h>
#include <stdint.h> 
int64_t seed; 

void setSeed(int64_t s) {
 seed = (s ^ 0x5DEECE66DL) & ((1LL << 48) - 1);
}

uint8_t nextChar(void){
 seed = (seed * 0x5DEECE66DL + 0xBL) & ((1LL << 48) - 1);
 return (uint8_t)(seed>>40);
} 

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void assure(int truth, char* message) {
  if (!truth) {
    printf("%s", message);
    exit(EXIT_FAILURE);
  }
}
/*
int inverse(int i) {

  i %= 251;
  if (i < 1 || i > 251) {
    return -1;
  }
  return inverses[i - 1];
}*/