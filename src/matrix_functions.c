#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include "matrix_functions.h"

static uint32_t mod                 = 251;

// este main es solo para testear esto

 //int main (void){
// 	int multiB[4][4] = {{122,13,11,82}, {13,184,182,154}, {11,182,113,21},{82,154,21,76}};
// 	int inv[4][4];
// 	inverse(4, multiB, inv);
// 	printf("inversa quedo \n");
// 	printMatrix(4,4, inv);
// 	printf("----------\n");

	// int b_matrix[4][4];
	// int current_columns = 0;
	// int k = 4;
	// int rows = 4;
	// int shadows[4][4][3]={ {{62,36,92},{59,101,58},{43,126,142},{84,26,163}}, {{40,71,132}, {28,169,47}, {28,198,72}, {48,14,150}} ,
	// {{42,46,42},{42,101,58},{43,126,142},{42,26,163}}, {{0,0,0},{0,101,58},{0,126,142},{0,26,163}}};
	// int v[4][4][1];
	// int g[4][4][2];
	// //separate_matrix_by_column(1, 2, 4, 3,shadows[0], v, g);
	// //printMatrix(4, 1, v);
	// //printMatrix(4, 2, g);
	// for(int counter=0; counter < k ;counter++){
	// 	separate_matrix_by_column(1, 2, 4, 3,shadows[counter], v[counter], g[counter]);
	// }
	// if(k==2){
	// 	concat (4, 1,1, v[0], v[1], b_matrix);
	// }else{
	// 	//k == 4
	// 	int curr1[rows][2];
	// 	concat (4, 1,1, v[0], v[1], curr1);
	// 	int curr2[rows][3];
	// 	concat (4, 2,1, curr1, v[2], curr2);
	// 	concat (4, 3,1, curr2, v[3], b_matrix);

	// }
	// printMatrix(4, 4, b_matrix);
	/*	int b[4][2] = {{62,40}, {59,28}, {43,28}, {84,48}};
		int ansproj[4][4];
		calculate_projection(4, 2, b, 4, 4, ansproj);
		printMatrix(4, 4, ansproj);
	*/
	// int a[4][2] = {{3,7}, {6,1}, {2,5}, {6,6}};
	// int at[2][4];
	// transpose(4,2,a,at);
	// int inv2[2][2] ;//= {{110,147},{147,238}};
	// int ata[4][2]; //at*a
	// multiply(2,4,4,2, at, a, ata); // a * inv
	// inverse(2, ata, inv2);
	// printf("Ok la inversa es\n");
	// printMatrix(2, 2, inv2);
	/*
	int mul[4][2];
	multiply(4,2,2,2, a, inv, mul); // a * inv
	printf("1er Resultado es\n");
	printMatrix(4, 2, mul);
	printf("-----------\n");
	int mul2[4][4];
	multiply(4,2,2,4, mul, at, mul2); // (a * i) * at
	printf("Resultado es\n");
	printMatrix(4, 4, mul2);
	/*int m1[2][2] = {{2,2}, {2,2}};
	int m2[2][2] = {{2,2}, {2,2}};
	int ans[2][2];
	printf("Sumamos\n");
	add(2,2, m1, m2, ans);
	printMatrix(2, 2, ans);
	printf("---------\n");
	printf("Restamos\n");
	substract(2,2, m1, m2, ans);
	printMatrix(2, 2, ans);
	printf("---------\n");
	int t1[3][2] = {{1,2}, {3,4}, {5,6}};
	int anst[2][3];
	printf("Trasponemos\n");
	transpose(3,2, t1, anst);
	printMatrix(2, 3, anst);
	printf("---------\n");
	printf("Concatenamos\n");
	int ansc[2][5];
	concat(2, 2, 3, m2, anst, ansc);
	printMatrix(2, 5, ansc);
	printf("---------\n");
	printf("Multiplicamos\n");
	int e1[2][3] = {{1,2,3}, {4,5,6}};
	int e2[3][1] = {{9},{8},{7}};
	int multAnswer[2][1];
	multiply(2,3,3,1, e1,e2,multAnswer);
	printMatrix(2, 1, multAnswer);
	printf("---------\n");
	printf("Invertimos\n");
	int d[2][2] = {{3,4}, {1,2}};	
	float inv[2][2];
	inverse(2, d, inv);
	printMatrixFloat(2, 2, inv);
	printf("---------\n");
	printf("Gauss Jordan\n");
	int gs[2][3] = {{1,1,36}, {1,2,71}};
	int *ansgs = calloc(2, sizeof(int));;
	gauss_jordan(2, gs, &ansgs);
	printf("Rta es %d y %d\n", ansgs[0], ansgs[1]);
	printf("---FIN---\n");
 }*/


void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){

	int i;
	int j;

	for(i = 0;i < rows; ++i) {
		for(j = 0;j < columns; ++j) {

			answer[i][j]=(m1[i][j]+m2[i][j])%251;
			
		}
		
	}

}

void substract (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){

	int i;
	int j;

	for(i = 0;i < rows; ++i) {
		for(j = 0;j < columns; ++j) {

			answer[i][j]=(m1[i][j]-m2[i][j]);

			if(answer[i][j] < 0) {
				answer[i][j] = answer[i][j] + 251;
			}

		}
		
	}

}

void transpose (size_t rows, size_t columns, int m[rows][columns],  int answer[columns][rows]){

	int i;
	int j;

	for (i = 0; i < rows; i++){
        for (j = 0; j < columns; j++){
            answer[j][i] = m[i][j];
        }
	}
}

void multiply (size_t rows1, size_t columns1, size_t rows2, size_t columns2 ,int m1[rows1][columns1], int m2[rows2][columns2], int answer[rows1][columns2]) {
	
	if(columns1 != rows2) {
		printf("Invalid multiplication.\n");
		return;
	}

	int i;
	int j;
	int k;
	int sum = 0;
	for (i = 0; i < rows1; i++) {
      for (j = 0; j < columns2; j++) {
        for (k = 0; k < rows2; k++) {
          	sum = (sum + (m1[i][k]*m2[k][j])%251)%251;
        }
        answer[i][j] = sum%251;
        sum = 0;
      }
    }
	
}

void separate_matrix_by_column (size_t wanted_cols1, size_t wanted_cols2, size_t rows, size_t columns, int m[rows][columns], int answer1[rows][wanted_cols1], int answer2[rows][wanted_cols2]) {

	int i;
	int j;
	int aux;

	if((wanted_cols1+wanted_cols2) != columns){
		printf("Invalid separation.\n");
		return;
	}

	for (i = 0 ; i < rows; i++) {
		for (j = 0; j < wanted_cols1; j++) {
			answer1[i][j] = m[i][j];
		}
		aux = j;
		for (j = 0; j < wanted_cols2; j++) {
			answer2[i][j] = m[i][aux];
			aux++;
		}
	}
}

void concat (size_t rows, size_t columns1, size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][columns1+columns2]) {

	int i;
	int j;
	int aux;

	for (i = 0 ;i < rows; i++) {
		for (j = 0; j < columns1; j++) {
			answer[i][j] = m1[i][j];
		}
		aux = j;
		for (j = 0; j < columns2; j++) {
			answer[i][aux] = m2[i][j];
			aux++;
		}
	}
}

void printMatrix(size_t rows, size_t columns, int m[rows][columns]) {

	int i;
	int j;
	for (i=0; i < rows; i++) {
		    for (j=0; j < columns; j++) {
		        printf("%d  ", m[i][j]);
		    }
		    printf("\n");
	}
}

void printMatrixFloat(size_t rows, size_t columns, float m[rows][columns]) {

	int i;
	int j;
	for (i=0; i < rows; i++){
		    for (j=0; j < columns; j++) {
		        printf("%f  ", m[i][j]);
		    }
		    printf("\n");
	}

}

void inverse(size_t size, int m[size][size], int answer[size][size]){


    int det = determinant(size, m); 

    if (det == 0) { 
        printf("Invalid matrix. Doesn't have inverse.\n");
        printf("Me pasaron\n");
        printMatrix(size,size, m);
        return;
    } 
    
    int adjMatrix[size][size]; 
    adjoint(size, m, adjMatrix); 

    for (int i = 0; i < size; i++) {
    	for (int j = 0; j < size; j++) {
        	answer[i][j] = (((adjMatrix[i][j])*mod_inverse((det % 251 + 251) % 251, 251)) % 251 + 251) % 251;  
        } 
          
    }

}

int mod_inverse(int a, int m) { 

    a = a%m; 
    for (int x=1; x<m; x++) 
       if ((a*x) % m == 1) 
          return x; 
} 



void cofactor(int size, int m[size][size], int ans[size-1][size-1], int forbidden_row, int forbidden_col) { 

    int i = 0;
    int j = 0; 
  	int k;
  	int z;

    for (k = 0; k < size; k++) { 
        for (z = 0; z < size; z++) { 
            if (k != forbidden_row && z != forbidden_col) { 

                ans[i][j++] = m[k][z]; 
                if (j == size - 1) { 
                    j = 0; 
                    i++; 
                } 

            } 
        } 
    } 
} 
  

int determinant(size_t size, int m[size][size]) { 
    int det = 0;  
  	int i;

    if (size == 1) {
        return m[0][0]; 
    }
  

    int aux[size-1][size-1];
    int sign = 1;  

    for (i = 0; i < size; i++) { 

        cofactor(size, m, aux, 0, i); 

        if(sign == -1){
        	det = (det + sign * (m[0][i] * determinant(size - 1, aux))); 
        }else{
        	det = (det + sign * (m[0][i] * determinant(size - 1, aux))); 
        }

        sign = -sign; 
    } 
  
    return det; 
} 


void adjoint(int size, int m[size][size],int answer[size][size]) { 

    if (size == 1) { 
        answer[0][0] = 1; 
        return; 
    } 
  
    int sign = 1;
    int aux[size-1][size-1];
    int i;
    int j; 

    for (i=0; i<size; i++) { 
        for (j=0; j<size; j++) { 
            cofactor(size, m, aux, i, j); 
            sign = ((i+j)%2==0)? 1: -1; 
            answer[j][i] = (sign)*(determinant(size-1,aux)); 
        } 
    } 

} 

// la ultima columna serian los valores G
void gauss_jordan(int rows, int m[rows][rows+1], int** answer) {

	int i,j,k;
    int c;
    int aux[rows][rows+1];

    for (i = 0; i < rows; i++) {
    	for (j = 0; j < (rows+1); j++) {
    		aux[i][j] = m[i][j];
    	}
    }

    for (j=0; j < rows; j++)
    {
        for (i=0; i < rows; i++)
        {
            if (i != j)
            {
                c=(aux[i][j] * mod_inverse(aux[j][j], 251))%251 ;

                for (k=0; k<(rows+1); k++) {
                    aux[i][k]=aux[i][k]-(c*aux[j][k])%251;
                    if (aux[i][k] < 0) {
                    	aux[i][k] = aux[i][k] + 251;
                    }
                }
            }
        }
    }

    for(i = 0; i < rows; i++) {
    	
        (*answer)[i]=(aux[i][rows] * mod_inverse(aux[i][i], 251))%251;
    }
}

size_t calculate_rank(int rows, int columns, int matrix[rows][columns]) {
	size_t rank = columns;
	int m[rows][columns];
	
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			m[i][j] = matrix[i][j];
		}
	}

	for (size_t row = 0; row < rank; row++) {
		if (m[row][row]) {
			for (size_t col = 0; col < rows; col++) {
				if (col != row) {
					double mult = (double) m[col][row] / m[row][row];
					for (size_t i = 0; i < rank; i++) {
						int elem = m[col][i] - (mult * m[row][i]);
						m[col][i] = elem;
					}
				}
			}
		}
		else {
			int reduce = 1;
			for (int i = row + 1; i < rows; i++) {
				if (m[i][row]) {
					for (size_t k = 0; k < rank; k++) {
						int temp = m[row][k];
						m[row][k] = m[i][k];
						m[i][k] = temp;
					}
					reduce = 0;
					break;
				}
			}
			if (reduce) {
				rank--;
				for (int i = 0; i < rows; i++)
					m[i][row] = m[i][rank];
			}
			row--;
		}
	}
	return rank;
}

void calculate_projection(int a_rows, int a_cols, int a_matrix[a_rows][a_cols], int rows, int cols, int answer[rows][cols]) {

	int at[a_cols][a_rows];
	transpose (a_rows, a_cols, a_matrix, at); //ahi obtengo mi transpuesta en at
	
	int multiplication[a_cols][a_cols];
	multiply (a_cols, a_rows, a_rows, a_cols ,at, a_matrix, multiplication); //en multiplication tengo at*a

	int invM[a_cols][a_cols];
	inverse(a_cols, multiplication, invM); //en invm tengo (at*a)^inv

	int multiplication2[a_rows][a_cols];
	multiply(a_rows, a_cols, a_cols, a_cols ,a_matrix, invM, multiplication2); //en multiplication tengo a * invM
	multiply(a_rows, a_cols, a_cols, a_rows,multiplication2, at, answer); //resultado final es multiplication2 * at

}
