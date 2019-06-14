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

void recover(int k, int n){ //, image_t* output_image, image_t* watermark_image){
	char* steg_type = "LSB1";
	if(!((k == 2 && n == 4) || (k==4 && n==8))){
		printf("Acceptable pairs are k=2 and n=4 OR k=4 and n=8\n");
		return;
	}
	char* watermark_path = "./Archivos de Prueba-4-8/shares/RW/RW.bmp";
	char* directory_path = "./Archivos de Prueba-4-8/shares/";
  //lo tendria q hacer por cada sombra
  //por cada cosa en el directorio q me pasan...
	DIR *directory;
	directory = opendir(directory_path); //esto me lo va a pasar el usuario *el path*
	struct dirent* file;
	int reached = 0;

	
	int rows = 440;
	int cols =280;
	int share_size = 0;
	int steg_aux = 0;
	if(k==2){
		share_size = 4*3;
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
	int max = 1925; //DESPS CAMBIARLO
	uint8_t shadows[k][max * share_size];

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
            int sharefile_size_decrypted = sharefile_size_encrypted/steg_aux;
            printf("Quantity es %d\n", sharefile_size_decrypted/share_size); 
            previous_quantity = quantity;
            quantity = sharefile_size_decrypted/share_size;
            if(quantity!= 0 && previous_quantity !=0 && previous_quantity !=quantity){
            	printf("Error in shares\n");
            	return;
            }
			sh = (uint8_t *) stegobmp_extract(image, "prueba_output.bmp", "LSB1"); 

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

	uint8_t * secret_projection_extended = calloc(123200, 1);;
	int sp_index = 0;
	for(int curr = 0; curr < quantity; curr++){
		// printf("spindex es %d\n", sp_index);
		int secret_projection[n][n];
		int my_shadows[k][n][3]; // ejemplo si uso 4,8 voy a tneer 4 shadows de 8x3
		for(int curr_k =0 ; curr_k < k; curr_k ++){
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
		ahroa vamos a obtener secret_projection que surge de proyeccion de B
		*/
		calculateProjection(n, k, b_matrix, n, n, secret_projection); 
		// printf("Proyeccion %d\n", curr);
		// printMatrix(n,n, secret_projection);
		// ahora guado los datos en secret_extended para desps sumar a rw

		for(int si = 0; si < n; si++){
			for(int sj=0; sj <n; sj++){
				secret_projection_extended[sp_index] = (uint8_t) secret_projection[si][sj];
				sp_index++;
			}
		}


	}
	printf("sp_index es %d\n", sp_index);
	printf("Done\n");





	/*
	ahora construimos R

	es medio quilombo. se hace  apartir de las matrices G
	hacemos gauss jordan con 

	Matriz de dos columnas, la primera llena de 1 y la segunda empieza en 1 hsta k.
	Mutliplicadoa por los I que quiero saber.
	eso igual a G(x,y) cada una (ver paper es quilombo)
	del resultado de gauss jordan sacamos como vamos llenando R
	*/
	//for(...){
		//creo q conviene ir por cada fila voy a tener m/k resultados de gauss.
		// hay que ver bien q le mando a gauss
		//de esos resultados lleno cada uno de los valores de la fila 
	//}

	int rw[440][280];
	int rw_index = 0;


	bmp_image_t8 *wimage = bmp_from_path8(watermark_path);
	uint8_t* water = bmp_get_data_buffer8(wimage);



	// AL MENOS LO DE ABAJO ANDA OK

	int rw_row;
	int rw_col;
	uint8_t * new_data = calloc(123200, 1);
	for(rw_index =0; rw_index < 123200; rw_index++){
				new_data[rw_index] = (water[rw_index] + secret_projection_extended[rw_index])%251;
	}



	// for (rw_row = 0; rw_row < 440; rw_row ++) {
 //        for (rw_col = 0; rw_col < 280; rw_col ++) {
 //                	rw[rw_row][rw_col] = water[rw_index];
 //                	rw_index++;
 //        }

 //    }


	// int watermark_m[rows][rows];
	// add(rows, rows, secret_projection,rw, watermark_m);
	
	bmp_image_t8 *final_watermark = wimage;
	
	// int aux_index = 0;
	// printf("rows es %d\n", rows);
	// int aux_i = 0;
	// int aux_j = 0;
	// for(aux_i= 0; aux_i < 440; aux_i++){
	// 	for(aux_j = 0; aux_j < 280; aux_j++){
	// 		//printf("mosca\n");
	// 		new_data[aux_index] = rw[aux_i][aux_j];
	// 		aux_index++;
	// 	}
	// }
	// printf("aux_index la quedo en %d y deberia ser 123200\n", aux_index);
	// printf("aux_i la quedo en %d\n", aux_i);
	// printf("aux_j la quedo en %d\n", aux_j);
	final_watermark->data = (uint8_t*)new_data;
	printf("Por guardar\n");
	bmp_save8(final_watermark, "deberia_ser_paris.bmp");
	printf("Guardado deberia_ser_paris.bmp\n");

}

