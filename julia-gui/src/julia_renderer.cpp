#include "julia_renderer.h"

#include <QColor>
#include <QImage>
#include <omp.h>
#include <cmath>

static int calculateIterations(
    double x,
    double y,
    double cReal,
    double cImag,
    int maxIterations)
{
    int iteration = 0;

    while (iteration < maxIterations) {
        double newX = x * x - y * y + cReal;
        double newY = 2.0 * x * y + cImag;

        x = newX;
        y = newY;

        if (x * x + y * y > 4.0) {
            break;
        }

        iteration++;
    }

    return iteration;
}

QImage JuliaRenderer::render(const
JuliaParameters &params)
{
    QImage image(
        params.width,
        params.height,
        QImage::Format_RGB32
    );

    const double aspectRatio =
        static_cast<double>(params.width) /
    params.height;

    const double viewHeight = 2.7 / params.zoom;
    const double viewWidth = viewHeight * aspectRatio;

    #pragma omp parallel for schedule(static)
    for (int y = 0; y < params.height; y++) {

        QRgb *row = reinterpret_cast<QRgb
        *>(image.scanLine(y));

        for (int x = 0; x < params.width; x++) {

            double real = params.centerX +
            (static_cast<double> (x) / params.width - 0.5)
            *viewWidth;

            double imag = params.centerY +
            (0.5 - static_cast<double>(y) / params.height)
            *viewHeight;

            int iterations = calculateIterations(
            real,
            imag,
            params.cReal,
            params.cImag,
            params.maxIterations
            );

            if (iterations == params.maxIterations) {
                row[x] = qRgb(5, 8, 20);
            }
            else {
                int red = (iterations * 3) % 256;
                int green = (iterations * 7) % 256;
                int blue = (iterations * 13) % 256;

                row[x] = qRgb(red, green, blue);
            }
        }
    }

    return image;
}
