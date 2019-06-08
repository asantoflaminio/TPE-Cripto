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
	char* directory_path = "./Archivos de Prueba-4-8/shares/";
  //lo tendria q hacer por cada sombra
  //por cada cosa en el directorio q me pasan...
	DIR *directory;
	directory = opendir(directory_path); //esto me lo va a pasar el usuario *el path*
	struct dirent* file;
	int reached = 0;
	int rows;
	int cols;
	uint8_t shadows[k][330][210];//seria lo mas grande  q voy a tener, de ambos.
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

	/*
	shadows a vector de uint8_t. Entonces hay q revisar todas las operaciones a ver q onda si cambia algo.
	y sino cambiar todas las funciones en matrix_functions tranquilamente. OJO q alejo dijo algo de los negativos
	Mejor intentar pasar todo a int y operar con ints
	fijarse como pasar vector de uint8_t  a int
	*/

	while ((file=readdir(directory)) != NULL && reached != k) {
		size_t index = 0;
        printf("%s\n", file->d_name);
        uint8_t * sh;
        if(strcmp("bmp",get_filename_ext(file->d_name)) == 0){
        	char *result = malloc(strlen(directory_path) + strlen(file->d_name) + 1); // +1 for the null-terminator
		    // in real code you would check for errors in malloc here
		    strcpy(result, directory_path);
		    strcat(result, file->d_name);
            bmp_image_t *image = bmp_from_path(result);
			sh = (uint8_t *) stegobmp_extract(image, "prueba_output.bmp", "LSB1"); 

			for(int i=0; i < rows; i++){
				for(int j = 0; j< cols;j++){
					//printf("reached %d, i %d, j %d\n", reached,i,j);
					shadows[reached][i][j] = sh[index];
					//printf("sh es %d\n", sh[index]);
					index++;
				}
			}
			reached++;
			index = 0;
        }
        

	}

		
	closedir(directory);
    
	/*
    printf("shadow es\n");

	int i;
	int j;
	for (i=0; i<rows; i++){
		    for(j=0; j<cols; j++){
		        printf("%d  ", shadows[0][i][j]);
		    }
		    printf("\n");
	}*/

	// faltaria la watermark

	/*
	Obtengo la matriz B que se obtiene concatenando las primeras columnas de todas las shadows.
	rows va a ser igual a las filas de las shadows
	*/
	/*
	int b_matrix[rows][k];
	int v[k][rows][1]; //matrices v
	int g[k][rows][cols-1]; //matrices g

	for(int counter=0; counter < k ;counter++){
		
		separateMatrixByColumn(1, cols-1, rows, cols,shadows[counter], v[counter], g[counter]);
	}
	if(k==2){
		concat (rows, 1,1, v[0], v[1], b_matrix);
	}else{
		//k == 4
		
		int curr1[rows][2];
		concat (rows, 1,1, v[0], v[1], curr1);		
		int curr2[rows][3];
		concat (rows, 2,1, curr1, v[2], curr2);	
		concat (rows, 3,1, curr2, v[3], b_matrix);

	}

	/*
	ahroa vamos a obtener secret_projection que surge de poryeccion de B
	*/
	//printMatrix(rows,k,b_matrix);
	//int secret_projection[rows][rows]; //no estoy seguro de si el tamaño es igual a rows de B
	//calculateProjection(rows, k, b_matrix, rows, rows, secret_projection); //ARREGLAR ESTO TIRA EXCEPCION. CHEQUEAR Q B ESTE BIEN ARMADA!
																			// al hacer Bt * B nos deberia dar cuadrada!

}

