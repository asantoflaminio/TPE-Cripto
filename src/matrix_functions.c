#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "matrix_functions.h"
// NO se si vamos a tener matrices de tipo int o que
// el tema es q leemos y guardamos como punteros
// no se bien como hadnlear eso desps


// este main es solo para testear esto
int main (void){
	int m1[2][2] = {{2,2}, {2,2}};
	int m2[2][2] = {{2,2}, {2,2}};
	int ans[2][2];
	printf("Sumamos\n");
	add(2,2, m1, m2, ans);
	printf("---------\n");
	printf("quiero chequear. si da 4 esta ok. dio --> %d\n", ans[1][1]);
	printf("---------\n");
	printf("Restamos\n");
	substract(2,2, m1, m2, ans);
}

void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){

	int i;
	int j;

	for(i=0;i<rows;++i)
	{
		for(j=0;j<columns;++j)
		{
			answer[i][j]=(m1[i][j]+m2[i][j])%251;
			printf("%d ",answer[i][j]);
		}
		
		printf("\n");
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
			printf("%d ",answer[i][j]);
		}
		
		printf("\n");
	}

}

void transpose (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){
	
}

void multiply (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){
	
}

void concat (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]){

}