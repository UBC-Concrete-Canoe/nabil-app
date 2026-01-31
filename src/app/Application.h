#pragma once
#include "render/OcctViewport.h"
#include "ui/OcctWidget.h"

class Application
{
public:
    Application();
    void run();

private:
    // TODO: Create the UI render area and create an associate viewport
    void initializeGraphics(OcctWidget *widget, OcctViewport *viewport);
};
