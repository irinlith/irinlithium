#ifndef IRINLITHIUM_IMAGE_H
#define IRINLITHIUM_IMAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>
#include <stdarg.h>

typedef struct {
	png_uint_32 width;
	png_uint_32 height;
	int bit_depth;
	int color_type;
	png_byte **row_pointers;
} bitmap_t;

void savePNG(bitmap_t *bitmap, const char *path);
void replaceByColor(bitmap_t *bitmap, png_byte *frompx, png_byte *topx);
bitmap_t loadPNG(const char *path);
png_byte *pixelAt(bitmap_t *bitmap, png_uint_32 x, png_uint_32 y);
void destroyBitmap(bitmap_t *bitmap);
void layerImage(bitmap_t *base, bitmap_t *overlay, png_uint_32 start_x, png_uint_32 start_y);
void flipPixels(bitmap_t *bitmap);
bitmap_t createBitmap(png_uint_32 width, png_uint_32 height, int color_type, int bit_depth);

#endif
