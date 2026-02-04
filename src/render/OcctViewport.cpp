#include "OcctViewport.h"

// OCCT Platform-specific headers
#ifdef Q_OS_WIN
#include <WNT_Window.hxx>
#elif defined(Q_OS_LINUX)
#include <Xw_Window.hxx>
#elif defined(Q_OS_MAC)
#include <Cocoa_Window.hxx>
#endif

#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <AIS_Shape.hxx>

OcctViewport::OcctViewport() {}

void OcctViewport::initialize(WId windowHandle)
{
    // 1. Create the Graphic Driver
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    // 2. Create the Viewer
    myViewer = new V3d_Viewer(graphicDriver);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    // 3. Create the Interactive Context (The "Selection" manager)
    myContext = new AIS_InteractiveContext(myViewer);

    // 4. Create the View and link it to the OS Window Handle
    myView = myViewer->CreateView();

#ifdef Q_OS_WIN
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)windowHandle);
#elif defined(Q_OS_LINUX)
    Handle(Xw_Window) wind = new Xw_Window(displayConnection, (Aspect_Drawable)windowHandle);
#elif defined(Q_OS_MAC)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *)windowHandle);
#endif

    myView->SetWindow(wind);

    if (!wind->IsMapped())
    {
        wind->Map();
    }
}

void OcctViewport::displayShape(const TopoDS_Shape &shape)
{
}

void OcctViewport::fitAll()
{
}

void OcctViewport::redraw()
{
}

void OcctViewport::setBackgroundColor(int r, int g, int b)
{
}
