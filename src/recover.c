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

void recover (int k, int n, char* secret_path, char* watermark_path, char* directory_path) { 

	printf ("Starting image recovery.\n");


	bmp_image_t8 *wimage = bmp_from_path8(watermark_path);

	if (wimage == NULL) {

		printf("Unable to get watermark image.\n");
		exit(EXIT_FAILURE);

	}

	int s_size = (wimage->info.imageWidth) * (wimage->info.imageHeight); //Secret tiene mismo size que watermark
	int max = (int) s_size/(n*n);

	if((max*(n*n)) != (s_size)) {
		printf("Invalid watermark image size.\n");
		exit(EXIT_FAILURE);
	}

	char* steg_type = "LSB1";

	DIR *directory;
	directory = opendir(directory_path); 

	if(directory == NULL){
		printf("Unable to open directory.\n");
		exit(EXIT_FAILURE);
	}

	struct dirent* file;
	int reached = 0;

	int share_size = 0;
	int steg_aux = 0;

	if (k == 2) {
		share_size = 4*3;
		steg_type = "LSB2";
	} else {
		// k == 4
		share_size = 8*3;
	}

	if (steg_type == "LSB1") {
		// k == 4
		steg_aux = 8;
	} else {
		steg_aux = 4;
	}

	int quantity = 0;
	int previous_quantity = 0;
	
	uint8_t shadows[k][max * share_size];
	int shadow_number[k];
	int width = 0;
	int height = 0;
	while ((file=readdir(directory)) != NULL && reached != k) {

		int index = 0;  
        uint8_t * sh;

        if ((strcmp("bmp",get_filename_ext(file->d_name)) == 0)) {

        	printf("%s\n", file->d_name);
        	char *result = malloc(strlen(directory_path) + strlen(file->d_name) + 1); 
		    strcpy(result, directory_path);
		    strcat(result, file->d_name);
            bmp_image_t24 *image = bmp_from_path24(result);
            int sharefile_size_encrypted = (image->info.imageWidth) * (image->info.imageHeight) * 3;
            width = (image->info.imageWidth);
            height = (image->info.imageHeight);
            int sharefile_size_decrypted = sharefile_size_encrypted/steg_aux;
            previous_quantity = quantity;
            quantity = sharefile_size_decrypted/share_size;

            if (quantity!= 0 && previous_quantity !=0 && previous_quantity !=quantity) {

            	printf("Error in shares\n");
            	exit(EXIT_FAILURE);

            }

			sh = (uint8_t *) recover_data(image, steg_type); 

			for (int i = 0; i < quantity * share_size; i++){

				shadows[reached][i] = sh[index]; // en shadows[0] guardaria todos los datos juntos de las share 0. 
				index++;

			}
			shadow_number[reached] = image->header.reserved1;
			//printf("num es %d\n", image->header.reserved1);
			reached++;
			index = 0;

			free(result);
			free(sh);
			free(image->data);
			free(image); 

        }

	}

	if (reached != k) {

		printf("Not enough shares\n");
		exit(EXIT_FAILURE);

	}

	closedir(directory);
	int secret_size = n * max * steg_aux;
	uint8_t * secret_projection_extended = calloc(secret_size, 1);;
	uint8_t * r_extended = calloc(secret_size, 1);;
	int sp_index = 0;
	int r_index = 0;

	for (int curr = 0; curr < quantity; curr++) {

		int secret_projection[n][n];
		int my_shadows[k][n][3]; // ejemplo si uso 4,8 voy a tneer 4 shadows de 8x3

		for (int curr_k = 0; curr_k < k; curr_k ++) {

			for (int aux_i = 0; aux_i < n; aux_i++) {

				for (int aux_j = 0; aux_j < 3; aux_j++) {

					my_shadows[curr_k][aux_i][aux_j] = (int) shadows[curr_k][curr*share_size + aux_i*3 + aux_j];

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
		

		for (int counter = 0; counter < k ; counter++) {
			int r;
			int c;
			int aux[n][3];
			for (r = 0; r < n; r++) {
				    for (c = 0; c < 3; c++) {
				        aux[r][c] = my_shadows[counter][r][c];
				    }
			}
			separate_matrix_by_column(1, 2, n, 3, aux, v[counter], g[counter]);
		}

		if (k == 2) {
			concat (n, 1,1, v[0], v[1], b_matrix);
		} else {
			//k == 4

			int curr1[n][2];
			concat (n, 1,1, v[0], v[1], curr1);		
			int curr2[n][2];
			concat (n, 1,1, v[2], v[3], curr2);	
			concat (n, 2,2, curr1, curr2, b_matrix);
			
		}



		/*
		ahora vamos a obtener secret_projection que surge de proyeccion de B
		*/
		calculate_projection(n, k, b_matrix, n, n, secret_projection); 
		
		// ahora guado los datos en secret_extended para desps sumar a rw

		for (int si = 0; si < n; si++) {

			for (int sj= 0; sj < n; sj++) {

				secret_projection_extended[sp_index] = (uint8_t) secret_projection[si][sj];
				sp_index++;

			}
		}


		int r_matrix[n][n];

		for (int r_row =0; r_row < n; r_row++) {
			
			if (k == 4) {

				int g00 = g[0][r_row][0]; //ESTO ES DE G0 elemento en la fila r_row y columna 0
				int g10 = g[1][r_row][0]; //ESTO ES DE G1 elemento en la fila r_row y columna 0
				int g20 = g[2][r_row][0]; //ESTO ES DE G2 elemento en la fila r_row y columna 0
				int g30 = g[3][r_row][0]; //ESTO ES DE G3 elemento en la fila r_row y columna 0
				int g01 = g[0][r_row][1]; //ESTO ES DE G0 elemento en la fila r_row y columna 1
				int g11 = g[1][r_row][1]; //ESTO ES DE G1 elemento en la fila r_row y columna 1
				int g21 = g[2][r_row][1]; //ESTO ES DE G2 elemento en la fila r_row y columna 1
				int g31 = g[3][r_row][1]; //ESTO ES DE G3 elemento en la fila r_row y columna 1


				/* shadow_number es un vector de longitud k. Guardo en shadow_number[0] lo que yo lei como shadow 0, la primer portadora
				que lei */

				int g0_coef = shadow_number[0] + 1;  // G0 en realidad no viene de la shadow nro 0. Aca le asigno de q numero viene. 
				int g1_coef = shadow_number[1] + 1; // y asi...
				int g2_coef = shadow_number[2] + 1; // Sumo uno pq tengo share 0 hasta 7 y yo quiero de 1 a 8
				int g3_coef = shadow_number[3] + 1;


				//  https://math.stackexchange.com/questions/3264557/solution-to-linear-equation-system-using-modulo-251
				// 
				int cubes[4] = {(int_pow(g0_coef,3))%251, (int_pow(g1_coef,3))%251, (int_pow(g2_coef,3))%251, (int_pow(g3_coef,3))%251};
				for(int i = 0; i < 4; i++){
					if(shadow_number[i] == 7){
						cubes[i] = 0;
					}
					if(shadow_number[i] == 6){
						cubes[i] = 87;
					}
				}

					int m[4][4] = {{1,g0_coef%251,(int_pow(g0_coef,2))%251, cubes[0]},
								{1,g1_coef%251,(int_pow(g1_coef,2))%251, cubes[1]},
								{1,g2_coef%251,(int_pow(g2_coef,2))%251, cubes[2]},
								{1,g3_coef%251,(int_pow(g3_coef,2))%251, cubes[3]}};




					int inversaM[4][4];
					inverse(4,m, inversaM);
					int g0[4][1] = {{g00},{g10},{g20},{g30}};
					int g1[4][1] = {{g01},{g11},{g21},{g31}};

					int answer[4][1];
					int answer2[4][1];

					for(int i = 0; i < k; i ++){
						answer[i][0] = (inversaM[i][0]*g00 + inversaM[i][1]*g10 + inversaM[i][2]*g20 + inversaM[i][3]*g30)%251;
					}

					for(int i = 0; i < k; i ++){
						answer2[i][0] = (inversaM[i][0]*g01 + inversaM[i][1]*g11 + inversaM[i][2]*g21 + inversaM[i][3]*g31)%251;
					}
					
					r_matrix[r_row][0] = answer[0][0];
					r_matrix[r_row][1] = answer[1][0];
					r_matrix[r_row][2] = answer[2][0];
					r_matrix[r_row][3] = answer[3][0];
					r_matrix[r_row][4] = answer2[0][0];
					r_matrix[r_row][5] = answer2[1][0];
					r_matrix[r_row][6] = answer2[2][0];
					r_matrix[r_row][7] = answer2[3][0];


			} else {
				// k == 2

				int g00 = g[0][r_row][0];
				int g10 = g[1][r_row][0];
				int g01 = g[0][r_row][1];
				int g11 = g[1][r_row][1];

				int g0_coef = shadow_number[0] + 1;  // G0 en realidad no viene de la shadow nro 0. Aca le asigno de q numero viene. 
				int g1_coef = shadow_number[1] + 1; // Sumo uno pq tengo share 0 hasta 7 y yo quiero de 1 a 8

				int m[2][2] = {{1,g0_coef},{1,g1_coef}};
				int inversaM[2][2];
				inverse(2,m, inversaM);
				int g0[2][1] = {g00,g10};
				int g1[2][1] = {g01,g11};
				int answer[2][1];
				int answer2[2][1];
				multiply(2,2,2,1,inversaM,g0,answer);
				multiply(2,2,2,1,inversaM,g1,answer2);


				r_matrix[r_row][0] = answer[0][0];
				r_matrix[r_row][1] = answer[1][0];
				r_matrix[r_row][2] = answer2[0][0];
				r_matrix[r_row][3] = answer2[1][0];

			}
				

		}

		for (int si = 0; si < n; si++) {

			for (int sj = 0; sj < n; sj++) {

				r_extended[r_index] = (uint8_t) r_matrix[si][sj];				
				r_index++;

			}
		}


	}
	
	/*leo la rw */


	bmp_image_t8 *simage = bmp_from_path8(watermark_path); 
	/* esto es la generacion de la secreta*/
	uint8_t* water = bmp_get_data_buffer8(wimage);

	
	uint8_t * secret_data = calloc(secret_size, 1);
	int secret_index = 0;

	for (secret_index = 0; secret_index < secret_size; secret_index++) {

				secret_data[secret_index] = (r_extended[secret_index] + secret_projection_extended[secret_index])%251;
				
	}

	bmp_image_t8 *final_secret = simage;
	final_secret->data = secret_data;
	bmp_save8(final_secret, secret_path);
	printf("Recovered image saved %s.\n", secret_path);
	
	


	/* esto es la generacion de la watermark*/
	int rw[height][width];
	int rw_index = 0;


	
	

	int rw_row;
	int rw_col;
	uint8_t * new_data = calloc(secret_size, 1);

	for (rw_index = 0; rw_index < secret_size; rw_index++) {

				new_data[rw_index] = (water[rw_index] + secret_projection_extended[rw_index])%251;

	}



	/* solo cambio la data, total el header es el mismo */
	
	bmp_image_t8 *final_watermark = wimage;
	final_watermark->data = new_data;
	bmp_save8(final_watermark, "recovered_watermark.bmp");
	printf("Recovered watermark saved as recovered_watermark.bmp\n");

	free(water);
	free(final_watermark);
	free(secret_projection_extended);
	free(r_extended);
	free(new_data);
	free(secret_data);
	free(final_secret);

	
}



