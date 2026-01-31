#include "viewer.h"

// Qt Includes
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>

// OpenCascade Includes
#include <OpenGl_GraphicDriver.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <WNT_Window.hxx>
#include <V3d_AmbientLight.hxx>
#include <V3d_DirectionalLight.hxx>

#include <AIS_Shape.hxx>

// 1. Constructor
Viewer::Viewer(QWidget* parent) : QWidget(parent), m_isInitialized(false)
{
    // Force this widget to be a native Windows window (HWND)
    setAttribute(Qt::WA_NativeWindow);
    // Tell Qt we will paint the screen manually
    setAttribute(Qt::WA_PaintOnScreen);
    // Tell Qt NOT to clear the background
    setAttribute(Qt::WA_NoSystemBackground);
    // Another flag to stop auto-filling the background
    setAutoFillBackground(false);
}

// 2. Setup Logic
void Viewer::init()
{
    if (m_isInitialized) return;

    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(displayConnection);
    m_viewer = new V3d_Viewer(graphicDriver);
    
    // Lighting
    Handle(V3d_DirectionalLight) lightDir = new V3d_DirectionalLight(V3d_Zneg, Quantity_Color(Quantity_NOC_GRAY97), 1);
    lightDir->SetDirection(1.0, -2.0, -10.0);
    Handle(V3d_AmbientLight) lightAmb = new V3d_AmbientLight();
    m_viewer->AddLight(lightDir);
    m_viewer->AddLight(lightAmb);
    m_viewer->SetLightOn(lightDir);
    m_viewer->SetLightOn(lightAmb);

    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->DefaultDrawer()->SetFaceBoundaryDraw(true); 

    m_view = m_viewer->CreateView();
    m_view->SetImmediateUpdate(false);
    m_view->SetShadingModel(V3d_PHONG);

    // --- FIX: FORCE BACKGROUND TO BLACK ---
    // This proves if the viewer is working or if Qt is blocking it
    m_view->SetBackgroundColor(Quantity_NOC_BLACK);

    // Bind Window
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) winId());
    m_view->SetWindow(wind);
    
    if (!wind->IsMapped()) wind->Map();

    m_evtMgr = new ViewerInteractor(m_view, m_context);
    
    m_isInitialized = true;
}

// 3. Paint Event 
void Viewer::paintEvent(QPaintEvent*)
{
    if (!m_isInitialized) {
        init();
    }
    
    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

// 4. Resize Event
void Viewer::resizeEvent(QResizeEvent*)
{
    if (!m_isInitialized) {
        init();
    }
    
    if (!m_view.IsNull()) {
        m_view->MustBeResized();
    }
}

// 5. Input Handling
void Viewer::mousePressEvent(QMouseEvent* event)
{
    if (m_evtMgr.IsNull()) return;
    Aspect_VKeyMouse btn = Aspect_VKeyMouse_NONE;
    if (event->button() == Qt::LeftButton) btn = Aspect_VKeyMouse_LeftButton;
    else if (event->button() == Qt::RightButton) btn = Aspect_VKeyMouse_RightButton;
    else if (event->button() == Qt::MiddleButton) btn = Aspect_VKeyMouse_MiddleButton;
    Graphic3d_Vec2i pos(event->position().x(), event->position().y());
    m_evtMgr->PressMouseButton(pos, btn, Aspect_VKeyFlags_NONE, false);
}

void Viewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_evtMgr.IsNull()) return;
    Aspect_VKeyMouse btn = Aspect_VKeyMouse_NONE;
    if (event->button() == Qt::LeftButton) btn = Aspect_VKeyMouse_LeftButton;
    else if (event->button() == Qt::RightButton) btn = Aspect_VKeyMouse_RightButton;
    else if (event->button() == Qt::MiddleButton) btn = Aspect_VKeyMouse_MiddleButton;
    Graphic3d_Vec2i pos(event->position().x(), event->position().y());
    m_evtMgr->ReleaseMouseButton(pos, btn, Aspect_VKeyFlags_NONE, false);
}

void Viewer::mouseMoveEvent(QMouseEvent* event)
{
    if (m_evtMgr.IsNull()) return;
    Graphic3d_Vec2i pos(event->position().x(), event->position().y());
    Aspect_VKeyMouse buttons = Aspect_VKeyMouse_NONE;
    if (event->buttons() & Qt::LeftButton) buttons |= Aspect_VKeyMouse_LeftButton;
    if (event->buttons() & Qt::RightButton) buttons |= Aspect_VKeyMouse_RightButton;
    if (event->buttons() & Qt::MiddleButton) buttons |= Aspect_VKeyMouse_MiddleButton;
    m_evtMgr->UpdateMousePosition(pos, buttons, Aspect_VKeyFlags_NONE, false);
}

void Viewer::wheelEvent(QWheelEvent* event)
{
    if (m_evtMgr.IsNull()) return;
    Graphic3d_Vec2i pos(event->position().x(), event->position().y());
    double delta = event->angleDelta().y() / 8.0 / 15.0; 
    m_evtMgr->UpdateMouseScroll(Aspect_ScrollDelta(pos, delta, Aspect_VKeyFlags_NONE));
}

void Viewer::addShape(const TopoDS_Shape& shape)
{
    // 1. Ensure the viewer is initialized before we try to use it
    if (!m_isInitialized) {
        init();
    }

    // 2. Wrap the raw shape in an interactive object
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    
    // 3. Display it!
    // true = update viewer immediately
    m_context->Display(aisShape, true); 
    
    // 4. Fit the view so we can see the object
    m_view->FitAll();
}