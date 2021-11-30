#include "irinlithiumImage.h"

png_byte *pixelAt(bitmap_t *bitmap, png_uint_32 x, png_uint_32 y) {
	if (!bitmap) return 0;
	return &((bitmap->row_pointers[y >= bitmap->height ? bitmap->height - 1 : y])[(x >= bitmap->width ? bitmap->width - 1 : x) * 4]);
}

bitmap_t loadPNG(const char *path) {
	bitmap_t bitmap;
	FILE *fp = fopen(path, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	
	bitmap.width = png_get_image_width(png, info);
	bitmap.height = png_get_image_height(png, info);
	bitmap.color_type = png_get_color_type(png, info);
	bitmap.bit_depth = png_get_bit_depth(png, info);
	bitmap.row_pointers = NULL;

	
	if (bitmap.bit_depth == 16)
		png_set_strip_16(png);

	if (bitmap.color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	if (bitmap.color_type == PNG_COLOR_TYPE_GRAY && bitmap.bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	if (bitmap.color_type == PNG_COLOR_TYPE_RGB || bitmap.color_type == PNG_COLOR_TYPE_GRAY || bitmap.color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if(bitmap.color_type == PNG_COLOR_TYPE_GRAY || bitmap.color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);


	png_read_update_info(png, info);

	if (bitmap.row_pointers) abort();

	bitmap.row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * bitmap.height);
	for (png_uint_32 y = 0; y < bitmap.height; y++)
		bitmap.row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));

	png_read_image(png, bitmap.row_pointers);

	fclose(fp);

	png_destroy_read_struct(&png, &info, NULL);

	return bitmap;
}

void savePNG(bitmap_t *bitmap, const char *path) {
	if (!bitmap) return;
	FILE *fp = fopen(path, "wb");
	if (!fp) abort();

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) abort();

	png_infop info = png_create_info_struct(png);
	if (!info) abort();

	if (setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_set_IHDR(png, info, bitmap->width, bitmap->height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	//png_set_filler(png, 0, PNG_FILLER_AFTER);

	if (!bitmap->row_pointers) abort();

	png_write_image(png, bitmap->row_pointers);
	png_write_end(png, NULL);

	fclose(fp);

	png_destroy_write_struct(&png, &info);
}

void destroyBitmap(bitmap_t *bitmap) {
	if (!bitmap) return;
	for (png_uint_32 y = 0; y < bitmap->height; y++)
		free(bitmap->row_pointers[y]);

	free(bitmap->row_pointers);
	bitmap->width = 0;
	bitmap->height = 0;
	bitmap->bit_depth = 0;
	bitmap->color_type = 0;
}

bitmap_t createBitmap(png_uint_32 width, png_uint_32 height, int color_type, int bit_depth) {
	bitmap_t bitmap;
	bitmap.width = width;
	bitmap.height = height;
	bitmap.bit_depth = bit_depth;
	bitmap.color_type = color_type;

	bitmap.row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * bitmap.height);
	for(png_uint_32 y = 0; y < bitmap.height; y++)
		bitmap.row_pointers[y] = (png_byte*)malloc(sizeof(png_byte) * (bitmap.width * 4));

	return bitmap;
}

void replaceByColor(bitmap_t *bitmap, png_byte *frompx, png_byte *topx) {
	if (!bitmap || !frompx || !topx) return;
	for (png_uint_32 y = 0; y < bitmap->height; y++) {
		for (png_uint_32 x = 0; x < bitmap->width; x++) {
			png_byte *px = pixelAt(bitmap, x, y);
			if (px[0] == frompx[0] && px[1] == frompx[1] && px[2] == frompx[2]) {
				px[0] = topx[0];
				px[1] = topx[1];
				px[2] = topx[2];
			}
		}
	}
}

void layerImage(bitmap_t *base, bitmap_t *overlay, png_uint_32 start_x, png_uint_32 start_y) {
	for (png_uint_32 y = 0; y < overlay->height && y < base->height; y++) {
		for (png_uint_32 x = 0; x < overlay->width && x < base->width; x++) {
			png_byte *px = pixelAt(overlay, x, y);
			png_byte *basepx = pixelAt(base, x + start_x >= base->width ? base->width : x + start_x, y + start_y >= base->height ? base->height : y + start_y);
			if (!px[3]) continue;
			basepx[0] = px[0];
			basepx[1] = px[1];
			basepx[2] = px[2];
			basepx[3] = px[3];
		}
	}
}

void flipPixels(bitmap_t *bitmap) {
	if (!bitmap) return;
	for (png_uint_32 y = 0; y < bitmap->height; y++) {
		for (png_uint_32 x = 0; x < bitmap->width; x++) {
			png_byte *px = pixelAt(bitmap, x, y);
			px[0] = ~px[0];
			px[1] = ~px[1];
			px[2] = ~px[2];
		}
	}
}

//Work-In-Progress
void resizeImage(bitmap_t *bitmap, png_uint_32 width, png_uint_32 height) {
	if (!bitmap) return;
	//bitmap->row_pointers = (png_bytep*)remalloc(sizeof(png_bytep) * bitmap->height);

	bitmap->width = width;
	bitmap->height = height;
}
