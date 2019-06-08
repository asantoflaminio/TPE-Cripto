#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include <getopt.h>
#include "bmp_processor.h"
#include "matrix_functions.h"
#include "steganography.h"
#include "utils.h"

void recover(int k, int n){ //, image_t* output_image, image_t* watermark_image){
	char* directory_path = "./Archivos de Prueba-4-8/shares/";
  //lo tendria q hacer por cada sombra
  //por cada cosa en el directorio q me pasan...
	DIR *directory;
	directory = opendir(directory_path); //esto me lo va a pasar el usuario *el path*
	struct dirent* file;
	int reached = 0;
	int rows;
	int cols;
	int shadows[k][330][210];//seria lo mas grande  q voy a tener, de ambos.
	if(k == 2 && n == 4){
		//LSB2
		//int shadows[k][330][210];
		rows = 330;
		cols = 210;
	}
	else if(k == 4 && n == 8){
		//int shadows[k][165][105];
		rows = 165;
		cols = 105;
	}else{
		return;
	}


	while ((file=readdir(directory)) != NULL && reached != k) {
		int index = 0;
        printf("%s\n", file->d_name);
        uint8_t * sh;
        if(strcmp("bmp",get_filename_ext(file->d_name)) == 0){
        	char *result = malloc(strlen(directory_path) + strlen(file->d_name) + 1); // +1 for the null-terminator
		    // in real code you would check for errors in malloc here
		    strcpy(result, directory_path);
		    strcat(result, file->d_name);
            bmp_image_t *image = bmp_from_path(result);
			sh = stegobmp_extract(image, "prueba_output.bmp", "LSB1"); 

			for(int i=0; i < rows; i++){
				for(int j = 0; j< cols;j++){
					printf("reached %d, i %d, j %d\n", reached,i,j);
					shadows[reached][i][j] = sh[index];
					index++;
				}
			}
			reached++;


			//shadows[reached] = sh;
        }
        

	}
		

    

    closedir(directory);
   
  // leo mi portadora
	// faltaria la watermark

}

