#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#undef main

unsigned char* BMP_Load(const char* fn);
unsigned char* BMP_Decode(unsigned char* raw);

int i, j;

void matrix_mult4x4(float a[16], float b[16], float c[16]) {
	int i;
	float d[16];
	//float c[16];
	for (i = 0; i < 16; i++) {
		d[i] =   a[0 + i % 4] * b[0 + 4 * (int)(i / 4)]
		         + a[4 + i % 4] * b[1 + 4 * (int)(i / 4)]
		         + a[8 + i % 4] * b[2 + 4 * (int)(i / 4)]
		         + a[12 + i % 4] * b[3 + 4 * (int)(i / 4)];
	}
	for (i = 0; i < 16; i++)
		c[i] = d[i];
}

char* file_read(const char* filename) {
	SDL_RWops *input = SDL_RWFromFile(filename, "rb");
	if (input == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(input);
	char* content = (char*)malloc(res_size + 1);

	Sint64 size = 0, nb_read = 1;
	char* buf = content;
	while (size < res_size && nb_read != 0) {
		nb_read = SDL_RWread(input, buf, 1, (res_size - size));
		size += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(input);
	if (size != res_size) {
		free(content);
		return NULL;
	}

	content[size] = '\0';
	return content;
}


int main() {
	SDL_Window* window = NULL;
	//SDL_GLContext context;
	SDL_Surface* screen;
	SDL_Surface* text_surface;
	SDL_Surface* tmp_surface;
	SDL_Event e;
	char* fontname = "./res/10x10C.bmp";
	int quit = 0;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_GL_SetSwapInterval(1);

	window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_OPENGL);
	//context = SDL_GL_CreateContext(window);
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
	//SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
