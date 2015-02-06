#include "Shape.h"

void Shape::addToBuffer(GLdouble *point, GLdouble U, GLdouble V)
{
    this->addToBuffer(point);

    this->buffer.back().UV[0] = U;
    this->buffer.back().UV[1] = V;
}

void Shape::addToBuffer(std::vector<Vertex> *buffer, GLdouble *point, GLdouble *normal)
{
    Vertex vert;
    vert.position[0] = point[0];
    vert.position[1] = point[1];
    vert.position[2] = point[2];

    vert.normal[0] = normal[0];
    vert.normal[1] = normal[1];
    vert.normal[2] = normal[2];

    vert.UV[0] = 0.0f;
    vert.UV[1] = 0.5f;

    buffer->push_back(vert);
}

void Shape::addToBuffer(GLdouble *point)
{
    this->addToBuffer(&this->buffer, point, point);
}

std::vector<Vertex>* Shape::getBuffer()
{
    return &this->buffer;
}
