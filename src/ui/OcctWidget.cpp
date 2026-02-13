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
	// We intentionally do nothing here because OCCT handles the rendering.
	// If we painted here, we might overwrite the 3D view.
}

// --- Event Forwarding ---

void
OcctWidget::mousePressEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		m_controller->onMouseEvent(e);
	}
}

void
OcctWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		m_controller->onMouseEvent(e);
	}
}

void
OcctWidget::mouseReleaseEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		m_controller->onMouseEvent(e);
	}
}

void
OcctWidget::wheelEvent(QWheelEvent* e)
{
	if (m_controller)
	{
		m_controller->onWheelEvent(e);
	}
}

void
OcctWidget::keyPressEvent(QKeyEvent* e)
{
	if (m_controller)
	{
		m_controller->onKeyEvent(e);
	}
}