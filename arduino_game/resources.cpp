
#include "resources.h"

const uint8_t terrain_1[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 34, 34, 34, 34, 34, 34, 34, 34, 24, 1, 1, 1, 1, 1, 1, 28, 34, 34, 34, 34, 34, 34, 24, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 28, 34, 34, 34, 33, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 28, 13, 13, 13, 14, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 15, 22, 22, 22, 22, 22, 22, 16, 29, 1, 1, 28, 13, 13, 13, 14, 15, 22, 22, 22, 22, 16, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 28, 13, 13, 13, 14, 19, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 28, 15, 22, 22, 22, 23, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 31, 26, 27, 0, 0, 0, 0, 0, 18, 29, 1, 1, 28, 4, 4, 4, 3, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 17, 0, 0, 0, 0, 18, 29, 1, 1, 28, 10, 10, 7, 8, 17, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 18, 29, 1, 1, 28, 13, 13, 7, 6, 4, 3, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 18, 32, 34, 34, 33, 15, 16, 7, 8, 10, 11, 17, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 18, 12, 13, 13, 14, 19, 18, 7, 8, 13, 14, 19, 0, 0, 18, 29, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 34, 33, 19, 0, 0, 0, 0, 18, 12, 13, 13, 14, 19, 18, 9, 11, 15, 22, 23, 0, 0, 18, 32, 34, 24, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 14, 19, 0, 0, 0, 0, 18, 12, 13, 13, 14, 19, 18, 12, 14, 19, 0, 0, 0, 0, 18, 12, 13, 29, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 15, 22, 23, 0, 0, 0, 0, 20, 21, 22, 22, 22, 23, 20, 21, 22, 23, 0, 0, 0, 0, 20, 21, 16, 29, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 31, 26, 27, 0, 0, 0, 0, 25, 26, 26, 26, 26, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 0, 0, 0, 0, 29, 1, 1, 1, 1, 28, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 31, 26, 26, 26, 26, 30, 1, 1, 1, 1, 31, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 30, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};


const uint8_t terrain_2[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 34, 34, 34, 34, 34, 34, 34, 34, 24, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 34, 34, 34, 34, 34, 34, 34, 33, 13, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 14, 13, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 14, 15, 22, 22, 22, 22, 22, 22, 16, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 13, 13, 13, 13, 13, 14, 19, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 15, 22, 22, 22, 22, 22, 22, 22, 23, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 25, 26, 26, 26, 26, 27, 0, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 18, 32, 34, 34, 34, 29, 28, 0, 0, 0, 0, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 18, 12, 13, 13, 13, 29, 31, 26, 26, 26, 26, 26, 26, 26, 26, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 18, 12, 13, 13, 13, 32, 34, 34, 34, 34, 34, 34, 34, 24, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 18, 12, 13, 13, 13, 12, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 20, 21, 22, 22, 16, 12, 13, 13, 13, 13, 13, 13, 13, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 18, 12, 13, 13, 13, 13, 13, 2, 4, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 20, 21, 22, 22, 22, 22, 16, 7, 0, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 18, 7, 0, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 31, 26, 26, 26, 27, 0, 18, 7, 8, 17, 0, 0, 18, 7, 0, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 34, 34, 24, 28, 17, 18, 7, 8, 19, 0, 0, 18, 7, 0, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 29, 28, 19, 18, 7, 8, 19, 0, 0, 18, 7, 0, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 13, 13, 32, 33, 19, 18, 7, 8, 19, 0, 0, 18, 9, 11, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 15, 16, 12, 14, 19, 18, 7, 8, 19, 0, 0, 20, 21, 16, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 18, 12, 14, 19, 18, 9, 11, 2, 3, 0, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 20, 21, 22, 23, 20, 21, 16, 7, 8, 17, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 0, 0, 18, 9, 11, 19, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 28, 19, 0, 0, 0, 0, 0, 0, 20, 21, 22, 23, 0, 0, 18, 29, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 31, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};


int terrain_color_table_length = 5;
uint16_t terrain_color_table[] = {
    0xdedb,
    0xb596,
    0x94d2,
    0x632c,
    0x4a69,
    0x0,
    0xffff,
    0x5acb,
    0x738e,
    0x8410,
    0xce95,
    0xbe31,
    0xb5cd,
    0x8c8a,
    0x73a8,
    0x0,
    0xdf19,
    0x7c09,
    0x94eb,
    0xa54b,
    0xcedc,
    0xa5b8,
    0x7cd6,
    0x6bf2,
    0x3a6c,
    0x0,
    0xffff,
    0x42ee,
    0x5bb2,
    0x6413,
    0xd595,
    0xc470,
    0xbb8c,
    0x9aa9,
    0x7a06,
    0x0,
    0xde99,
    0x8247,
    0xa2a9,
    0xaae9,
    0xde5a,
    0xcd78,
    0xbcb6,
    0xb415,
    0x9af1,
    0x7a0d,
    0xe71c,
    0xa312,
    0xabb4,
    0xb415
};

const uint8_t terrain_tiles_indexed[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 3, 3, 3, 3, 3, 3, 
	5, 5, 3, 3, 3, 3, 3, 3, 
	5, 5, 3, 3, 3, 3, 3, 3, 
	5, 5, 3, 3, 3, 3, 3, 3, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	3, 3, 3, 3, 3, 3, 5, 5, 
	3, 3, 3, 3, 3, 3, 5, 5, 
	3, 3, 3, 3, 3, 3, 5, 5, 
	3, 3, 3, 3, 3, 3, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	5, 5, 1, 1, 1, 1, 1, 1, 
	5, 5, 1, 1, 1, 1, 1, 1, 
	5, 5, 1, 1, 1, 1, 1, 1, 
	5, 5, 3, 1, 1, 1, 1, 1, 
	5, 5, 3, 3, 1, 1, 1, 1, 
	5, 5, 3, 3, 3, 1, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 5, 5, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	1, 1, 1, 3, 3, 3, 3, 3, 
	1, 1, 1, 1, 3, 3, 3, 3, 
	1, 1, 1, 1, 1, 3, 3, 3, 
	1, 1, 1, 1, 1, 1, 3, 3, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	5, 5, 3, 3, 3, 3, 1, 1, 
	3, 3, 3, 3, 3, 3, 1, 1, 
	3, 3, 3, 3, 3, 3, 1, 1, 
	3, 3, 3, 3, 3, 3, 1, 1, 
	3, 3, 3, 3, 3, 3, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 
	6, 9, 9, 9, 9, 9, 9, 9, 
	6, 9, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	6, 7, 7, 7, 7, 7, 7, 7, 
	6, 7, 7, 7, 7, 7, 7, 7, 
	6, 8, 8, 8, 8, 8, 8, 8, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 
	8, 8, 8, 8, 8, 8, 8, 8, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	7, 7, 7, 7, 7, 7, 7, 6, 
	7, 7, 7, 7, 7, 7, 7, 6, 
	8, 8, 8, 8, 8, 8, 8, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	6, 8, 9, 9, 9, 9, 9, 9, 
	7, 8, 9, 9, 9, 9, 9, 9, 
	7, 8, 9, 9, 9, 9, 9, 9, 
	8, 8, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 6, 
	9, 9, 9, 9, 9, 9, 9, 7, 
	9, 9, 9, 9, 9, 9, 9, 7, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	9, 9, 9, 9, 9, 9, 9, 9, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	2, 4, 4, 4, 4, 4, 4, 4, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	4, 4, 4, 4, 4, 4, 4, 2, 
	6, 6, 6, 6, 6, 6, 6, 6, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 
};


const Entity entities_01[] = {
    {plr_t, 16, 6, 2, 0, 0, &plr_behaviour, &sprite_prof_stationary_west},
    // {officer_t, 8, 8, 2, 0, 0, NULL , NULL},
    // {office_chair_t, 7, 7, 2, 0, 0, NULL , NULL},

    // {desktop_terminal_t, 12, 7, 4, 0, 0, NULL , NULL},
    // {dotpanel_right_t, 13, 9, 4, 0, 0, NULL , NULL},
    // {inbuilt_terminal_t, 14, 8, 4, 0, 0, NULL , NULL},
    // {desk_clutter_t, 13, 8, 4, 0, 0, NULL , NULL},

    {target_t, 12, 7, 1, 0, 0, NULL, &sprite_target},
    {target_t, 16, 8, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 12, 9, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 15, 10, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 14, 11, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 17, 11, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 15, 12, 1, 0, 0, NULL, &sprite_target},

    {crate_t, 15, 7, 2, 0, 0, &crate_behaviour, &sprite_crate},
    // {crate_t, 15, 8, 2, 0, 0, &crate_behaviour, &sprite_crate},
    // {crate_t, 15, 9, 2, 0, 0, &crate_behaviour, &sprite_crate},
    // {crate_t, 12, 11, 2, 0, 0, &crate_behaviour, &sprite_crate},
    // {crate_t, 14, 11, 2, 0, 0, &crate_behaviour, &sprite_crate},
    // {crate_t, 15, 11, 2, 0, 0, &crate_behaviour, &sprite_crate},
    // {crate_t, 16, 11, 2, 0, 0, &crate_behaviour, &sprite_crate},
};
const uint8_t entities_01_length = sizeof(entities_01) / sizeof(Entity);

const Entity entities_02[] = {
    {plr_t, 6, 7, 2, 0, 0, &plr_behaviour, &sprite_prof_stationary_east},
    // {strongman_t,     11, 10, 2, 0, 0, NULL , NULL},
    // {office_chair_t,  11, 8, 2, 0, 0, NULL , NULL},
    // {powerconverter_t, 4, 11, 2, 0, 0, &powerconverter_behaviour, &sprite_powerconverter},
    // {powerconverter_t, 8, 8, 2, 0, 0, &powerconverter_behaviour, &sprite_powerconverter},
    // {powerconverter_t, 7, 12, 2, 0, 0, &powerconverter_behaviour, &sprite_powerconverter},
    // {powerconverter_t, 10, 13, 2, 0, 0, &powerconverter_behaviour, &sprite_powerconverter},
    // {target_t, 6, 11, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 6, 12, 1, 0, 0, NULL, &sprite_target},
    // {target_t, 6, 13, 1, 0, 0, NULL, &sprite_target},
};
const uint8_t entities_02_length = sizeof(entities_02) / sizeof(Entity);


// Collate all terrains and entities
const EnvSpec environmentList[] = {
    {terrain_1, entities_01, 1, sizeof(entities_01) / sizeof(Entity)},
    {terrain_2, entities_02, 2, sizeof(entities_02) / sizeof(Entity)},
};
