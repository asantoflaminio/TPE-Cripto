#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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