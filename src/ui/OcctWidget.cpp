#include "OcctWidget.h"

// The Linker was specifically looking for this constructor definition
OcctWidget::OcctWidget(QWidget *parent)
    : QWidget(parent)
{
    // Essential for external engine rendering (OCCT/OpenGL)
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Optional: Enable mouse tracking for the active editing loop
    setMouseTracking(true);
}

void OcctWidget::resizeEvent(QResizeEvent *e)
{
    // The App Layer/Linker will eventually use this to trigger OcctViewport::handleResize
    QWidget::resizeEvent(e);
}

void OcctWidget::mouseMoveEvent(QMouseEvent *e)
{
    // This is where the active editing loop begins
    // We will eventually convert 'e' into a Core-layer command
    QWidget::mouseMoveEvent(e);
}