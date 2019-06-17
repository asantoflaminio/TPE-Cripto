#ifndef __matrixfunctions__
#define __matrixfunctions__

/* 
*	Answers for matrix functions are always placed inside an answer matrix received as a parameter.
*	All operations are done using modular arithmetic with modulo 251.
*/

/* Matrix addition */
void add (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);

/* Substracts m1 - m2*/
void substract (size_t rows, size_t columns, int m1[rows][columns], int m2[rows][columns], int answer[rows][columns]);

/* Calculates transpose for matrix m */
void transpose (size_t rows, size_t columns, int m[rows][columns],  int answer[columns][rows]);

/* Multiplies two matrices */
void multiply (size_t rows1, size_t columns1, size_t rows2, size_t columns2 ,int m1[rows1][columns1], int m2[rows2][columns2], int answer[rows1][columns2]);

/* Merges two matrices */
void concat (size_t rows, size_t columns1,size_t columns2, int m1[rows][columns1], int m2[rows][columns2], int answer[rows][columns1+columns2]);

/* Prints a matrix with int values */
void printMatrix(size_t rows, size_t columns, int m[rows][columns]);

/* Prints a matrix with float values */
void printMatrixFloat(size_t rows, size_t columns, float m[rows][columns]);

/* Calculates inverse for matrix */
void inverse(size_t size, int m[size][size], int answer[size][size]);

/* Calculates cofactor for matrix */
void cofactor(int size, int m[size][size], int ans[size][size], int forbidden_row, int forbidden_col);

/* Calculates determinant for matrix */
int determinant(size_t size, int m[size][size]);

/* Calculates adjoint matrix */
void adjoint(int size, int m[size][size],int answer[size][size]);

/* Gauss Jordan solver. Accepts only matrices with the scheme m[rows][rows+1] with the last column being the values
of the matrix the other side of the 'equal' symbol */
void gauss_jordan(int rows, int m[rows][rows+1], int** answer);

/* Calculates modular inverse for number a using modulo m */
int mod_inverse(int a, int m);

/* Calculates matrix projection */
void calculate_projection(int a_rows, int a_cols, int a_matrix[a_rows][a_cols], int rows, int cols, int answer[rows][cols]);

/* Separates matrix by columns */
void separate_matrix_by_column(size_t wanted_cols1, size_t wanted_cols2, size_t rows, size_t columns, int m[rows][columns], int answer1[rows][wanted_cols1], int answer2[rows][wanted_cols2]);

/* Calculates rank for matrix */
size_t calculate_rank(int rows, int columns, int matrix[rows][columns]);

/* Returns 1 if the matrix has all different values. Otherwise it returns 0. */
int check_all_different(int rows, int cols, int m[rows][cols]);
#endif