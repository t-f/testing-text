#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#undef main

int i;

unsigned char* BMP_Load(const char* fn) {
	FILE* fp;
	//unsigned char* image;
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

	//image = BMP_Decode(BMP_raw_buffer);
	fclose(fp);

	//return image;
	return NULL;
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

	BMP_Load(fontname);

	tmp_surface = SDL_LoadBMP(fontname);
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
