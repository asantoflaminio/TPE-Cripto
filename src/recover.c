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

int resta(int a, int b){
	int ans = a - b;
	if(ans < 0){
		ans = ans + 251;
	}
	return ans;
}

void recover(int k, int n){ //, image_t* output_image, image_t* watermark_image){
	char* steg_type = "LSB1";

	// esto viene del main pasado!
	char* secret_path = "deberia_ser_albert.bmp";
	// char* watermark_path = "./Archivos de Prueba-4-8/shares/RW/RW.bmp";
	// char* directory_path = "./Archivos de Prueba-4-8/shares/"; 
	char* watermark_path = "generated_watermark.bmp";
	//char* directory_path = "./test_shares/"; 
	//char* directory_path = "./test24/";
	char* directory_path = "./48/";

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
			// printf("ERA \n");
			// printMatrix(n,3,aux);
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
			
			if(k == 4){

				int g00 = g[0][r_row][0];
				int g10 = g[1][r_row][0];
				int g20 = g[2][r_row][0];
				int g30 = g[3][r_row][0];
				int g01 = g[0][r_row][1];
				int g11 = g[1][r_row][1];
				int g21 = g[2][r_row][1];
				int g31 = g[3][r_row][1];

				//printf("imprimo %d, %d, %d, %d, %d, %d, %d, %d\n", g00, g10, g20, g30, g01,g11, g21, g31);

				// int h = (177*(resta(g31 , g01) + resta((3*g11)%251 , (3*g21)%251)))%251;
				// int g =  (126*(g21 + resta( resta(g01 , (2*g11)%251) , (12*h)%251 )))%251;		
				// int f = (resta(g11 , resta(g01 , resta((3*g)%251 , (7*h)%251))))%251;
				// int e = (resta(g01,resta(f,resta(g,h))))%251;

				// int d =  (177*(resta(g30 , g00) + resta((3*g10)%251 , (3*g20)%251)))%251;
				// int c =  (126*(g20 + resta( resta(g00 , (2*g10)%251) , (12*d)%251 )))%251;		
				// int b =  (resta(g10 , resta(g00 , resta((3*c)%251 , (7*d)%251))))%251;
				// int a = (resta(g00,resta(b,resta(c,d))))%251;

				// int h = (177*(g31 - g01 + 3*g11 - 3*g21))%251;
				// int g =  (126*(g21 +  g01 - 2* g11 - 12*h ))%251;		
				// int f = (g11 - g01 - 3*g - 7*h)%251;
				// int e = (g01-f-g-h)%251;

				// int d =  (177*(g30 - g00 + 3*g10 - 3*g20))%251;
				// int c =  (126*(g20 +  g00 - 2* g10 - 12*d ))%251;		
				// int b =  (g10 - g00 - 3*c - 7*d)%251;
				// int a = (g00-b-c-d)%251;

				int m[4][4] = {{1,1,1,1},{1,2,4,8},{1,3,9,27}, {1,4,16,64}};
				int inversaM[4][4];
				inverse(4,m, inversaM);
				int g0[4][1] = {g00,g10,g20,g30};
				int g1[4][1] = {g01,g11,g21,g31};
				int answer[4][1];
				int answer2[4][1];
				multiply(4,4,4,1,inversaM,g0,answer);
				// printf("Imprimo inversa\n");
				// printMatrix(4, 4, inversaM);
				multiply(4,4,4,1,inversaM,g1,answer2);

				
				r_matrix[r_row][0] = answer[0][0];
				r_matrix[r_row][1] = answer[1][0];
				r_matrix[r_row][2] = answer[2][0];
				r_matrix[r_row][3] = answer[3][0];
				r_matrix[r_row][4] = answer2[0][0];
				r_matrix[r_row][5] = answer2[1][0];
				r_matrix[r_row][6] = answer2[2][0];
				r_matrix[r_row][7] = answer2[3][0];
				// r_matrix[r_row][0] = a;
				// r_matrix[r_row][1] = b;
				// r_matrix[r_row][2] = c;
				// r_matrix[r_row][3] = d;
				// r_matrix[r_row][4] = e;
				// r_matrix[r_row][5] = f;
				// r_matrix[r_row][6] = g;
				// r_matrix[r_row][7] = h;

				// for(int neg = 0; neg < 8; neg++){
				// 	if(r_matrix[r_row][neg] < 0){
				// 		r_matrix[r_row][neg] = r_matrix[r_row][neg] + 251;
				// 	}
				// }

			}else{
				// k == 2
				//TODO

				int g00 = g[0][r_row][0];
				int g10 = g[1][r_row][0];
				int g01 = g[0][r_row][1];
				int g11 = g[1][r_row][1];

				int m[2][2] = {{1,1},{1,2}};
				int inversaM[2][2];
				inverse(2,m, inversaM);
				int g0[2][1] = {g00,g10};
				int g1[2][1] = {g01,g11};
				int answer[2][1];
				int answer2[2][1];
				multiply(2,2,2,1,inversaM,g0,answer);
				// printf("Imprimo inversa\n");
				// printMatrix(4, 4, inversaM);
				multiply(2,2,2,1,inversaM,g1,answer2);


				r_matrix[r_row][0] = answer[0][0];
				r_matrix[r_row][1] = answer[1][0];
				r_matrix[r_row][2] = answer2[0][0];
				r_matrix[r_row][3] = answer2[1][0];

			}
				

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
	bmp_image_t8 *simage = bmp_from_path8("./Archivos de Prueba-4-8/Secreto.bmp"); //cambiar
	/* esto es la generacion de la secreta*/
	uint8_t* water = bmp_get_data_buffer8(wimage);

	
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



