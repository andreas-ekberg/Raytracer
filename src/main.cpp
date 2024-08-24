#include <iostream>
#include <fstream>

#include "vec3.hpp"
#include "color.hpp"

using namespace std;

int main()
{

    // Image size
    int image_width = 256;
    int image_height = 256;

    ofstream image_file("image.ppm");
    if (!image_file)
    {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    // Render
    // Pixels are written in a row left -> right and the rows top -> bottom
    image_file << "P3\n"
               << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    {
        clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            write_color(image_file, pixel_color);
        }
    }

    clog << "\rDone.                 \n";
}