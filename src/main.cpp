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
    double aspectRatio = 16.0 / 9.0;
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

    Rectangle rec1 = Rectangle(glm::vec3(10, 6, 5), glm::vec3(13, 0, 5), glm::vec3(10, 6, -5), glm::vec3(13, 0, -5), color(1, 0, 0));
    Camera camera = Camera(glm::vec3(0, -1, 1), glm::vec3(0, 1, 1), glm::vec3(0, -1, -1), glm::vec3(0, 1, -1), glm::vec3(-1, 0, 0));
    double pixelSizeX = 2.0 / imageWidth;
    double pixelSizeY = 2.0 / imageHeight;

    for (int j = 0; j < imageHeight; j++)
    {
        clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << flush;
        for (int i = 0; i < imageWidth; i++)
        {
            glm::vec3 pixelPos = glm::vec3(0, i * pixelSizeX - 0.99875, j * pixelSizeY - 0.99875);
            glm::vec3 rayDirection = pixelPos - camera.eyePos;
            for (int n = 0; n < 16; n++) // SAMPLING
            {
                glm::vec3 offset = getOffset();
                rayDirection += offset;
                rayDirection = glm::normalize(rayDirection);
                // Create the ray for this pixel
                Ray r = Ray(camera.eyePos, rayDirection);
                color pixel_color;
                glm::vec3 intersectionPoint = rec1.isHit(r);
                bool didntHit = glm::all(glm::isnan(intersectionPoint));
                if (!didntHit)
                {
                    // cout << "Hit" << endl;
                    pixel_color = color(1, 0, 0);
                }
                else
                {
                    pixel_color = color(1, 1, 1);
                }
            }

            write_color(image_file, pixel_color);
        }
    }

    /*    concurrency::parallel_for(size_t(0), (size_t)imageHeight, [&](size_t j)
                                 {
           for (int i = 0; i < imageWidth; i++)
           {
               Ray r = Ray(glm::vec3(0,0,0), glm::vec3(0,1,0));
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