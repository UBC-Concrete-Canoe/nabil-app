#include "OcctViewport.h"

// OCCT Core
#include <OpenGl_GraphicDriver.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <AIS_Shape.hxx>

// Platform Specific Windows
#ifdef _WIN32
#include <WNT_Window.hxx>
#elif defined(__APPLE__)
#include <Cocoa_Window.hxx>
#else
#include <Xw_Window.hxx>
#endif

OcctViewport::OcctViewport() {}

void OcctViewport::initialize(WId windowHandle)
{
    // 1. Create Graphic Driver
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    myViewer = new V3d_Viewer(graphicDriver);

    // --- LIGHTING SETUP ---
    Handle(V3d_DirectionalLight) lightDir = new V3d_DirectionalLight(V3d_Zneg, Quantity_Color(Quantity_NOC_GRAY97), 1);
    lightDir->SetDirection(1.0, -2.0, -10.0);
    Handle(V3d_AmbientLight) lightAmb = new V3d_AmbientLight();

    myViewer->AddLight(lightDir);
    myViewer->AddLight(lightAmb);
    myViewer->SetLightOn(lightDir);
    myViewer->SetLightOn(lightAmb);

    // 2. Create Context
    myContext = new AIS_InteractiveContext(myViewer);

    // Preference: Draw face boundaries (from viewer.cpp)
    myContext->DefaultDrawer()->SetFaceBoundaryDraw(true);

    // 3. Create View
    myView = myViewer->CreateView();
    myView->SetImmediateUpdate(false);
    myView->SetShadingModel(V3d_PHONG);

    // This resolves issues where Qt might paint over the background.
    myView->SetBackgroundColor(Quantity_NOC_BLACK);

    // 4. Window Embedding
    // Logic adapted to be cross-platform compatible based on viewer.cpp
#ifdef _WIN32
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)windowHandle);
#elif defined(__APPLE__)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *)windowHandle);
#else
    Handle(Xw_Window) wind = new Xw_Window(displayConnection, (Aspect_Drawable)windowHandle);
#endif

    myView->SetWindow(wind);
    if (!wind->IsMapped())
        wind->Map();
}

void OcctViewport::displayShape(const TopoDS_Shape &shape)
{
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    myContext->Display(aisShape, true);
    myView->FitAll();
}

void OcctViewport::removeAll()
{
    if (!myContext.IsNull())
    {
        myContext->RemoveAll(true);
    }
}

void OcctViewport::fitAll()
{
    if (!myView.IsNull())
        myView->FitAll();
}

void OcctViewport::fitSelected()
{
    if (!myContext.IsNull())
        myContext->FitSelected(myView);
}

void OcctViewport::setViewPreset(V3d_TypeOfOrientation orientation)
{
    if (!myView.IsNull())
    {
        myView->SetProj(orientation);
        myView->Redraw();
    }
}

void OcctViewport::setShadingMode(bool wireframe)
{
    if (myContext.IsNull())
        return;

    // Logic ported from ViewerInteractor.cpp
    const int dm = wireframe ? AIS_WireFrame : AIS_Shaded;

    if (myContext->NbSelected() == 0)
    {
        myContext->SetDisplayMode(dm, false);
    }
    else
    {
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
        {
            myContext->SetDisplayMode(myContext->SelectedInteractive(), dm, false);
        }
    }
    myContext->UpdateCurrentViewer();
}

void OcctViewport::redraw()
{
    if (!myView.IsNull())
        myView->Redraw();
}