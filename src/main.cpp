#include <iostream>
#include <fstream>
#include <ppl.h>
#include <thread>

#include "headers/color.hpp"
#include "headers/camera.hpp"
#include "headers/ray.hpp"
#include "headers/rectangle.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"
#include "headers/HelperFunctions.hpp"
#include "headers/polygon.hpp"
#include "headers/triangle.hpp"

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

    Material recMirror = Material(Material::MaterialType::Mirror);
    // Material recLamb1 = Material(Material::MaterialType::Mirror);
    Material Lamb = Material(Material::MaterialType::Lambertian);

    std::vector<Polygon *> polygons;
    polygons.push_back(new Rectangle(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), color(1, 0, 0), Lamb));
    polygons.push_back(new Rectangle(glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), color(0, 1, 0), recMirror));
    polygons.push_back(new Rectangle(glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), color(0, 0, 1), Lamb));
    polygons.push_back(new Rectangle(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), color(0, 0, 1), recMirror));
    polygons.push_back(new Rectangle(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), color(0, 1, 1), Lamb));
    polygons.push_back(new Rectangle(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), color(1, 0, 1), Lamb));
    polygons.push_back(new Rectangle(glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), color(0.5, 0.5, 0.5), Lamb));
    polygons.push_back(new Rectangle(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), color(0.5, 0.5, 0.5), Lamb));
    polygons.push_back(new Triangle(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), color(1, 1, 0), Lamb));
    polygons.push_back(new Triangle(glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), color(1, 1, 0), Lamb));
    polygons.push_back(new Triangle(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), color(0, 1, 0), Lamb));
    polygons.push_back(new Triangle(glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), color(0, 1, 0), Lamb));
    Camera camera = Camera(glm::dvec3(0, -1, 1), glm::dvec3(0, 1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(-1, 0, 0), pixelSizeX, pixelSizeY, imageWidth, imageHeight);

    int n = 4;
    int rowsDone = 0;

    concurrency::parallel_for(size_t(0), (size_t)imageHeight, [&](size_t j)
                              {
        // clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << flush;
        for (int i = 0; i < imageWidth; i++)
        {
            int pixelIndex = j * imageWidth + i;
            glm::dvec3 pixelPos = glm::dvec3(0.0, i * pixelSizeX - 0.99875, j * pixelSizeY - 0.99875);
            glm::dvec3 rayDirection = pixelPos - camera.eyePos;
            glm::dvec3 pixel_color = glm::dvec3(0.0, 0.0, 0.0);
            for (int k = 0; k < n; k++) // SAMPLING - ANIT ALIASING
            {
                Ray r = Ray(camera.eyePos, rayDirection);
                glm::dvec3 newDirectionRay = r.calculateOffsetRay(pixelSizeX, pixelSizeY);

                // GÖR RAYPATH
                Ray *lastRay =  r.calculateRayPath(polygons);

               
                /* if(didntHit){
                    glm::dvec3 tempColor = glm::dvec3(1.0, 1.0, 1.0) / ((double)n);
                    pixel_color += tempColor;
                } */
                // FÅ FÄRG
                 pixel_color += lastRay->getColorOfRayPath() / (double)n;
                
                /* bool didntHit = true;
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
                } */
            }
            camera.thePixels[pixelIndex].color = pixel_color;
        }
        rowsDone++;
        theHelperFunctions.DisplayLoadingBar(rowsDone, imageHeight); });

    clog
        << "\rWriting Image to File...                 \n";
    rowsDone = 0;
    for (int i = 0; i < imageHeight * imageWidth; i++)
    {
        write_color(image_file, camera.thePixels[i].color);
        rowsDone++;
        theHelperFunctions.DisplayLoadingBar(rowsDone, imageHeight * imageWidth);
    }

    clog
        << "\rDone.                 \n";
}