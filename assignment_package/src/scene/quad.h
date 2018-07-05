#ifndef QUAD_H
#define QUAD_H

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Quad : public Drawable
{
public:
    Quad(OpenGLContext* context); // A quad split into 4 triangles along its diagonals

    float yvals[5]; // y-coordinates of the 4 corners in counter-clockwise order,
                    // starting from (0,0), followed by the height of the center point

    virtual ~Quad(){}
    void create() override;
};

#endif // QUAD_H
