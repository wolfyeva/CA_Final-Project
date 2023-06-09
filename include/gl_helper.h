#pragma once

#include <gl.h>

GLuint quickCreateProgram(const char* vert_shader_filename, const char* frag_shader_filename);

GLuint createShader(const char* filename, GLenum type);

GLuint createProgram(GLuint vert, GLuint frag);

GLuint createTexture(const char* filename, bool nearest, bool repeat_x_edge);