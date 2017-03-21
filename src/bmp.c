#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

unsigned char* BMP_Load(const char* fn);
unsigned char* BMP_Decode(unsigned char* raw);

int i, j;

unsigned char* BMP_Load(const char* fn) {
	FILE* fp;
	unsigned char* image;
	unsigned char* BMP_raw_buffer;
	int size;

	fp = fopen(fn, "rb");
	if (!fp)
		printf("No file\n");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	BMP_raw_buffer = malloc(size * sizeof(char));
	fread(BMP_raw_buffer, 1, size, fp);

	image = BMP_Decode(BMP_raw_buffer);
	free(BMP_raw_buffer);
	fclose(fp);

	return image;
}

unsigned char* BMP_Decode(unsigned char* raw) {
	typedef struct {
		uint16_t magic_number;
		uint32_t size;
		uint32_t pad;
		uint32_t offset;
	} Bitmap_file_header_t;

	typedef struct {
		uint32_t header_size;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bpp;
		uint32_t compression;
		uint32_t image_size;
		uint64_t pad1;
		uint64_t pad2;
	} DIB_header_t;

	Bitmap_file_header_t Bitmap_file_header;
	DIB_header_t DIB_header;

	unsigned char* tmp_image;

	memcpy(&Bitmap_file_header.size, raw + 0x2, 4);
	memcpy(&Bitmap_file_header.offset, raw + 0xA, 4);

	memcpy(&DIB_header.header_size, raw + 0xE, 4);
	memcpy(&DIB_header.width, raw + 0x12, 4);
	memcpy(&DIB_header.height, raw + 0x16, 4);
	memcpy(&DIB_header.bpp, raw + 0x1C, 2);
	memcpy(&DIB_header.image_size, raw + 0x22, 4);


	int h = DIB_header.height;
	int w = DIB_header.width;
	int bytes = DIB_header.bpp / 8;

	tmp_image = malloc(DIB_header.image_size);
	for (i = 0; i < h; i++)
		memcpy(tmp_image + bytes * i * w, raw + 14 + DIB_header.header_size + bytes * (h - 1 - i) * w, bytes * w);

	return tmp_image;
}
