#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include <getopt.h>
#include "bmp_processor.h"
#include "steganography.h"
#include "matrix_functions.h"
#include "utils.h"

void distribute(int k, int n){ 

	char* secret_path = "./Archivos de Prueba-4-8/Secreto.bmp";
	char* watermark_path = "generated_watermark.bmp";
	char* directory_path = "./Archivos de Prueba-4-8/shares/test"; 
	
	bmp_image_t8 *image = bmp_from_path8(secret_path);
	if(image == NULL){
		printf("Unable to get secret image.\n");
		return;
	}
	int secret_size = (image->info.imageWidth) * (image->info.imageHeight);
	int shadow_size = (secret_size/n)*3;
	uint8_t shadows[n][shadow_size]; //sin el tema de LSB1 o LSB2
	int individual_s_size = n*n;
	int quantity = secret_size/(n*n);

	uint8_t * secret_data = bmp_get_data_buffer8(image);

	for(int q = 0; q < quantity; q++){

		int s_matrix[n][n];

		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				s_matrix[i][j] =  secret_data[q*individual_s_size + i*n + j];   
			}
		}

		//Para la matrix A de m*k tomo m=8 si k es 4 y m=4 sino, es decir seria lo mismo que n

		int a_matrix[n][k];
		uint8_t num;
		setSeed(SET);
		for (int g = 0; g < MAX; g++)
		{
			num = nextChar();
			printf("%d\t", num);
		} 

	}




}