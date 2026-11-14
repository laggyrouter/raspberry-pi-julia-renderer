#include <utility>
#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>

std::pair<double, double> pixelToComplex(int x, int y) {

    double a = -1.8 + (3.6 * x / 640.0);
    double b =  1.35 - (2.7 * y / 480.0);

    return {a, b};
}

int calculateIterations(double a, double b, double c_real, double c_imag, int max_iterations) {

    int iteration = 0;

    while (iteration < max_iterations) {
        double new_a = a * a - b * b + c_real;
        double new_b = 2 * a * b + c_imag;

        a = new_a;
        b = new_b;

        if (a * a + b * b > 4.0) {
            break;
        }

        iteration++;
    }

    return iteration;
}


int main() {

    const int WIDTH = 640;
    const int HEIGHT = 480;
    const int MAX_ITERATIONS = 500;
    std::vector<int> pixels(WIDTH * HEIGHT);
    std::vector<unsigned char> image(WIDTH * HEIGHT * 3);

    for(int y = 0; y < HEIGHT; y++){

        for(int x = 0; x < WIDTH; x++){

            auto coordinates = pixelToComplex(x, y);

           int iterations = calculateIterations(
               coordinates.first,
               coordinates.second,
               -0.8,
               0.156,
               MAX_ITERATIONS
           );

           pixels[y * WIDTH + x] = iterations;
        }
    }

    for(int i = 0; i < WIDTH * HEIGHT; i++){

        int iterations = pixels[i];

        if(iterations  == MAX_ITERATIONS){
            image[i*3] = 0;
            image[i*3+1] = 0;
            image[i*3+2] = 0;
        }
        else {
            image[i*3] = iterations % 256;
            image[i*3+1] = (iterations * 5) % 256;
            image[i*3+2] = (iterations * 10) % 256;
        }
    }

    std::ofstream out("julia.ppm", std::ios::binary);

    if(!out.is_open()) {
        std::cerr << "ERROR: Could not create julia.ppm\n";
        return 1;
    }

    out << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";

    out.write(
        reinterpret_cast<const char*>(image.data()),
        image.size()
    );

    if(!out.good()) {
        std::cerr << "ERROR: Writing image failed\n";
        return 1;
    }

    std::cout << "Image saved successfully!\n";
    out.close();
    auto result = pixelToComplex(0, 0);
    std::cout << "Complex coordinates: "
              << result.first << ", "
              << result.second << "\n";

    int iterations = calculateIterations(0.5, 0.5, -0.8, 0.156, MAX_ITERATIONS);

    std::cout << "Escape iterations: "
              << iterations << "\n";

    std::cout << "Julia Set Renderer\n";
    std::cout << "Resolution: " << WIDTH
              << "x" << HEIGHT << "\n";
    std::cout << "Maximum iterations: "
              << MAX_ITERATIONS << "\n";
    std::cout << "Calculated pixels: "
              << pixels.size() << "\n";
    return 0;
}
