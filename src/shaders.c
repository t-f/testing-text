#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "extensions.h"
#include <stdio.h>

char* file_read(const char* filename);


int init_shaders(SDL_Window* window, SDL_GLContext context) {
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
	if (!temp_param) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Shader Program link error (program_01)", NULL);
		return 1;
	}
	return 0;
}