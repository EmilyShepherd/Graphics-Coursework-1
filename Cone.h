#ifndef CONE_H
#define CONE_H

#include <Shape.h>


class Cone : public Shape
{
    public:
        Cone(int edges);
    protected:
    private:
        int edges;
        void draw();
};

#endif // CONE_H
