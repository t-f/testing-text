#include <SDL2/SDL.h>

#undef main

int main() {
	SDL_Window* window = NULL;
	SDL_Event e;
	int quit = 0;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, 0);
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT)
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
