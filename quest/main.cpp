#include "game.hpp"

int main(int argc, char* argv[])
{
    quest::game g { { argc, argv } };

    g.main_loop();

    return EXIT_SUCCESS;
}