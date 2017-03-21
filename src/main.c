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
		memcpy(tmp_image + bytes * i * w, raw + 14 + DIB_header.header_size + bytes * (h - i) * w, bytes * w);

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
