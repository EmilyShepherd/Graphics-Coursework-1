#include "Cone.h"

Cone::Cone(int edges)
{
    this->edges = edges;
    this->draw();
}

void Cone::draw()
{
    double angle_increment = M_PI / this->edges;
    double angle = 0;

    GLdouble top[3]        = { 0, 1, 0 };
    GLdouble center[3]     = { 0, -1, 0 };
    GLdouble downNormal[3] = { 0, -1, 0};

    GLdouble point[3]           = { 0, -1, 0.5 };
    GLdouble pointNormal[3]     = { 0, 0., 0.5 };
    GLdouble midPointNormal[3]  = { 0, 0.25, 0 };
    GLdouble nextPoint[3]       = { 0, -1, 0.5 };
    GLdouble nextPointNormal[3] = { 0, 0., 0.5 };

    for (int x = 0; x < this->edges; x++)
    {
        point[0] = pointNormal[0] = nextPoint[0];
        point[2] = pointNormal[2] = nextPoint[2];

        angle            += angle_increment;
        midPointNormal[0] = sin(angle) * 0.5;
        midPointNormal[2] = cos(angle) * 0.5;

        angle       += angle_increment;
        nextPoint[0] = nextPointNormal[0] = sin(angle) * 0.5;
        nextPoint[2] = nextPointNormal[2] = cos(angle) * 0.5;

        this->addToBuffer(&this->buffer, top, midPointNormal);
        this->addToBuffer(&this->buffer, point, pointNormal);
        this->addToBuffer(&this->buffer, nextPoint, nextPointNormal);

        this->addToBuffer(&this->buffer, point, downNormal);
        this->addToBuffer(&this->buffer, center, downNormal);
        this->addToBuffer(&this->buffer, nextPoint, downNormal);
    }
}
