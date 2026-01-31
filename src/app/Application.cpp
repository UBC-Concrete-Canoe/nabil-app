#include "Application.h"
#include "ui/MainWindow.h"
#include "render/OcctViewport.h"
#include "ui/OcctWidget.h"
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Aspect_DisplayConnection.hxx>

Application::Application() = default;

void Application::run()
{
    MainWindow *window = new MainWindow();
    OcctWidget *occt_widget = new OcctWidget();       // TODO: Change this, temporary code for now
    OcctViewport *occt_viewport = new OcctViewport(); // TODO:  Change this, temporary code for now
    initializeGraphics(occt_widget, occt_viewport);
    window->show();
}

void Application::initializeGraphics(OcctWidget *widget, OcctViewport *viewport)
{
    // Pass the widget to the viewport
    viewport->initialize(widget->winId());
}