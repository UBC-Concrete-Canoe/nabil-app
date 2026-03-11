#include "Application.h"
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include "app/ViewportController.h"
#include "render/OcctViewport.h"
#include "ui/MainWindow.h"
#include "ui/OcctWidget.h"
#include "ui/ViewportManager.h"
// OCCT helper for creating a test box
#include <BRepPrimAPI_MakeBox.hxx>

Application::Application() = default;

void
Application::run()
{
	MainWindow* window = new MainWindow();
	// Show the window to create the native window handle
	window->show();

	//Create helper
	ViewportManager* viewMgr = new ViewportManager();
	viewMgr->initializeViewport(window);

	// Create and display a demo object
	TopoDS_Shape box = BRepPrimAPI_MakeBox(10.0, 10.0, 20.0).Shape();
	viewMgr->create_shape(box);

	//Configure planar views
	viewMgr->set_planars();
}

Application::~Application(void) {}
