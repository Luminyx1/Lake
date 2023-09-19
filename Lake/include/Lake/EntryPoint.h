#pragma once

#include "Lake/Common.h"

#include "Lake/Application.h"

namespace Lake {

    extern Application* createApplication();

    int lmain(int argc, char** argv) {
        Application* app = createApplication();
        app->run();
        delete app;

        return 0;
    }

} // namespace Lake

int main(int argc, char** argv) {
    return Lake::lmain(argc, argv);
}
