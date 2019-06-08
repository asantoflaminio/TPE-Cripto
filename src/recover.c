#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "bmp_processor.h"
#include "matrix_functions.h"
#include "steganography.h"

void recover(int k, int n){ //, image_t* output_image, image_t* watermark_image){

  //lo tendria q hacer por cada sombra
  //por cada cosa en el directorio q me pasan...
  
  // leo mi portadora
	bmp_image_t *image = bmp_from_path("./beautybeastshare.bmp");
	stegobmp_extract(image, "prueba_output.bmp", "LSB1"); // faltaria la watermark

}