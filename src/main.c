#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "bmp_processor.h"
#include "recover.h"

#define FALSE 0
#define TRUE 1



int main(int argc, char* argv[]) {
  /*int recoverImage = FALSE;
  int k = -1;
  int n = 0;
  char * secret_image = calloc(sizeof(char), 40);
  char * mark_image = calloc(sizeof(char), 40);
  char * dir = calloc(sizeof(char), 40);

  int opt = 0;
  static struct option long_options[] = {
        {"s",    required_argument, 0,  's' },
        {"m",      required_argument, 0,  'm' },
        {"dir",       required_argument, 0,  'i' },
        {"d",         no_argument,       0,  'd' },
        {"r",         no_argument,       0,  'r' },
        {"k",         required_argument, 0,  'k' },
        {"n",         required_argument, 0,  'n' },
        {0,           0,                 0,  0   }
  };

  int long_index =0;
  while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
    switch (opt){
      case 's':
        strcpy(secret_image, optarg);
        break;
       case 'm':
        strcpy(mark_image, optarg);
        break;
      case 'i':
        strcpy(dir, optarg);
        break;
      case 'd':
        recoverImage = FALSE;
        break;
      case 'r':
        recoverImage = TRUE;
        break;
      case 'k':
        k = atoi(optarg);
        break;
      case 'n':
        n = atoi(optarg);
        break;
      default:
        printf("Invalid!\n");
        break;
    }
  }

  if(k == -1){
  	printf("Failure: k was not specified! \n");
  	exit(EXIT_FAILURE);
  }
  if(!(k >= 2 && k <= n)){
  	printf("Failure: k must be between 2 and n \n");
  	exit(EXIT_FAILURE);
  }

  if(dir[0] == 0){
  	printf("Failure: directory was not specified \n");
  	exit(EXIT_FAILURE);
  }

  printf("Parameters validated! \n");

  if (recoverImage) {
   // write_image(decrypt(dir, k, image)); //change this
  } else {
  //  encode(read_image(image), k, n, dir); //change this
  }*/

  //image_t *image = read_image("./beautybeastshare.bmp");




  //write_image(image);

  recover(2, 4); //, image_t* output_image, image_t* watermark_image)

  return 0;
}