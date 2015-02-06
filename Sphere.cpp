#include "Sphere.hpp"

Sphere::Sphere(int rows)
{
    this->rows         = rows;
    this->draw();
}

std::vector<Vertex>* Sphere::getNormals()
{
    return &this->normal_lines;
}

void Sphere::draw()
{
    // For the current row, we will save a copy of the circles at the top and bottom
    float row_radius_bottom = 0;
    float row_radius_top    = 0;
    float row_height_bottom = -1;
    float row_height_top    = -1;
    float row_angle         = - M_PI_2;
    float angle_increment   = M_PI / this->rows;
    float V_bottom = 0;
    float V_top    = 0;
    float U_left   = 0;
    float U_right  = 1;
    float V_row_jump  = 1.0f / this->rows;
    float U_cell_jump = 0.5f / this->rows;

    GLdouble* cell_top_left     = (GLdouble*)malloc(sizeof(GLdouble) * 3);
    GLdouble* cell_top_right    = (GLdouble*)malloc(sizeof(GLdouble) * 3);
    GLdouble* cell_bottom_left  = (GLdouble*)malloc(sizeof(GLdouble) * 3);
    GLdouble* cell_bottom_right = (GLdouble*)malloc(sizeof(GLdouble) * 3);

    for (int y = 0; y < this->rows; y++)
    {
        // We're going up a row, so the previous one's top is this one's
        // bottom. We then need to calculate the new top
        row_angle         += angle_increment;
        row_radius_bottom  = row_radius_top;
        row_radius_top     = cos(row_angle);
        row_height_bottom  = row_height_top;
        row_height_top     = sin(row_angle);

        float cell_angle = 0;

        cell_top_right[0]    = 0;
        cell_top_right[1]    = row_height_top;
        cell_top_right[2]    = row_radius_top;

        cell_top_left[1]     = row_height_top;
        cell_bottom_left[1]  = row_height_bottom;

        cell_bottom_right[0] = 0;
        cell_bottom_right[1] = row_height_bottom;
        cell_bottom_right[2] = row_radius_bottom;

        V_bottom = V_top;
        V_top    = V_top + V_row_jump;

        for (int x = 0; x < this->rows * 2; x++)
        {
            cell_angle      += angle_increment;
            // Swap the buffers
            GLdouble *oldTL   = cell_top_left;
            GLdouble *oldBL   = cell_bottom_left;
            cell_top_left     = cell_top_right;    // We'll keep this value
            cell_bottom_left  = cell_bottom_right; // We'll keep this value
            cell_top_right    = oldTL;             // We'll write over this
            cell_bottom_right = oldBL;             // We'll write over this

            cell_top_right[0] = sin(cell_angle) * row_radius_top;
            cell_top_right[2] = cos(cell_angle) * row_radius_top;

            cell_bottom_right[0] = sin(cell_angle) * row_radius_bottom;
            cell_bottom_right[2] = cos(cell_angle) * row_radius_bottom;

            U_left   = U_right;
            U_right -= U_cell_jump;

            this->addToBuffer(cell_top_left,     U_left,  V_top);
            this->addToBuffer(cell_bottom_right, U_right, V_bottom);
            this->addToBuffer(cell_bottom_left,  U_left,  V_bottom);

            this->addToBuffer(cell_bottom_right, U_right, V_bottom);
            this->addToBuffer(cell_top_right,    U_right, V_top);
            this->addToBuffer(cell_top_left,     U_left,  V_top);

            GLdouble bottom_left_normal[3] =
            {
                cell_bottom_left[0] * 1.1,
                cell_bottom_left[1] * 1.1,
                cell_bottom_left[2] * 1.1
            };

            this->addToBuffer(&this->normal_lines, cell_bottom_left,   cell_bottom_left);
            this->addToBuffer(&this->normal_lines, bottom_left_normal, cell_bottom_left);
        }
    }

    free(cell_top_left);
    free(cell_top_right);
    free(cell_bottom_left);
    free(cell_bottom_right);
}
