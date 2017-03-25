#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "extensions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#undef main

unsigned char* BMP_Load(const char* fn);
unsigned char* BMP_Decode(unsigned char* raw);
void init_extensions();

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
	SDL_GLContext context;
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
	context = SDL_GL_CreateContext(window);
	init_extensions();
	screen = SDL_GetWindowSurface(window);

	const char* vertex_shader_string   = file_read("./shaders/01.vert");
	const char* fragment_shader_string = file_read("./shaders/01.frag");
	if (vertex_shader_string == NULL || fragment_shader_string == 0 ) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "error", "A shader could not be loaded", NULL);
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	GLuint shader_01V = glCreateShader(GL_VERTEX_SHADER);
	GLuint shader_01F = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint program_01 = glCreateProgram();

	glShaderSource(shader_01V, 1, &vertex_shader_string, NULL);
	glShaderSource(shader_01F, 1, &fragment_shader_string, NULL);

	GLint temp_param = 0;
	GLsizei log_length = 0;
	GLchar info_log[1000];
	char temp_char[1000];

	glCompileShader(shader_01V);
	glGetShaderiv(shader_01V, GL_COMPILE_STATUS, &temp_param);
	if (!temp_param) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Vertex Shader compilation error (01.vert)", NULL);
		glGetShaderInfoLog(shader_01V, 1000, &log_length, info_log);
		sprintf(temp_char, "%s\n\n\n\n\n\n...\n\n\n\nlength: %d", info_log, log_length);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "log", temp_char, NULL);
		glDeleteShader(shader_01V);
		return 1;
	}

	glCompileShader(shader_01F);
	glGetShaderiv(shader_01F, GL_COMPILE_STATUS, &temp_param);
	if (!temp_param) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Fragment Shader compilation error (01.frag)", NULL);
		glGetShaderInfoLog(shader_01F, 1000, &log_length, info_log);
		sprintf(temp_char, "%s\n\n\n\n\n\n...\n\n\n\nlength: %d", info_log, log_length);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Log", temp_char, NULL);
		glDeleteShader(shader_01F);
		return 1;
	}

	glAttachShader(program_01, shader_01V);
	glAttachShader(program_01, shader_01F);
	glLinkProgram(program_01);
	glDetachShader(program_01, shader_01V);
	glDetachShader(program_01, shader_01F);

	glGetProgramiv(program_01, GL_LINK_STATUS, &temp_param);
	if (!temp_param)
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Shader Program link error (program_01)", NULL);

	//glUseProgram(program_01);

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
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
