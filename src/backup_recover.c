#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <math.h>
#include <getopt.h>
#include "bmp_processor.h"
#include "steganography.h"
#include "matrix_functions.h"
#include "utils.h"

void recover(int k, int n){ //, image_t* output_image, image_t* watermark_image){
	char* steg_type = "LSB1";

	// esto viene del main pasado!
	char* secret_path = "deberia_ser_albert.bmp";
	char* watermark_path = "./Archivos de Prueba-4-8/shares/RW/RW.bmp";
	char* directory_path = "./Archivos de Prueba-4-8/shares/"; 

	DIR *directory;
	directory = opendir(directory_path); 
	struct dirent* file;
	int reached = 0;

	
	int rows = 440;
	int cols =280;
	int share_size = 0;
	int steg_aux = 0;
	int max = 1925; //DESPS CAMBIARLO

	if(k==2){

		share_size = 4*3;
		steg_type = "LSB2";
		max = 7700;

	}else{
		// k == 4
		share_size = 8*3;

	}

	if(steg_type == "LSB1"){
		// k == 4
		steg_aux = 8;

	}else{

		steg_aux = 4;
	}

	int quantity = 0;
	int previous_quantity = 0;
	
	uint8_t shadows[k][max * share_size];
	int width = 0;
	int height = 0;

	while ((file=readdir(directory)) != NULL && reached != k) {

		int index = 0;  
        uint8_t * sh;

        if((strcmp("bmp",get_filename_ext(file->d_name)) == 0)){

        	printf("%s\n", file->d_name);
        	char *result = malloc(strlen(directory_path) + strlen(file->d_name) + 1); // +1 for the null-terminator
		    // in real code you would check for errors in malloc here
		    strcpy(result, directory_path);
		    strcat(result, file->d_name);
            bmp_image_t24 *image = bmp_from_path24(result);
            int sharefile_size_encrypted = (image->info.imageWidth) * (image->info.imageHeight) * 3;
            width = (image->info.imageWidth);
            height = (image->info.imageHeight);
            int sharefile_size_decrypted = sharefile_size_encrypted/steg_aux;
            previous_quantity = quantity;
            quantity = sharefile_size_decrypted/share_size;

            if(quantity!= 0 && previous_quantity !=0 && previous_quantity !=quantity){

            	printf("Error in shares\n");
            	return;

            }

			sh = (uint8_t *) stegobmp_extract(image, steg_type); 

			for(int i = 0; i < quantity * share_size; i++){

				shadows[reached][i] = sh[index]; // en shadows[0] guardaria todos los datos juntos de la share 0. 
				index++;

			}

			reached++;
			index = 0;

        }

	}
	if(reached != k){

		printf("Not enough shares\n");
		return;

	}

	closedir(directory);
	int secret_size = n * max * steg_aux;
	uint8_t * secret_projection_extended = calloc(secret_size, 1);;
	uint8_t * r_extended = calloc(secret_size, 1);;
	int sp_index = 0;
	int r_index = 0;


	for(int curr = 0; curr < quantity; curr++){

		// printf("spindex es %d\n", sp_index);
		int secret_projection[n][n];
		int my_shadows[k][n][3]; // ejemplo si uso 4,8 voy a tneer 4 shadows de 8x3

		for(int curr_k = 0; curr_k < k; curr_k ++){

			for(int aux_i = 0; aux_i < n; aux_i++){

				for(int aux_j = 0; aux_j < 3; aux_j++){

					my_shadows[curr_k][aux_i][aux_j] = shadows[curr_k][curr*share_size + aux_i*3 + aux_j];

				}

			}
			
		}
		

		int b_matrix[n][k]; // matriz b
		int v[k][n][1]; //matrices v
		int g[k][n][2]; //matrices g


		/*
		Obtengo la matriz B que se obtiene concatenando las primeras columnas de todas las shadows.
		rows va a ser igual a las filas de las shadows
		*/
		

		for (int counter=0; counter < k ;counter++){
			int r;
			int c;
			int aux[n][3];
			for (r=0; r<n; r++){
				    for(c=0; c<3; c++){
				        aux[r][c] = my_shadows[counter][r][c];
				    }
			}
			separateMatrixByColumn(1, 2, n, 3, aux, v[counter], g[counter]);
		}
		if (k==2){
			concat (n, 1,1, v[0], v[1], b_matrix);
		} else {
			//k == 4
			
			int curr1[n][2];
			concat (n, 1,1, v[0], v[1], curr1);		
			int curr2[n][3];
			concat (n, 2,1, curr1, v[2], curr2);	
			concat (n, 3,1, curr2, v[3], b_matrix);
			

		}

		/*
		ahora vamos a obtener secret_projection que surge de proyeccion de B
		*/
		calculateProjection(n, k, b_matrix, n, n, secret_projection); 
		
		// ahora guado los datos en secret_extended para desps sumar a rw

		for(int si = 0; si < n; si++){

			for(int sj= 0; sj < n; sj++){

				secret_projection_extended[sp_index] = (uint8_t) secret_projection[si][sj];
				sp_index++;

			}
		}


		int r_matrix[n][n];

		for(int r_row =0; r_row < n; r_row++){
			/* hago la matriz que tiene la columna de 1s y del 1 a k)*/
			int aux_g[k][k+1];
			int aux_g2[k][k+1];
			int filler = 1;
			int *ansgs1 = calloc(k, sizeof(int));
			int *ansgs2 = calloc(k, sizeof(int));
			
			for(int aux_row = 0; aux_row < k; aux_row++){
				aux_g[aux_row][0] = 1;
				for(int cj = 1; cj < k; cj++){
					aux_g[aux_row][cj] = pow(filler,cj);
				}
				aux_g[aux_row][k] = g[aux_row][r_row][0];
				filler++;
			}

			filler = 1;

			for(int aux_row = 0; aux_row < k; aux_row++){
				aux_g2[aux_row][0] = 1;
				for(int cj = 1; cj < k; cj++){
					aux_g2[aux_row][cj] = pow(filler,cj);
				}
				aux_g2[aux_row][k] = g[aux_row][r_row][1];
				filler++;
			}
				// printf("------\n");
				// printMatrix(k,k+1, aux_g);

				gauss_jordan(k, aux_g, &ansgs1);
				gauss_jordan(k, aux_g2, &ansgs2);
				//printf("valores son %d %d %d %d\n", ansgs1[0], ansgs1[1], ansgs1[2], ansgs1[3]);

				r_matrix[r_row][0] = ansgs1[0];
				r_matrix[r_row][1] = ansgs1[1];
				r_matrix[r_row][2] = ansgs1[2];
				r_matrix[r_row][3] = ansgs1[3];
				r_matrix[r_row][4] = ansgs2[0];
				r_matrix[r_row][5] = ansgs2[1];
				r_matrix[r_row][6] = ansgs2[2];
				r_matrix[r_row][7] = ansgs2[3];

		}

		for(int si = 0; si < n; si++){

			for(int sj = 0; sj < n; sj++){

				r_extended[r_index] = (uint8_t) r_matrix[si][sj];
				r_index++;

			}
		}



	}

	/*leo la rw */
	bmp_image_t8 *wimage = bmp_from_path8(watermark_path);
	bmp_image_t8 *simage = bmp_from_path8(watermark_path);
	/* esto es la generacion de la secreta*/

	
	uint8_t * secret_data = calloc(secret_size, 1);
	int secret_index = 0;

	for(secret_index = 0; secret_index < secret_size; secret_index++){

				secret_data[secret_index] = (r_extended[secret_index] + secret_projection_extended[secret_index])%251;

	}

	bmp_image_t8 *final_secret = simage;
	final_secret->data = secret_data;
	bmp_save8(final_secret, secret_path);
	printf("Recovered image saved %s.\n", secret_path);
	
	


	/* esto es la generacion de la watermark*/
	int rw[height][width];
	int rw_index = 0;


	
	uint8_t* water = bmp_get_data_buffer8(wimage);

	int rw_row;
	int rw_col;
	uint8_t * new_data = calloc(secret_size, 1);

	for(rw_index = 0; rw_index < secret_size; rw_index++){

				new_data[rw_index] = (water[rw_index] + secret_projection_extended[rw_index])%251;

	}



	/* solo cambio la data, total el header es el mismo */
	
	bmp_image_t8 *final_watermark = wimage;
	final_watermark->data = new_data;
	bmp_save8(final_watermark, "recovered_watermark.bmp");
	printf("Guardado recovered_watermark.bmp\n");

}

