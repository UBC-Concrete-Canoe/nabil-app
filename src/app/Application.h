#pragma once
#include "render/OcctViewport.h"
#include "ui/OcctWidget.h"

class Application
{
public:
	Application();
	~Application(); // Destructor is required now for unique_ptr
	void run();

private:
	// TODO: Create the UI render area and create an associate viewport
	void initializeGraphics(OcctWidget* widget, OcctViewport* viewport);
};
