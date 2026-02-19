#include "OcctWidget.h"
#include "app/ViewportController.h"

OcctWidget::OcctWidget(QWidget* parent)
  : QWidget(parent)
{
	// --- PRESERVED LOGIC: Window Attributes ---
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground); // Do not clear background (prevents flickering)
	setAutoFillBackground(false);

	// Interaction settings
	setMouseTracking(true);          // Needed for hover highlighting
	setFocusPolicy(Qt::StrongFocus); // Needed to catch Key Events (F, S, W)
}

void
OcctWidget::resizeEvent(QResizeEvent* e)
{
	if (m_controller)
	{
		m_controller->onResize();
	}
	QWidget::resizeEvent(e);
}

void
OcctWidget::paintEvent(QPaintEvent* e)
{
	// OCCT handles all rendering; we just need to exist as a container
	// The view redraws itself through FlushViewEvents called by the controller
}

// --- Event Forwarding ---

void
OcctWidget::mousePressEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		m_controller->onMousePressEvent(e);
		update(); // Schedule a repaint
	}
}

void
OcctWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		m_controller->onMouseMoveEvent(e);
		update(); // Schedule a repaint
	}
}

void
OcctWidget::mouseReleaseEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		m_controller->onMouseReleaseEvent(e);
		update(); // Schedule a repaint
	}
}

void
OcctWidget::wheelEvent(QWheelEvent* e)
{
	if (m_controller)
	{
		m_controller->onWheelEvent(e);
		update(); // Schedule a repaint
	}
}

void
OcctWidget::keyPressEvent(QKeyEvent* e)
{
	if (m_controller)
	{
		m_controller->onKeyEvent(e);
		update(); // Schedule a repaint
	}
}
