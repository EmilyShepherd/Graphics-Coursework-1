#ifndef _UTILS_H_
#define _UTILS_H_

#include <GL/glew.h>

#include <GLFW/glfw3.h>

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(char *file);

GLuint loadBMP(const char * imagepath);

#endif /* #ifndef _UTILS_H_ */
