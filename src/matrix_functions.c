#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "matrix_functions.h"
// NO se si vamos a tener matrices de tipo int o que
// el tema es q leemos y guardamos como punteros
// no se bien como hadnlear eso desps


// este main es solo para testear esto
/*
int main (void){
	int a[4][2] = {{3,7}, {6,1}, {2,5}, {6,6}};
	int at[2][4];
	transpose(4,2,a,at);
	int inv[2][2] ;//= {{110,147},{147,238}};
	int ata[4][2]; //at*a
	multiply(2,4,4,2, at, a, ata); // a * inv
	inverse(2, ata, inv);
	printf("Ok la inversa es\n");
	printMatrix(2, 2, inv);
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
}
*/

void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){

	int i;
	int j;

	for(i=0;i<rows;++i)
	{
		for(j=0;j<columns;++j)
		{
			answer[i][j]=(m1[i][j]+m2[i][j])%251;
			//printf("%d ",answer[i][j]);
		}
		
		//printf("\n");
	}

}

void substract (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){

	int i;
	int j;

	for(i=0;i<rows;++i)
	{
		for(j=0;j<columns;++j)
		{
			answer[i][j]=(m1[i][j]-m2[i][j]);
			if(answer[i][j] < 0){
				answer[i][j] = answer[i][j] + 251;
			}
			//printf("%d ",answer[i][j]);
		}
		
		//printf("\n");
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

void multiply (size_t rows1, size_t columns1, size_t rows2, size_t columns2 ,int m1[rows1][columns1], int m2[rows2][columns2], int answer[rows1][columns2]){
	//columns1 y rows2 deben ser iguales!
	if(columns1 != rows2){
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
        answer[i][j] = sum;
        sum = 0;
      }
    }
	
}

void concat (size_t rows, size_t columns1, size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][columns1+columns2]){
	int i;
	int j;
	int aux;

	for(i=0;i<rows;i++){
		for(j=0; j<columns1;j++){
			answer[i][j] = m1[i][j];
		}
		aux = j;
		for(j=0; j<columns2;j++){
			answer[i][aux] = m2[i][j];
			aux++;
		}
	}
}

void printMatrix(size_t rows, size_t columns, int m[rows][columns]){
	int i;
	int j;
	for (i=0; i<rows; i++){
		    for(j=0; j<columns; j++){
		        printf("%d  ", m[i][j]);
		    }
		    printf("\n");
	}
}

void printMatrixFloat(size_t rows, size_t columns, float m[rows][columns]){
	int i;
	int j;
	for (i=0; i<rows; i++){
		    for(j=0; j<columns; j++){
		        printf("%f  ", m[i][j]);
		    }
		    printf("\n");
	}
}

void inverse(size_t size, int m[size][size], int answer[size][size]){


    int det = determinant(size, m); 
    if (det == 0) { 
        printf("Invalid matrix. Doesn't have inverse.");
        return;
    } 
    
    int adjMatrix[size][size]; 
    adjoint(size, m, adjMatrix); 
  
    for (int i=0; i<size; i++) {
    	for (int j=0; j<size; j++){
        	answer[i][j] = ((adjMatrix[i][j]%251)*mod_inverse(det, 251))%251; 

        	if(answer[i][j] < 0){
				answer[i][j] = answer[i][j] + 251;
			}
        } 
          
    }

}

int mod_inverse(int a, int m) 
{ 
    a = a%m; 
    for (int x=1; x<m; x++) 
       if ((a*x) % m == 1) 
          return x; 
} 



void cofactor(int size, int m[size][size], int ans[size][size], int forbidden_row, int forbidden_col) { 
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
  
  	// aux es para guardar los cofactors
    int aux[size][size]; 
    int sign = 1;  
  
    for (i = 0; i < size; i++) { 
        cofactor(size, m, aux, 0, i); 
        det = (det + sign * (m[0][i] * determinant(size - 1, aux))%251)%251; 
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
    int aux[size][size];
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

	//free(*answer);

   // *answer = malloc(rows * sizeof(float));

	int i,j,k;
    int c;
    int aux[rows][rows+1];
    //copio la matriz
    for(i=0; i<rows;i++){
    	for(j=0;j<(rows+1);j++){
    		aux[i][j] = m[i][j];
    	}
    }

    for(j=0; j<rows; j++)
    {
        for(i=0; i<rows; i++)
        {
            if(i!=j)
            {
                c=(aux[i][j] * mod_inverse(aux[j][j], 251))%251 ;
                //printf("c es %f con i %d y j %d \n", c, i, j);
                for(k=0; k<(rows+1); k++)
                {
                    aux[i][k]=aux[i][k]-(c*aux[j][k])%251;
                }
            }
        }
    }

    for(i=0; i<rows; i++)
    {
    	
        (*answer)[i]=(aux[i][rows] * mod_inverse(aux[i][i], 251))%251;

    }
}