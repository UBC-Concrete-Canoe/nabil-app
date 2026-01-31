#pragma once
#include <QWidget> 

// OpenCascade Includes
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

// Local Includes
#include "viewerInteractor.h"

#include <TopoDS_Shape.hxx>

class Viewer : public QWidget 
{
    Q_OBJECT

public:
    explicit Viewer(QWidget* parent = nullptr);
    virtual ~Viewer() = default;

    Handle(AIS_InteractiveContext) GetContext() const { return m_context; }

    // --- CRITICAL: This line disables Qt's own rendering engine ---
    QPaintEngine* paintEngine() const override { return nullptr; }

    // New function to display a shape
    void addShape(const TopoDS_Shape& shape);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    // Inputs
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void init(); 

    Handle(V3d_Viewer)              m_viewer;
    Handle(V3d_View)                m_view;
    Handle(AIS_InteractiveContext)  m_context;
    Handle(ViewerInteractor)        m_evtMgr;
    bool                            m_isInitialized; 
};