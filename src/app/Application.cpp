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

	OcctWidget* occt_widget = window->findChild<OcctWidget*>("renderWidget"); //This must change if the promoted widget in Qt changes name.
	OcctWidget* plan_widget = window->findChild<OcctWidget*>("planWidget");
	OcctWidget* profile_widget = window->findChild<OcctWidget*>("profileWidget");
	OcctWidget* bodyplan_widget = window->findChild<OcctWidget*>("bodyplanWidget");

	OcctViewport* occt_viewport = new OcctViewport();
	OcctViewport* plan_viewport = new OcctViewport();
	OcctViewport* profile_viewport = new OcctViewport();
	OcctViewport* bodyplan_viewport = new OcctViewport();


	// Create the controller to handle user input
	ViewportController* viewport_controller = new ViewportController(occt_viewport);
	ViewportController* plan_controller = new ViewportController(plan_viewport);
	ViewportController* profile_controller = new ViewportController(profile_viewport);
	ViewportController* bodyplan_controller = new ViewportController(bodyplan_viewport);

	// Set the OCCT widget as the main window's central widget
	//window->setCentralWidget(occt_widget);

	// Show the window to create the native window handle
	window->show();

	// Ensure Qt creates a native handle; required for OCCT embedding
	(void)occt_widget->winId();
	(void)plan_widget->winId();
	(void)profile_widget->winId();
	(void)bodyplan_widget->winId();

	occt_widget->setController(viewport_controller);
	plan_widget->setController(plan_controller);
	profile_widget->setController(profile_controller);
	bodyplan_widget->setController(bodyplan_controller);

	// Initialize OCCT within the widget's native window
	initializeGraphics(occt_widget, occt_viewport);

	//Obtain context so that the other three viewing planes can be dependent on the main 3D view
	Handle(AIS_InteractiveContext) sharedCtx = occt_viewport->getContext();

	//Initialize three planar views based on main 3D view
	plan_viewport->initialize(plan_widget->winId(), sharedCtx);
	profile_viewport->initialize(profile_widget->winId(), sharedCtx);
	bodyplan_viewport->initialize(bodyplan_widget->winId(), sharedCtx);

	// Create and display a demo object
	TopoDS_Shape box = BRepPrimAPI_MakeBox(10.0, 10.0, 20.0).Shape();
	occt_viewport->displayShape(box);

	//Set viewing angles
	plan_viewport->setViewPreset(V3d_Zpos); //down Z-axis
	profile_viewport->setViewPreset(V3d_Yneg); //down Y-axis
	bodyplan_viewport->setViewPreset(V3d_Xpos); //down X-axis

	//Fit planar views to shape
	plan_viewport->fitAll();
	profile_viewport->fitAll();
	bodyplan_viewport->fitAll();
}

void
Application::initializeGraphics(OcctWidget* widget, OcctViewport* viewport)
{
	// Pass the widget to the viewport
	viewport->initialize(widget->winId());
}

Application::~Application(void) {}
