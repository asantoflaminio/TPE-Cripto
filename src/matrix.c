#include "matrix.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct matrixCDT {
	int **elems;
	size_t rows;
	size_t cols;
	unsigned int base;
	unsigned int *inverses;
} matrixCDT;

const unsigned int inverses[251] = {
	1,   126, 84,  63,  201, 42,  36,  157, 28,  226, 137, 21,  58,  18,  67,
	204, 192, 14,  185, 113, 12,  194, 131, 136, 241, 29,  93,  9,   26,  159,
	81,  102, 213, 96,  208, 7,   95,  218, 103, 182, 49,  6,   216, 97,  106,
	191, 235, 68,  41,  246, 64,  140, 90,  172, 178, 130, 229, 13,  234, 205,
	107, 166, 4,   51,  112, 232, 15,  48,  211, 104, 99,  129, 196, 173, 164,
	109, 163, 177, 197, 91,  31,  150, 124, 3,   189, 108, 176, 174, 110, 53,
	80,  221, 27,  243, 37,  34,  44,  146, 71,  123, 169, 32,  39,  70,  153,
	45,  61,  86,  76,  89,  199, 65,  20,  240, 227, 132, 118, 117, 135, 228,
	195, 179, 100, 83,  249, 2,   168, 151, 72,  56,  23,  116, 134, 133, 119,
	24,  11,  231, 186, 52,  162, 175, 165, 190, 206, 98,  181, 212, 219, 82,
	128, 180, 105, 207, 217, 214, 8,   224, 30,  171, 198, 141, 77,  75,  143,
	62,  248, 127, 101, 220, 160, 54,  74,  88,  142, 87,  78,  55,  122, 152,
	147, 40,  203, 236, 19,  139, 200, 247, 85,  144, 46,  17,  238, 22,  121,
	73,  79,  161, 111, 187, 5,   210, 183, 16,  60,  145, 154, 35,  245, 202,
	69,  148, 33,  156, 244, 43,  155, 38,  149, 170, 92,  225, 242, 158, 222,
	10,  115, 120, 57,  239, 138, 66,  237, 59,  47,  184, 233, 193, 230, 114,
	25,  223, 94,  215, 209, 50,  188, 167, 125, 250};

static void generateInverses(matrixADT matrix);
static int module(int x, int N);
static void getCofactor(const matrixADT restrict mat,
						const matrixADT restrict temp, const int p, const int q,
						const int n);
static void swap(const matrixADT restrict m, const size_t row1,
				 const size_t row2, const size_t col);
static inline int vectorMultiply(const int *restrict vec1,
								 const int *restrict vec2, const int length);
static int determinantOfMatrixRecursive(const matrixADT restrict matrix,
										const size_t n);
static int modularInverse(const matrixADT restrict matrix, const int num);

int main(void){
	int m[4][4] = {{122,13,11,82}, {13,184,182,154}, {11,182,113,21},{82,154,21,76}};
	matrixADT test = createMatrix(4,4,251);
	for (size_t i = 0; i < test->rows; i++){

		for (size_t j = 0; j < test->cols; j++){
			test->elems[i][j] = m[i][j];
		}
	}
	matrixADT inv = createMatrix(4,4,251);
	inv = inverseMatrix(test);
	printMatrix(inv);
}

matrixADT createMatrix(const size_t rows, const size_t cols,
					   const size_t base) {
	if (!rows || !cols)
		return NULL;

	matrixADT matrix = malloc(sizeof(matrixCDT));
//	checkIsNotNull(matrix, "Not enogh memory");

	matrix->rows = rows;
	matrix->cols = cols;
	matrix->base = base;
	// TODO HACER NO MALLOC AHI, HACER STATIC
	matrix->inverses = malloc(matrix->base * sizeof(unsigned int));
//	checkIsNotNull(matrix->inverses, "Not enogh memory");
	generateInverses(matrix);

	matrix->elems = malloc(rows * sizeof(int *));
	//checkIsNotNull(matrix->elems, "Not enogh memory");

	for (size_t i = 0; i < rows; i++) {
		matrix->elems[i] = malloc(cols * sizeof(int));
		//checkIsNotNull(matrix->elems[i], "Not enogh memory");
	}

	//logDebug("Creating a %d x %d matrix.", matrix->rows, matrix->cols);
	return matrix;
}

void destroyMatrix(const matrixADT restrict matrix) {
	for (size_t i = 0; i < matrix->rows; i++) {
		free(matrix->elems[i]);
	}
	free(matrix->elems);
	free(matrix->inverses);
	free(matrix);
}

inline size_t getRows(const matrixADT restrict matrix) {
	return matrix->rows;
}

inline size_t getCols(const matrixADT restrict matrix) {
	return matrix->cols;
}

static void generateInverses(matrixADT matrix) {
	memcpy(matrix->inverses, inverses, matrix->base * sizeof(unsigned int));
}

// TODO: llevar a otra libreria
static int module(const int x, const int N) {
	return (x % N + N) % N;
}

/*
retStatus assignToMatrix(const matrixADT restrict matrix,
						 const int *restrict elems) {
	if (matrix == NULL || elems == NULL)
		return FAIL_RET_STATUS;

	size_t nbytes = matrix->cols * sizeof(int);
	for (size_t i = 0; i < matrix->rows; i++) {
		memcpy(matrix->elems[i], elems + i * matrix->cols, nbytes);
	}
	return SUCCESS_RET_STATUS;
}
*/
void fillRandomMatrix(const matrixADT restrict matrix) {
	//checkIsNotNull(matrix, "Cannot fill a null matrix.");

//	for (size_t i = 0; i < matrix->rows; i++)
//		for (size_t j = 0; j < matrix->cols; j++)
			//matrix->elems[i][j] = module(nextChar(), matrix->base);
}

void fillRandomKRankedMatrix(const matrixADT restrict matrix, const size_t k) {
	// checkIsNotNull(matrix, "Cannot fill a null matrix.");
	// checkGreaterOrEqualsThan(
	// 	matrix->rows, k,
	// 	"Cannot find a matrix with rank bigger than min of dimmensions.");

	int rank;
	//logDebug("Filling matrix with random chars and rank k.");
	do {
		for (size_t i = 0; i < matrix->rows; i++)
			for (size_t j = 0; j < matrix->cols; j++)
			//matrix->elems[i][j] = module(nextChar(), matrix->base);

		rank = rankOf(matrix);
	//	logDebug("Rank is %d", rank);
	} while (rank != k);
}

inline int getElem(const matrixADT restrict m, const size_t row,
				   const size_t col) {
	return m->elems[row][col];
}

inline void assignElem(const matrixADT restrict m, const size_t row,
					   const size_t col, const int elem) {
	m->elems[row][col] = elem;
}

static inline void swap(const matrixADT restrict m, const size_t row1,
						const size_t row2, const size_t col) {
	for (size_t i = 0; i < col; i++) {
		int temp = getElem(m, row1, i);
		assignElem(m, row1, i, getElem(m, row2, i));
		assignElem(m, row2, i, temp);
	}
}

matrixADT copyMatrix(const matrixADT restrict m) {
//	checkIsNotNull(m, "Cannot get a copy of a NULL matrix.");
	matrixADT copy = createMatrix(m->rows, m->cols, m->base);

	size_t nbytes = m->cols * sizeof(int);
	for (size_t i = 0; i < m->rows; i++)
		memcpy(copy->elems[i], m->elems[i], nbytes);

	return copy;
}

size_t rankOf(const matrixADT restrict matrix) {
	//checkIsNotNull(matrix, "You cannot get the rank of NULL");
	//logDebug("Calculating the rank of Matrix");
	size_t rank = matrix->cols;

	matrixADT m = copyMatrix(matrix);

	for (size_t row = 0; row < rank; row++) {
		if (getElem(m, row, row)) {
			for (size_t col = 0; col < matrix->rows; col++) {
				if (col != row) {
					double mult =
						(double) getElem(m, col, row) / getElem(m, row, row);
					for (size_t i = 0; i < rank; i++) {
						int elem =
							getElem(m, col, i) - mult * getElem(m, row, i);
						assignElem(m, col, i, elem);
					}
				}
			}
		}
		else {
			bool reduce = true;
			for (int i = row + 1; i < matrix->rows; i++) {
				if (getElem(m, i, row)) {
					swap(m, row, i, rank);
					reduce = false;
					break;
				}
			}
			if (reduce) {
				rank--;
				for (int i = 0; i < matrix->rows; i++)
					assignElem(m, i, row, getElem(m, i, rank));
			}
			row--;
		}
	}
	destroyMatrix(m);
	return rank;
}

static void getCofactor(const matrixADT restrict mat,
						const matrixADT restrict temp, const int p, const int q,
						const int n) {
	// checkIsNotNull(mat, "Cannot get the cofactor of a NULL matrix");
	// checkIsNotNull(temp, "Cannot get the cofactor of a NULL matrix");

	int i = 0, j = 0;
	for (int row = 0; row < n; row++) {
		for (int col = 0; col < n; col++) {
			if (row != p && col != q) {
				assignElem(temp, i, j++, getElem(mat, row, col));
				if (j == n - 1) {
					j = 0;
					i++;
				}
			}
		}
	}
}

int determinantOfMatrix(const matrixADT restrict matrix) {
	//checkIsNotNull(matrix, "Cannot get the determinat of a NULL matrix.");
	/*checkAreEquals(matrix->rows, matrix->cols,
				   "The col number must be equal to the row number to "
				   "calculate the determinat.");*/

	return determinantOfMatrixRecursive(matrix, matrix->rows);
}

static int determinantOfMatrixRecursive(const matrixADT restrict matrix,
										const size_t n) {
	int det = 0;
	if (n == 1)
		return getElem(matrix, 0, 0);

	matrixADT temp = copyMatrix(matrix);
	int sign	   = 1;
	for (int f = 0; f < n; f++) {
		getCofactor(matrix, temp, 0, f, n);
		det += sign * getElem(matrix, 0, f) *
			   determinantOfMatrixRecursive(temp, n - 1);
		sign = -sign;
	}
	destroyMatrix(temp);
	return det;
}

void scalarMultiplyMatrix(const matrixADT restrict matrix, const int scalar) {
	//checkIsNotNull(matrix, "Cannot get the scalar multiply of a NULL matrix.");
	for (size_t i = 0; i < matrix->cols; i++)
		for (size_t j = 0; j < matrix->rows; j++)
			matrix->elems[i][j] =
				module(matrix->elems[i][j] * scalar, matrix->base);
}

matrixADT transposeMatrix(const matrixADT restrict matrix) {
	//checkIsNotNull(matrix, "Cannot transpose a NULL matrix.");
	matrixADT trans = createMatrix(matrix->cols, matrix->rows, matrix->base);
	for (size_t i = 0; i < trans->rows; i++)
		for (size_t j = 0; j < trans->cols; j++)
			trans->elems[i][j] = matrix->elems[j][i];
	return trans;
}

static inline int vectorMultiply(const int *restrict vec1,
								 const int *restrict vec2, const int length) {
	int sum = 0;
	for (size_t i = 0; i < length; i++)
		sum += vec1[i] * vec2[i];

	return sum;
}

matrixADT multiplyMatrix(const matrixADT restrict m1,
						 const matrixADT restrict m2) {
	// checkIsNotNull(m1, "Cannot multiply null matrix.");
	// checkIsNotNull(m2, "Cannot multiply null matrix.");
	/*checkAreEquals(m1->cols, m2->rows, "Invalid multiply %d x %d with %d x %d.",
				   m1->rows, m1->cols, m2->rows, m2->cols);*/

	matrixADT trans   = transposeMatrix(m2);
	matrixADT product = createMatrix(m1->rows, m2->cols, m1->base);
	for (size_t i = 0; i < product->rows; i++)
		for (size_t j = 0; j < product->cols; j++)
			product->elems[i][j] =
				module(vectorMultiply(m1->elems[i], trans->elems[j], m1->cols),
					   m1->base);

	destroyMatrix(trans);
	return product;
}

static int modularInverse(const matrixADT restrict matrix, const int num) {
	// checkIsNotNull(
	// 	matrix,
	// 	"Cannot calculate de modularInverse of a num with NULL matrix.");
	if (num < 1 || num > matrix->base)
		return -1;
	return matrix->inverses[num - 1];
}

matrixADT inverseMatrix(const matrixADT restrict matrix) {
	// checkIsNotNull(matrix, "Cannot get the inverse of a NULL matrix.");
	int mInv, det = determinantOfMatrix(matrix);
	if (det == 0)
		return NULL; // TODO: use checks

	matrixADT inverse = adjoint(matrix);
	printf("Aca viene la adjunta\n");
	printMatrix(inverse);
	printf("------------------\n");
	mInv			  = modularInverse(inverse, module(det, matrix->base)); // module es (x % N + N) % N
	printf("mInv es %d\n", mInv);
	scalarMultiplyMatrix(inverse, mInv);
	return inverse;
}

matrixADT adjoint(const matrixADT restrict m) {
	//checkAreEquals(m->rows, m->cols, "Matrix adjoint with invalid args.");

	matrixADT adj = createMatrix(m->rows, m->cols, m->base);
	if (m->rows == 1) {
		assignElem(adj, 0, 0, 1);
		return adj;
	}
	int sign	   = 1;
	matrixADT temp = createMatrix(m->rows, m->cols, m->base);

	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			getCofactor(m, temp, i, j, m->rows);

			sign = ((i + j) % 2 == 0) ? 1 : -1;
			int det =
				(sign) * (determinantOfMatrixRecursive(temp, temp->rows - 1));
			assignElem(adj, i, j, det);
		}
	}
	destroyMatrix(temp);
	return adj;
}

matrixADT add(const matrixADT restrict m1, const matrixADT restrict m2) {
	if (m1 == NULL || m2 == NULL)
		return NULL;
	if (m1->rows != m2->rows || m1->cols != m2->cols)
		return NULL;

	matrixADT sub = createMatrix(m1->rows, m1->cols, m1->base);
	for (size_t i = 0; i < m1->rows; i++)
		for (size_t j = 0; j < m1->cols; j++)
			sub->elems[i][j] =
				module(m1->elems[i][j] + m2->elems[i][j], m1->base);

	return sub;
}

matrixADT subtract(const matrixADT restrict m1, const matrixADT restrict m2) {
	//checkIsNotNull(m1, "Cannot subtract a NULL matrix");
	//checkIsNotNull(m2, "Cannot subtract a NULL matrix");
	/*checkAreEquals(m1->rows, m2->rows,
				   "Cannot subtract two matrix with diferent number of rows, "
				   "recived: %zu x %zu and %zu x %zu.",
				   m1->rows, m1->cols, m2->rows, m2->cols);
	checkAreEquals(m1->cols, m2->cols,
				   "Cannot subtract two matrix with diferent number of cols, "
				   "recived: %zu x %zu and %zu x %zu.",
				   m1->rows, m1->cols, m2->rows, m2->cols);*/

	matrixADT sub = createMatrix(m1->rows, m1->cols, m1->base);
	for (size_t i = 0; i < m1->rows; i++)
		for (size_t j = 0; j < m1->cols; j++)
			sub->elems[i][j] =
				module(m1->elems[i][j] - m2->elems[i][j], m1->base);

	return sub;
}

matrixADT projectionMatrix(const matrixADT restrict a) {
	// checkIsNotNull(a, "Cannot project a NULL matrix.");
	matrixADT aTrans = transposeMatrix(a);
	matrixADT m1	 = multiplyMatrix(aTrans, a);
	matrixADT m1Inv  = inverseMatrix(m1);

	if (m1Inv == NULL) {
		destroyMatrix(aTrans);
		destroyMatrix(m1);
		return NULL;
	}

	matrixADT m2 = multiplyMatrix(a, m1Inv);
	matrixADT s  = multiplyMatrix(m2, aTrans);

	destroyMatrix(aTrans);
	destroyMatrix(m1);
	destroyMatrix(m1Inv);
	destroyMatrix(m2);
	return s;
}

matrixADT horizontalCombineMatrix(const matrixADT restrict m1,
								  const matrixADT restrict m2) {
	// checkIsNotNull(m1, "Cannot combine a NULL matrix.");
	// checkIsNotNull(m1, "Cannot combine a NULL matrix.");
	
	matrixADT comb = createMatrix(m1->rows, m1->cols + m2->cols, m1->base);

	for (size_t i = 0; i < comb->rows; i++) {
		memcpy(comb->elems[i], m1->elems[i], m1->cols * sizeof(int));
		memcpy(comb->elems[i] + m1->cols, m2->elems[i], m2->cols * sizeof(int));
	}
	return comb;
}

void printMatrix(const matrixADT restrict matrix) {
	if (matrix != NULL) {
		for (size_t i = 0; i < matrix->rows; i++) {
			for (size_t j = 0; j < matrix->cols; j++)
				printf("%d\t", matrix->elems[i][j]);
			printf("\n");
		}
	}
}

uint8_t *toCharArray(matrixADT m, size_t *size) {
//	checkIsNotNull(m, "Cannot convert to char array a NULL matrix.");
	*size		   = 0;
	uint8_t *array = malloc(m->rows * m->cols * sizeof(uint8_t));

	for (size_t i = 0; i < m->rows; i++)
		for (size_t j = 0; j < m->cols; j++)
			array[*size++] = getElem(m, i, j);
	return array;
}
