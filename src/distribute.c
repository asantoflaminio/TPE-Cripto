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

void distribute(int k, int n){ 

	char* secret_path = "./Archivos de Prueba-4-8/Secreto.bmp";
	char* watermark_path = "./Archivos de Prueba-4-8/Marca.bmp";
	char* watermark_destiny_path = "generated_watermark.bmp";
	// char* directory_path = "./48/"; 
	char* directory_path = "./test24/"; 
	char* lsb;

	if(k == 2){

		lsb = "LSB2";

	}else{

		lsb = "LSB1";

	}

	bmp_image_t8 *image = bmp_from_path8(secret_path);
	bmp_image_t8 *wimage = bmp_from_path8(watermark_path);

	if(image == NULL){

		printf("Unable to get secret image.\n");
		return;

	}

	if(wimage == NULL){

		printf("Unable to get watermark image.\n");
		return;

	}


	int secret_size = (image->info.imageWidth) * (image->info.imageHeight);
	int shadow_size = (secret_size/n)*3;
	uint8_t shadows[n][shadow_size]; //sin el tema de LSB1 o LSB2
	int individual_s_size = n*n;
	int quantity = secret_size/(n*n);

	uint8_t * secret_data = bmp_get_data_buffer8(image);
	uint8_t * secret_projection_extended = calloc(secret_size, 1);
	int sp_index = 0;
	int sh_aux_counter = 0;
	uint8_t num;
	setSeed(SET);

	for(int q = 0; q < quantity; q++){

		int s_matrix[n][n];

		for(int i = 0; i < n; i++){

			for(int j = 0; j < n; j++){

				s_matrix[i][j] =  secret_data[q*individual_s_size + i*n + j];   

			}

		}

		//Para la matrix A de m*k tomo m=8 si k es 4 y m=4 sino, es decir seria lo mismo que n

		int a_matrix[n][k];
		//int a_matrix[4][2] =  {{3,7}, {6,1}, {2,5}, {6,6}}; //BORRAR y descometnar arriba y abajo
		for(int ai = 0; ai < n; ai++){

			for(int aj = 0; aj < n; aj++){

				num = nextChar();
				a_matrix[ai][aj] = num;

			}

		}

		
		// printf("----------------\n");
		// printMatrix(n,k, a_matrix);
		/*
		ahora vamos a obtener secret_projection que surge de proyeccion de A
		*/
		int secret_projection[n][n];
		calculateProjection(n, k, a_matrix, n, n, secret_projection); 


		for(int si = 0; si < n; si++){

			for(int sj = 0; sj < n; sj++){

				secret_projection_extended[sp_index] = (uint8_t) secret_projection[si][sj];
				sp_index++;

			}

		}

		/*
		ahora obtenemos r_matriz que surge de restar S con la proyeccion
		*/
		int r_matrix[n][n];
		substract(n, n, s_matrix, secret_projection, r_matrix);

		/* aca hay que generar n cantidad de X cada uno de kx1 con valores random */
		int x_matrices[n][k][1];
		//int x_matrices[4][2][1] = { {{9}, {5}} ,   {{4}, {4}} ,  {{9}, {8}}  ,  {{3}, {2}} }  ; //BORRAR y desocmentar arriba

		
		for(int x_counter = 0; x_counter < n; x_counter++){

			for(int col = 0; col < k; col++){

				num = nextChar();
				x_matrices[x_counter][col][0] = num;

			}

		} 
		
		int v_matrices[n][n][1];

		/* aca generar n matrices V cada una surje de hacer A*Xj */
		for(int v_counter = 0; v_counter < n; v_counter++){

			multiply(n ,k, k, 1, a_matrix, x_matrices[v_counter], v_matrices[v_counter]);
			// printf("--------------\n");
			// printMatrix(n,1,v_matrices[v_counter]);

		}

		int g_matrices[n][n][2];
		/* aca armar n matrices G cada una de n*2 */

		for(int g_counter = 0; g_counter < n; g_counter++){


			for(int i = 0; i < n; i++){

					if(k == 2){

						g_matrices[g_counter][i][0] = (r_matrix[i][0] + (r_matrix[i][1]*(((int) pow((g_counter+1),1))%251))%251)%251;
						g_matrices[g_counter][i][1] = (r_matrix[i][2] + (r_matrix[i][3]*(((int) pow((g_counter+1),1))%251))%251)%251;

					}else{

						g_matrices[g_counter][i][0] = (r_matrix[i][0] + (r_matrix[i][1]*(((int) pow((g_counter+1),1))%251))%251 + (r_matrix[i][2]*(((int) pow((g_counter+1),2))%251))%251+ (r_matrix[i][3]*(((int) pow((g_counter+1),3))%251))%251)%251;
						g_matrices[g_counter][i][1] = (r_matrix[i][4] + (r_matrix[i][5]*(((int) pow((g_counter+1),1))%251))%251 + (r_matrix[i][6]*(((int) pow((g_counter+1),2))%251))%251 + (r_matrix[i][7]*(((int) pow((g_counter+1),3))%251))%251)%251;
						
					}
					

			}


		}

		// printf("----\n");
		// printMatrix(n,2,g_matrices[0]);

		/* concateno cada v con su correspondiente G para obtener las n sombras*/
		int my_shadows[n][n][3];
		for(int sh_counter = 0; sh_counter < n; sh_counter++){
			concat (n, 1,2, v_matrices[sh_counter], g_matrices[sh_counter], my_shadows[sh_counter]);
		}

		/* agrego las sombras a su version extendida */
		for(int ext_counter = 0; ext_counter < n; ext_counter++){

			for(int i = 0; i < n; i++){

				for(int j = 0; j < 3; j++){

					shadows[ext_counter][sh_aux_counter] = my_shadows[ext_counter][i][j];
					sh_aux_counter++;

				}

			}

			sh_aux_counter = sh_aux_counter - n*3;

		}

		sh_aux_counter = sh_aux_counter + n*3;


	}

	/* aplico esteganografia a cada sombra extendida y guardo en archivo*/


	DIR *directory;
	directory = opendir(directory_path); 
	struct dirent* file;
	int reached = 0;
	int current = 0;
	if(directory == NULL){
		printf("Invalid directory for shares.\n");
		return;
	}
	while ((file=readdir(directory)) != NULL && reached != n) {

		if((strcmp("bmp",get_filename_ext(file->d_name)) == 0)){

			reached++;
        	char *result = malloc(strlen(directory_path) + strlen(file->d_name) + 1); // +1 for the null-terminator
		    // in real code you would check for errors in malloc here
		    strcpy(result, directory_path);
		    strcat(result, file->d_name);
		    printf("Using share %s\n", result);
			hide_data(shadows[current],result,lsb, current);
			printf("out\n");
			current++;

		}

	}

	if(reached != n){

		printf("Not enough files for shares.\n");
		return;

	}

	

	/* guardo en archivo las 8 shares*/

	/* calculo Rw que es watermark - secret_projection_extended */
	uint8_t * watermark_data = bmp_get_data_buffer8(wimage);
	uint8_t * new_watermark_data = calloc(secret_size, 1);
	int rw_index = 0;
	for(rw_index =0; rw_index < secret_size; rw_index++){
				int aux = (watermark_data[rw_index] - secret_projection_extended[rw_index]);
				if(aux < 0){
					aux = aux + 251;
				}
				new_watermark_data[rw_index] = aux;
	}

	/* Guardo Rw en archivo */
	bmp_image_t8 *final_rw = wimage;
	final_rw->data = new_watermark_data;
	bmp_save8(final_rw, watermark_destiny_path);
	printf("Watermark saved %s.\n", watermark_destiny_path);



}