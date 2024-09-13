#include <iostream>
#include <fstream>
#include <ppl.h>
#include <thread>

#include "color.hpp"
#include "camera.hpp"
#include "ray.hpp"
#include "rectangle.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"

using namespace std;
using namespace glm;

color rayColor(const Ray &ray)
{
    return color(0, 0, 0);
}

int main()
{

    // Image size
    int imageWidth = 800;
    int imageHeight = 800;

    ofstream image_file("image.ppm");
    if (!image_file)
    {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    // Render
    // Pixels are written in a row left -> right and the rows top -> bottom
    image_file << "P3\n"
               << imageWidth << ' ' << imageHeight << "\n255\n";

    Rectangle rec1 = Rectangle(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), color(1, 0, 0));
    Camera camera = Camera(glm::dvec3(0, -1, 1), glm::dvec3(0, 1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(-1, 0, 0));
    double pixelSizeX = 2.0 / (double)imageWidth;
    double pixelSizeY = 2.0 / (double)imageHeight;
    int n = 16;

    for (int j = 0; j < imageHeight; j++)
    {
        clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << flush;
        for (int i = 0; i < imageWidth; i++)
        {
            glm::dvec3 pixelPos = glm::dvec3(0.0, i * pixelSizeX - 0.99875, j * pixelSizeY - 0.99875);
            glm::dvec3 rayDirection = pixelPos - camera.eyePos;
            glm::dvec3 pixel_color = glm::dvec3(0.0,0.0,0.0);
            for (int k = 0; k < n; k++) // SAMPLING
            {
                //srand(static_cast<unsigned int>(time(0)));
                double randomNumberX = ((double)rand() / (double)RAND_MAX) - 1.0;
                double randomNumberY = ((double)rand() / (double)RAND_MAX) - 1.0;
                glm::dvec3 offset = glm::dvec3(0.0, randomNumberX * pixelSizeX, randomNumberY * pixelSizeY);
                glm::dvec3 newRayDirection = rayDirection + offset;
                newRayDirection = glm::normalize(newRayDirection);
                // Create the ray for this pixel
                Ray r = Ray(camera.eyePos, newRayDirection);
                
                glm::dvec3 intersectionPoint = rec1.isHit(r);
                bool didntHit = glm::all(glm::isnan(intersectionPoint));
                if (!didntHit)
                {
                    glm::dvec3 tempColor = glm::dvec3(0.0, 1.0, 0.0) / (double)n;
                    // cout << "Hit" << endl;
                    pixel_color += tempColor;
                }
                else
                {
                    glm::dvec3 tempColor = glm::dvec3(1.0, 1.0, 1.0) / (double)n;
                    pixel_color += tempColor;
                }

            }

            write_color(image_file, pixel_color);
        }
    }

    /*    concurrency::parallel_for(size_t(0), (size_t)imageHeight, [&](size_t j)
                                 {
           for (int i = 0; i < imageWidth; i++)
           {
               Ray r = Ray(glm::dvec3(0,0,0), glm::dvec3(0,1,0));
               float t;
              // color pixel_color = color(1,0,0);
              /*  if(rec1.isHit(r, t)){
                  // cout << "Hit" << endl;
                    pixel_color = color(1,0, 0);
               }
               else {
                   pixel_color = color(1,1,1);
               }

             auto  pixel_color = color(double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0);
               write_color(image_file, pixel_color);
           } }); */

    clog
        << "\rDone.                 \n";
}