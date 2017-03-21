#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#undef main

unsigned char* BMP_Load(const char* fn);
unsigned char* BMP_Decode(unsigned char* raw);

int i, j;


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
