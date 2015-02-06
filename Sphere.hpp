#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"

class Sphere : public Shape
{
    public:
        Sphere(int rows);
        std::vector<Vertex> *getNormals();
    protected:
    private:
        int rows;
        std::vector<Vertex> normal_lines;
        void draw();
};

#endif // SPHERE_H
