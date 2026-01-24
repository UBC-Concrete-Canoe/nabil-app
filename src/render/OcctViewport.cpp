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
    Handle(Xw_Window) wind = new Xw_Window(displayConnection, (Aspect_Handle)windowHandle);
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
    if (myContext.IsNull() || shape.IsNull())
        return;

    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);

    // Default display settings
    myContext->Display(aisShape, AIS_Shaded, 0, false);
    myContext->SetDisplayMode(aisShape, AIS_Shaded, true);

    fitAll();
}

void OcctViewport::fitAll()
{
    if (!myView.IsNull())
    {
        myView->MustBeResized();
        myView->FitAll();
    }
}

void OcctViewport::redraw()
{
    if (!myView.IsNull())
    {
        myView->Redraw();
    }
}

void OcctViewport::setBackgroundColor(int r, int g, int b)
{
    if (!myView.IsNull())
    {
        myView->SetBackgroundColor(Quantity_NOC_BLACK); // Simplest for skeleton
    }
}