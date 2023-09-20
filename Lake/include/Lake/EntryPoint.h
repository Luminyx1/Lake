#pragma once

#include "Lake/Common.h"

#include "Lake/Application.h"

namespace lake {

    extern Application* createApplication();

    int main(int argc, char** argv) {
        Application* app = createApplication();
        app->run();
        delete app;

        return 0;
    }

} // namespace lake

int main(int argc, char** argv) {
    return lake::main(argc, argv);
}
