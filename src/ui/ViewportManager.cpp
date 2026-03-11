#include "OcctWidget.h"
#include "ViewportManager.h"

ViewportManager::ViewportManager() {
    //Create viewports
    persp_viewport = new OcctViewport();
	plan_viewport = new OcctViewport();
	profile_viewport = new OcctViewport();
	bodyplan_viewport = new OcctViewport();

    // Create the controllers to handle user input
	persp_controller = new ViewportController(persp_viewport);
	plan_controller = new ViewportController(plan_viewport);
	profile_controller = new ViewportController(profile_viewport);
	bodyplan_controller = new ViewportController(bodyplan_viewport);
}

ViewportManager::~ViewportManager() {
    delete persp_viewport;
    persp_controller;

    delete plan_viewport;
    delete plan_controller;

    delete profile_viewport;
    delete profile_controller;

    delete bodyplan_viewport;
    delete bodyplan_controller;
}

void ViewportManager::initializeViewport(MainWindow* window) {
    //Locate the widgets from Qt
    OcctWidget* persp_widget = window->findChild<OcctWidget*>("renderWidget"); //This must change if the promoted widget in Qt changes name.
	OcctWidget* plan_widget = window->findChild<OcctWidget*>("planWidget");
	OcctWidget* profile_widget = window->findChild<OcctWidget*>("profileWidget");
	OcctWidget* bodyplan_widget = window->findChild<OcctWidget*>("bodyplanWidget");

    // Ensure Qt creates a native handle; required for OCCT embedding
	(void)persp_widget->winId();
	(void)plan_widget->winId();
	(void)profile_widget->winId();
	(void)bodyplan_widget->winId();

    //Connect widgets to input controllers
    persp_widget->setController(persp_controller);
	plan_widget->setController(plan_controller);
	profile_widget->setController(profile_controller);
	bodyplan_widget->setController(bodyplan_controller);

    // Initialize OCCT within the widget's native window
	persp_viewport->initialize(persp_widget->winId());

	//Obtain context so that the other three viewing planes can be dependent on the main 3D view
	Handle(AIS_InteractiveContext) sharedCtx = persp_viewport->getContext();

	//Initialize three planar views based on main 3D view
	plan_viewport->initialize(plan_widget->winId(), sharedCtx);
	profile_viewport->initialize(profile_widget->winId(), sharedCtx);
	bodyplan_viewport->initialize(bodyplan_widget->winId(), sharedCtx);
}

void ViewportManager::create_shape(TopoDS_Shape shape) {
    persp_viewport->displayShape(shape);
}

void ViewportManager::set_planars() {
    //Set viewing angles
	plan_viewport->setViewPreset(V3d_Zpos); //down Z-axis
	profile_viewport->setViewPreset(V3d_Yneg); //down Y-axis
	bodyplan_viewport->setViewPreset(V3d_Xpos); //down X-axis

	//Fit planar views to shape
	plan_viewport->fitAll();
	profile_viewport->fitAll();
	bodyplan_viewport->fitAll();
}