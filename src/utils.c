#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void assure(int truth, char* message) {
  if (!truth) {
    printf("%s", message);
    exit(EXIT_FAILURE);
  }
}

int inverse(int i) {

  i %= 251;
  if (i < 1 || i > 251) {
    return -1;
  }
  return inverses[i - 1];
}