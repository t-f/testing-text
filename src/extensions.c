#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "extensions.h"

void init_extensions() {
	//glGenBuffers    = SDL_GL_GetProcAddress("glGenBuffers");
	//glBindBuffer    = SDL_GL_GetProcAddress("glBindBuffer");
	//glBufferData    = SDL_GL_GetProcAddress("glBufferData");
	//glBufferSubData = SDL_GL_GetProcAddress("glBufferSubData");
	//glDeleteBuffers = SDL_GL_GetProcAddress("glDeleteBuffers");

	glCreateShader  = SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource  = SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = SDL_GL_GetProcAddress("glCompileShader");
	glCreateProgram = SDL_GL_GetProcAddress("glCreateProgram");
	glAttachShader  = SDL_GL_GetProcAddress("glAttachShader");
	glDetachShader  = SDL_GL_GetProcAddress("glDetachShader");
	glLinkProgram   = SDL_GL_GetProcAddress("glLinkProgram");
	//glUseProgram    = SDL_GL_GetProcAddress("glUseProgram");
	//glEnableVertexAttribArray  = SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	//glDisableVertexAttribArray = SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	//glVertexAttribPointer      = SDL_GL_GetProcAddress("glVertexAttribPointer");
	//glBindVertexArray          = SDL_GL_GetProcAddress("glBindVertexArray");
	//glGenVertexArrays          = SDL_GL_GetProcAddress("glGenVertexArrays");
	//glGetUniformLocation       = SDL_GL_GetProcAddress("glGetUniformLocation");
	//glUniform1f                = SDL_GL_GetProcAddress("glUniform1f");
	//glUniform2f                = SDL_GL_GetProcAddress("glUniform2f");
	//glUniform3f                = SDL_GL_GetProcAddress("glUniform3f");
	//glUniformMatrix4fv         = SDL_GL_GetProcAddress("glUniformMatrix4fv");
	//glDrawElementsBaseVertex   = SDL_GL_GetProcAddress("glDrawElementsBaseVertex");
	glGetShaderiv              = SDL_GL_GetProcAddress("glGetShaderiv");
	glGetProgramiv             = SDL_GL_GetProcAddress("glGetProgramiv");

	glGetShaderInfoLog         = SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glDeleteShader             = SDL_GL_GetProcAddress("glDeleteShader");
	//glGetUniformBlockIndex     = SDL_GL_GetProcAddress("glGetUniformBlockIndex");
	//glUniformBlockBinding      = SDL_GL_GetProcAddress("glUniformBlockBinding");
	//glBindBufferRange          = SDL_GL_GetProcAddress("glBindBufferRange");
	//glBindBufferBase           = SDL_GL_GetProcAddress("glBindBufferBase");
}
