#include "Application.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include "app/ViewportController.h"
#include "render/OcctViewport.h"
#include "ui/MainWindow.h"
#include "ui/OcctWidget.h"
// OCCT helper for creating a test box
#include <BRepPrimAPI_MakeBox.hxx>

Application::Application() = default;

void
Application::run()
{
	MainWindow* window = new MainWindow();
	OcctWidget* occt_widget = new OcctWidget();
	OcctViewport* occt_viewport = new OcctViewport();

	// Initialise a new controller for occt_viewport
	ViewportController* viewport_controller = new ViewportController(occt_viewport);

	// Put the OCCT-rendering widget into the main window
	window->setCentralWidget(occt_widget);

	// Show the window first to ensure native handles are created
	window->show();

	// Ensure the widget has a native handle created
	(void)occt_widget->winId();
	occt_widget->setController(viewport_controller);

	// Initialize the viewport with the widget handle
	initializeGraphics(occt_widget, occt_viewport);

	// Create a demo box and display it via the viewport
	TopoDS_Shape box = BRepPrimAPI_MakeBox(10.0, 10.0, 20.0).Shape();
	occt_viewport->displayShape(box);
}

void
Application::initializeGraphics(OcctWidget* widget, OcctViewport* viewport)
{
	// Pass the widget to the viewport
	viewport->initialize(widget->winId());
}

Application::~Application(void) {}
