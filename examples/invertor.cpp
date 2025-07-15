#include <iostream>

#include <halcyon/image.hpp>

#include <halcyon/video/message_box.hpp>

#include <halcyon/utility/strutil.hpp>

#include <halcyon/main.hpp>

// invertor.cpp:
// Inverts a given image.

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [image path]\n";
        return EXIT_FAILURE;
    }

    hal::surface surf { hal::image::load(argv[1]) };

    const hal::pixel::point size { surf.size() };

    for (hal::pixel::point i { 0, 0 }; i.y < size.y; ++i.y)
    {
        for (i.x = 0; i.x < size.x; ++i.x)
        {
            surf.pixel(i, -surf.pixel(i).get());
        }
    }

    hal::image::save::png(surf, "invert.png");

    return EXIT_SUCCESS;
}
