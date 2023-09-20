#include "Lake/EntryPoint.h"

#include "Lake/Application.h"

int lake::main(int argc, char** argv) {
    lake::Application* app = lake::createApplication();
    app->run();
    delete app;

    return 0;
}

int main(int argc, char** argv) {
    return lake::main(argc, argv);
}
