
#ifndef JULIA_RENDERER_H
#define JULIA_RENDERER_H

#include <QImage>

struct JuliaParameters {
    int width = 800;
    int height = 480;
    int maxIterations = 500;

    double cReal = -0.8;
    double cImag = 0.156;

    double zoom = 1.0;
    double centerX = 0.0;
    double centerY = 0.0;
};

class JuliaRenderer 
{
public:
    static QImage render(const JuliaParameters 
&params);
};

#endif 
