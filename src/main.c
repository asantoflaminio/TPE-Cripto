#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include "bmp_processor.h"
#include "recover.h"
#include "distribute.h"
#include "utils.h"

#define FALSE 0
#define TRUE 1



int main(int argc, char* argv[]) {
  setSeed(time(NULL));
  /*
  int recover_image = FALSE;
  int k = -1;
  int n = 0;
  char * secret_image = calloc(sizeof(char), 100);
  char * mark_image = calloc(sizeof(char), 100);
  char * dir = calloc(sizeof(char), 100);

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
  int count = 0;

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
        recover_image = FALSE;
        count++;
        break;
      case 'r':
        recover_image = TRUE;
        count++;
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

  if (count == 2) {
    printf("Failure: You can only choose between options r (recover) or d (distribute).\n");
    exit(EXIT_FAILURE);
  }

  if (count == 0) {
    printf("Failure: You must specify the desired action: r (recover) or d (distribute).\n");
    exit(EXIT_FAILURE);
  }

  if (secret_image[0] == 0) {
    printf("Failure: Secret image path was not specified.\n");
    exit(EXIT_FAILURE);
  } 

  if (mark_image[0] == 0) {
    printf("Failure: Watermark image path was not specified.\n");
    exit(EXIT_FAILURE);
  }

  if (dir[0] == 0) {
  	printf("Failure: Shares directory was not specified.\n");
  	exit(EXIT_FAILURE);
  }
  
  if (!((k == 2 && n == 4) || (k == 4 && n == 8))) {
    printf("Failure: Acceptable pairs are k=2 and n=4 OR k=4 and n=8.\n");
    exit(EXIT_FAILURE);
  }

  printf("Parameters validated! \n");

  if (recover_image) {
    recover(k,n, secret_image, mark_image, dir); 
  } else {
    distribute(k,n, secret_image, mark_image, dir);
  }*/


  // distribute(2,4, "./Archivos de Prueba-4-8/Secreto.bmp", "./Archivos de Prueba-4-8/Marca.bmp", "./24/");
  // recover(2, 4, "test.bmp", "generated_watermark.bmp", "./24/"); 

  distribute(4,8, "./Archivos de Prueba-4-8/Secreto.bmp", "./Archivos de Prueba-4-8/Marca.bmp", "./Archivos de Prueba-4-8/shares/");
  recover(4,8, "test.bmp", "generated_watermark.bmp", "./Archivos de Prueba-4-8/shares/"); 
  // printf("Action executed successfully.\n");
  exit(EXIT_SUCCESS);
}