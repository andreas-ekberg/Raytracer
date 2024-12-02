#include <iostream>
#include <fstream>
#include <ppl.h>
#include <thread>
#include <chrono>

#include "headers/color.hpp"
#include "headers/camera.hpp"
#include "headers/ray.hpp"
#include "headers/rectangle.hpp"
#include "glmlib/glm.hpp"
#include "glmlib/gtx/string_cast.hpp"
#include "headers/HelperFunctions.hpp"
#include "headers/polygon.hpp"
#include "headers/triangle.hpp"
#include "headers/sphere.hpp"
#include "headers/light.hpp"
#include <iostream>

using namespace std;
using namespace glm;
using namespace std::chrono;

std::vector<Polygon *> Polygon::polygons;

int main()
{
    HelperFunctions theHelperFunctions = HelperFunctions();
    // Image size
    int imageWidth = 400;
    int imageHeight = 400;

    ofstream image_file("image.ppm");
    if (!image_file)
    {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    auto render_start = high_resolution_clock::now();
    // Render
    // Pixels are written in a row left -> right and the rows top -> bottom
    image_file << "P3\n"
               << imageWidth << ' ' << imageHeight << "\n255\n";

    double pixelSizeX = 2.0 / (double)imageWidth;
    double pixelSizeY = 2.0 / (double)imageHeight;

    Material recMirror = Material(Material::MaterialType::Mirror);
    // Material recLamb1 = Material(Material::MaterialType::Mirror);
    Material Lamb = Material(Material::MaterialType::Lambertian);

    // ---- The room ---- //
    // std::vector<Polygon *> polygons;
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), color(0.8, 0.2, 0.2), Lamb));
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), color(0.2, 0.8, 0.2), recMirror));
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), color(0.2, 0.2, 0.8), Lamb));
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), color(0.2, 0.2, 0.8), Lamb));
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), color(0.8, 0.2, 0.2), Lamb));
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), color(0.8, 0.2, 0.8), Lamb));

    Polygon::polygons.push_back(new Rectangle(glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), color(1, 1, 1), Lamb));

    //Polygon::polygons.push_back(new Rectangle(glm::dvec3(8, 0.5, -2), glm::dvec3(8, -0.5, -2), glm::dvec3(7, 0.5, -2), glm::dvec3(7, -0.5, -2), color(0.2, 0.8, 0.2), Lamb));
    Polygon::polygons.push_back(new Rectangle(glm::dvec3(8, -5, -4), glm::dvec3(8, -5.8, -4), glm::dvec3(7, -5, -4), glm::dvec3(7, -5.8, -4), color(0.2, 0.8, 0.2), Lamb));

    Polygon::polygons.push_back(new Rectangle(glm::dvec3(0, 6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), color(1, 1, 1), Lamb));

    // Polygon::polygons.push_back(new Triangle(glm::dvec3(13, 0, 5), glm::dvec3(10, 6, 5), glm::dvec3(10, -6, 5), color(0.5, 0.5, 0.5), Lamb));
    Polygon::polygons.push_back(new Triangle(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), color(1, 1, 1), Lamb));
    Polygon::polygons.push_back(new Triangle(glm::dvec3(10, 6, -5), glm::dvec3(10, -6, -5), glm::dvec3(13, 0, -5), color(1, 1, 1), Lamb));
    Polygon::polygons.push_back(new Triangle(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), color(1, 1, 1), Lamb));
    Polygon::polygons.push_back(new Triangle(glm::dvec3(0, -6, -5), glm::dvec3(0, 6, -5), glm::dvec3(-3, 0, -5), color(1, 1, 1), Lamb));

    Polygon::polygons.push_back(new Sphere(glm::dvec3(8.0, 0.0, 0.0), 2.0, color(0.8, 0.8, 0.8), Lamb));

    // ---- The lights ---- //
    Light *areaLight = new Light(glm::dvec3(5.0, 1.5, 4.99), glm::dvec3(5.0, -1.5, 4.99), glm::dvec3(8.0, 1.5, 4.99), glm::dvec3(8.0, -1.5, 4.99), 50.0);
    //Light *areaLight = new Light(glm::dvec3(5, 5.9, 4.99), glm::dvec3(5, 2.9, 4.99), glm::dvec3(8, 5.9, 4.99), glm::dvec3(8, 2.9, 4.99), 50.0);
    Polygon::polygons.push_back(areaLight);

    // ---- The camera ---- //
    Camera camera = Camera(glm::dvec3(0, -1, 1), glm::dvec3(0, 1, 1), glm::dvec3(0, -1, -1), glm::dvec3(0, 1, -1), glm::dvec3(-1, 0, 0), pixelSizeX, pixelSizeY, imageWidth, imageHeight);

    int n = 10; // Number of samples per pixel
    int rowsDone = 0;

    concurrency::parallel_for(size_t(0), (size_t)imageHeight, [&](size_t j)
                              {
        for (int i = 0; i < imageWidth; i++)
        {
            int pixelIndex = j * imageWidth + i;
            int invertedY = imageHeight - j - 1; // Invert Y position
            glm::dvec3 pixelPos = glm::dvec3(0.0, i * pixelSizeX - (1-pixelSizeX), invertedY * pixelSizeY - (1.0-pixelSizeY));
            glm::dvec3 rayDirection = pixelPos - camera.eyePos;
            glm::dvec3 pixel_color = glm::dvec3(0.0, 0.0, 0.0);
            for (int k = 0; k < n; k++) // SAMPLING - ANTI ALIASING
            {
                Ray r = Ray(camera.eyePos, rayDirection);
                glm::dvec3 newDirectionRay = r.calculateOffsetRay(pixelSizeX, pixelSizeY);

                // GÖR RAYPATH
                Ray *lastRay =  r.calculateRayPath();

                // FÅ FÄRG
                pixel_color += lastRay->getColorOfRayPath(*areaLight) / (double)n;
                //std::cout << pixelIndex << std::endl;
                
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

    auto render_end = high_resolution_clock::now();
    auto render_duration = duration_cast<seconds>(render_end - render_start);

    cout << "Render time: " << render_duration.count() << " seconds." << endl;
}