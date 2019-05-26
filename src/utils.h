#ifndef __utils__
#define __utils__

#include <stdlib.h>
typedef unsigned char byte;
typedef struct tagImage{
	short first_2_byte;
	int second_4_byte;
	byte* header;
	byte* bytes;
	int size;
	int offset;
	char* id;
	unsigned short hidden_x;
} image_t;

#define IMG_MAX 8
#define HIDEN_X_1 8
#define HIDEN_X_2 9
#define MODULUS 251
//Modular Inverses
static const int inverses[] = {1, 126, 84, 63, 201, 42, 36, 157, 28, 226, 137, 21, 58, 18, 67, 204, 192, 14, 185, 113, 12, 194, 131, 136, 241, 29, 93, 9, 26, 159, 81, 102, 213, 96, 208, 7, 95, 218, 103, 182, 49, 6, 216, 97, 106, 191, 235, 68, 41, 246, 64, 140, 90, 172, 178, 130, 229, 13, 234, 205, 107, 166, 4, 51, 112, 232, 15, 48, 211, 104, 99, 129, 196, 173, 164, 109, 163, 177, 197, 91, 31, 150, 124, 3, 189, 108, 176, 174, 110, 53, 80, 221, 27, 243, 37, 34, 44, 146, 71, 123, 169, 32, 39, 70, 153, 45, 61, 86, 76, 89, 199, 65, 20, 240, 227, 132, 118, 117, 135, 228, 195, 179, 100, 83, 249, 2, 168, 151, 72, 56, 23, 116, 134, 133, 119, 24, 11, 231, 186, 52, 162, 175, 165, 190, 206, 98, 181, 212, 219, 82, 128, 180, 105, 207, 217, 214, 8, 224, 30, 171, 198, 141, 77, 75, 143, 62, 248, 127, 101, 220, 160, 54, 74, 88, 142, 87, 78, 55, 122, 152, 147, 40, 203, 236, 19, 139, 200, 247, 85, 144, 46, 17, 238, 22, 121, 73, 79, 161, 111, 187, 5, 210, 183, 16, 60, 145, 154, 35, 245, 202, 69, 148, 33, 156, 244, 43, 155, 38, 149, 170, 92, 225, 242, 158, 222, 10, 115, 120, 57, 239, 138, 66, 237, 59, 47, 184, 233, 193, 230, 114, 25, 223, 94, 215, 209, 50, 188, 167, 125, 250};
static const char mask_last_bit = 0x01;
static const char mask_no_last_bit = 0xFE;
static const char full_mask = 0xFF;
static const char mask_first_bit = 0x80;
int inverse(int i);
void assure(int truth, char* message);
#endif