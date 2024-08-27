#include <iostream>
#include <fstream>
#include <ppl.h>
#include <thread>

#include "color.hpp"
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
    int imageWidth = 400;

    int imageHeight = int(imageWidth / aspectRatio);
    // Checks if its atleast one
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    double viewportHeight = 2.0;
    double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

    glm::vec3 cameraCenter = glm::vec3(0, 0, 0);

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
/* 
    for (int j = 0; j < imageHeight; j++)
    {
        clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << flush;
        for (int i = 0; i < imageWidth; i++)
        {
            auto pixel_color = color(double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0);
            write_color(image_file, pixel_color);
        }
    }
 */
    Rectangle rec1 = Rectangle(glm::vec3(-2, 0, 2), glm::vec3(2, 0, 2), glm::vec3(-2, 0, -2), glm::vec3(2, 0, -2), glm::vec3(0.5, 0.5, 0.5));
    concurrency::parallel_for(size_t(0), (size_t)imageHeight, [&](size_t j)
    {
        for (int i = 0; i < imageWidth; i++)
        {
            Ray r = Ray(glm::vec3(0,2,0), glm::vec3(0,1,0));
            float t;
            color pixel_color;
            if(rec1.isHit(r, t)){
                cout << "Hit" << endl;
                 pixel_color = color(1,0, 0);
            }
            else {
                pixel_color = color(0,0,0);
            }
           





            write_color(image_file, pixel_color);
        } 
    });

    clog
        << "\rDone.                 \n";
}