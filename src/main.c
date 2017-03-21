#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#undef main

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
	printf("size: %d\n", size);
	rewind(fp);

	BMP_raw_buffer = malloc(size * sizeof(char));
	fread(BMP_raw_buffer, 1, size, fp);

	printf("first 20 bytes:\n");
	for (i = 0; i < 20; i++)
		printf("%02X ", *(BMP_raw_buffer + i) & 0xFF);
	printf("\nlast 20 bytes:\n");
	for (i = size - 20; i < size; i++)
		printf("%02X ", *(BMP_raw_buffer + i) & 0xFF);
	printf("\n");

	image = BMP_Decode(BMP_raw_buffer);
	free(BMP_raw_buffer);
	//free(image);
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

	//printf("size of 1st header: %d\n", (int)sizeof(Bitmap_file_header));
	//printf("size of 2nd header: %d\n", (int)sizeof(DIB_header));

	memcpy(&Bitmap_file_header.size, raw + 0x2, 4);
	memcpy(&Bitmap_file_header.offset, raw + 0xA, 4);

	memcpy(&DIB_header.header_size, raw + 0xE, 4);
	memcpy(&DIB_header.width, raw + 0x12, 4);
	memcpy(&DIB_header.height, raw + 0x16, 4);
	memcpy(&DIB_header.bpp, raw + 0x1C, 2);
	memcpy(&DIB_header.image_size, raw + 0x22, 4);


	printf("header 1 size: %d, offset: %08X\n", Bitmap_file_header.size, Bitmap_file_header.offset);
	printf("header 2 size: %d, width: %d, height: %d, bpp: %d, img_size: %d\n", DIB_header.header_size,
	       DIB_header.width, DIB_header.height, DIB_header.bpp, DIB_header.image_size);

	int h = DIB_header.height;
	int w = DIB_header.width;
	int bytes = DIB_header.bpp / 8;

	tmp_image = malloc(DIB_header.image_size);
	for (i = 0; i < h; i++)
		memcpy(tmp_image + bytes * i * w, raw + 14 + DIB_header.header_size + bytes * (h - i) * w, bytes * w);

	// draws a diagonal line in the array data that changes the magenta with blue color
	for (i = 0; i < h; i++)
		*(tmp_image + 3 * i + h * 3 * i) = 0x00;

	printf("first 20 bytes:\n");
	for (i = 0; i < 20; i++)
		printf("%02X ", *(tmp_image + i) & 0xFF);
	printf("\nlast 20 bytes:\n");
	for (i = DIB_header.image_size - 20; i < DIB_header.image_size; i++)
		printf("%02X ", *(tmp_image + i) & 0xFF);
	printf("\n");

	return tmp_image;
}

int main() {
	SDL_Window* window = NULL;
	SDL_Surface* screen;
	SDL_Surface* text_surface;
	SDL_Surface* tmp_surface;
	SDL_Event e;
	char* fontname = "./res/10x10C.bmp";
	int quit = 0;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, 0);
	screen = SDL_GetWindowSurface(window);

	//tmp_surface = SDL_LoadBMP(fontname);
	uint32_t rmask, gmask, bmask, amask;
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
	unsigned char* tmp_image = BMP_Load(fontname);
	tmp_surface = SDL_CreateRGBSurfaceFrom(tmp_image, 160, 160, 24, 3 * 160, rmask, gmask, bmask, amask);
	free(tmp_image);
	text_surface = SDL_ConvertSurfaceFormat(tmp_surface, SDL_PIXELFORMAT_RGBA8888, 0);
	SDL_FreeSurface(tmp_surface);

	while (!quit) {
		SDL_BlitSurface(text_surface, NULL, screen, NULL);
		SDL_UpdateWindowSurface(window);
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				quit = 1;
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;
			}
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
