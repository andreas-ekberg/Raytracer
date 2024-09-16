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
#include "HelperFunctions.hpp"
#include "polygon.hpp"
#include "triangle/triangle.h"

using namespace std;
using namespace glm;

color rayColor(const Ray &ray)
{
    return color(0, 0, 0);
}

int main()
{
    HelperFunctions theHelperFunctions = HelperFunctions();
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

    double pixelSizeX = 2.0 / (double)imageWidth;
    double pixelSizeY = 2.0 / (double)imageHeight;
    
    //Rectangle rec1 = Rectangle(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), color(1, 0, 0));
    //Rectangle rec2 = Rectangle(glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), color(0, 1, 0));
    std::vector<std::unique_ptr<Polygon>> polygons;
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), color(1, 0, 0)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), color(0, 1, 0)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), color(0, 0, 1)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), color(0, 0, 1)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), color(0, 1, 1)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), color(1, 0, 1)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), color(0, 0, 0)));
    polygons.push_back(std::make_unique<Rectangle>(glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), color(0, 0, 0)));
    Camera camera = Camera(glm::dvec3(0, -1, 1), glm::dvec3(0, 1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(-1, 0, 0), pixelSizeX, pixelSizeY, imageWidth, imageHeight);
    
    int n = 128;
    int rowsDone = 0;

    std::vector<std::unique_ptr<Polygon>> polygons2;
    polygons2.push_back(std::make_unique<Triangle>(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), color(1, 0, 0)));
    glm::dvec3 hej = polygons2[0]->getColor();
    std::cout << glm::to_string(hej) << std::endl;

    /* Triangle tri1 = Triangle(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, -5), color(1, 0, 0));
    std::string hej = tri1.toString();
    std::cout << hej << std::endl; */

    concurrency::parallel_for(size_t(0), (size_t)imageHeight, [&](size_t j)
    {
        //clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << flush;
        for (int i = 0; i < imageWidth; i++)
        {
            int pixelIndex = j * imageWidth + i;
            glm::dvec3 pixelPos = glm::dvec3(0.0, i * pixelSizeX - 0.99875, j * pixelSizeY - 0.99875);
            glm::dvec3 rayDirection = pixelPos - camera.eyePos;
            glm::dvec3 pixel_color = glm::dvec3(0.0, 0.0, 0.0);
            for (int k = 0; k < n; k++) // SAMPLING
            {
                Ray r = Ray(camera.eyePos, rayDirection);
                glm::dvec3 newDirectionRay = r.calculateOffsetRay(pixelSizeX, pixelSizeY);
                
                bool didntHit = true;
                for(auto& p : polygons) {
                    glm::dvec3 intersectionPoint = p->isHit(r);
                    didntHit = glm::all(glm::isnan(intersectionPoint));
                    if (!didntHit)
                    {
                        glm::dvec3 tempColor = p->getColor() / (double)n;
                        pixel_color += tempColor;
                        break;
                    }
                }
                if(didntHit){
                    glm::dvec3 tempColor = glm::dvec3(1.0, 1.0, 1.0) / ((double)n);
                    pixel_color += tempColor;
                }
            }
            camera.thePixels[pixelIndex].color = pixel_color;
        }
        rowsDone++;
        theHelperFunctions.DisplayLoadingBar(rowsDone, imageHeight);
    });

   clog
        << "\rWriting Image to File...                 \n";
    rowsDone = 0;
    for (int i = 0; i < imageHeight * imageWidth; i++)
    {
        write_color(image_file, camera.thePixels[i].color);
        rowsDone++;
        theHelperFunctions.DisplayLoadingBar(rowsDone, imageHeight*imageWidth);
    }

    clog
        << "\rDone.                 \n";
}