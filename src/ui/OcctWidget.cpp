#include "OcctWidget.h"
#include "app/ViewportController.h"

OcctWidget::OcctWidget(QWidget* parent)
  : QWidget(parent)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setAutoFillBackground(false);

	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
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
	// OCCT handles rendering; Qt's paint engine is disabled in paintEngine()
}

void
OcctWidget::mousePressEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		const auto scale = m_controller->inputScale(width(), height(), devicePixelRatioF());
		m_controller->onMousePressEvent(e, scale);
		update();
	}
}

void
OcctWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		const auto scale = m_controller->inputScale(width(), height(), devicePixelRatioF());
		m_controller->onMouseMoveEvent(e, scale);
		update();
	}
}

void
OcctWidget::mouseReleaseEvent(QMouseEvent* e)
{
	if (m_controller)
	{
		const auto scale = m_controller->inputScale(width(), height(), devicePixelRatioF());
		m_controller->onMouseReleaseEvent(e, scale);
		update();
	}
}

void
OcctWidget::wheelEvent(QWheelEvent* e)
{
	if (m_controller)
	{
		const auto scale = m_controller->inputScale(width(), height(), devicePixelRatioF());
		m_controller->onWheelEvent(e, scale);
		update();
	}
}

void
OcctWidget::keyPressEvent(QKeyEvent* e)
{
	if (m_controller)
	{
		m_controller->onKeyEvent(e);
		update();
	}
}
