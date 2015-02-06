#ifndef SHAPE_H
#define SHAPE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include "utils.hpp"
#include <math.h>
#include <time.h>
#include <vector>

typedef struct Vertex
{
    GLdouble position[3];
    GLdouble normal[3];
    GLdouble UV[2];
} Vertex;

class Shape
{
    public:
        std::vector<Vertex> *getBuffer();
    protected:
        std::vector<Vertex> buffer;
        void addToBuffer(GLdouble[3]);
        void addToBuffer(std::vector<Vertex> *buffer, GLdouble[3], GLdouble[3]);
        void addToBuffer(GLdouble[3], GLdouble, GLdouble);

    private:
};

#endif // SHAPE_H
