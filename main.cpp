#include <iostream>
#include "main.h"

import Engine;

int main()
{
    std::cout << "AhTom Engine Version " << ENGINE_VERSION_MAJOR << "."
        << ENGINE_VERSION_MINOR << "." << ENGINE_VERSION_PATCH << std::endl;

    AhtomEngine* engine = new AhtomEngine(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);

    try {

        engine->run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
