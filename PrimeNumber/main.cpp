#include "Application.h"

int main(int argc, char** argv)
{
    Application* app = new Application();

    app->Execute();

    delete app;
    return 0;
}
