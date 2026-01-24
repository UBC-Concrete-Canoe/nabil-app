#ifndef OCCTVIEWPORT_H
#define OCCTVIEWPORT_H

#include <qwindowdefs.h> // For WId
#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class OcctViewport
{
public:
    OcctViewport();
    ~OcctViewport() = default;

    // Bridge: Connects Qt's window handle to OCCT's rendering engine
    void initialize(WId windowHandle);

    // Business Logic: The "Request" to show geometry
    void displayShape(const TopoDS_Shape &shape);

    // Camera & Interaction
    void fitAll();
    void redraw();
    void setBackgroundColor(int r, int g, int b);

private:
    Handle(V3d_Viewer) myViewer;
    Handle(V3d_View) myView;
    Handle(AIS_InteractiveContext) myContext;
};

#endif