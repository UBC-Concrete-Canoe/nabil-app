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

	// Create the controller to handle user input
	ViewportController* viewport_controller = new ViewportController(occt_viewport);

	// Set the OCCT widget as the main window's central widget
	window->setCentralWidget(occt_widget);

	// Show the window to create the native window handle
	window->show();

	// Ensure Qt creates a native handle; required for OCCT embedding
	(void)occt_widget->winId();
	occt_widget->setController(viewport_controller);

	// Initialize OCCT within the widget's native window
	initializeGraphics(occt_widget, occt_viewport);

	// Create and display a demo object
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
