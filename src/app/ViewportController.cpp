#include "ViewportController.h"

ViewportController::ViewportController(OcctViewport* viewport)
  : m_viewport(viewport)
{
}

void
ViewportController::onResize()
{
	if (m_viewport && m_viewport->getView())
	{
		m_viewport->getView()->MustBeResized();
	}
}

void
ViewportController::onMousePressEvent(QMouseEvent* e)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	Aspect_VKeyMouse btn = Aspect_VKeyMouse_NONE;
	if (e->button() == Qt::LeftButton)
	{
		btn = Aspect_VKeyMouse_LeftButton;
	}
	else if (e->button() == Qt::RightButton)
	{
		btn = Aspect_VKeyMouse_RightButton;
	}
	else if (e->button() == Qt::MiddleButton)
	{
		btn = Aspect_VKeyMouse_MiddleButton;
	}

	Graphic3d_Vec2i pos(e->position().x(), e->position().y());
	PressMouseButton(pos, btn, Aspect_VKeyFlags_NONE, false);
	FlushViewEvents(m_viewport->getContext(), m_viewport->getView(), true);
}

void
ViewportController::onMouseReleaseEvent(QMouseEvent* e)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	Aspect_VKeyMouse btn = Aspect_VKeyMouse_NONE;
	if (e->button() == Qt::LeftButton)
	{
		btn = Aspect_VKeyMouse_LeftButton;
	}
	else if (e->button() == Qt::RightButton)
	{
		btn = Aspect_VKeyMouse_RightButton;
	}
	else if (e->button() == Qt::MiddleButton)
	{
		btn = Aspect_VKeyMouse_MiddleButton;
	}

	Graphic3d_Vec2i pos(e->position().x(), e->position().y());
	ReleaseMouseButton(pos, btn, Aspect_VKeyFlags_NONE, false);
	FlushViewEvents(m_viewport->getContext(), m_viewport->getView(), true);
}

void
ViewportController::onMouseMoveEvent(QMouseEvent* e)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	Graphic3d_Vec2i pos(e->position().x(), e->position().y());
	Aspect_VKeyMouse buttons = Aspect_VKeyMouse_NONE;

	if (e->buttons() & Qt::LeftButton)
	{
		buttons |= Aspect_VKeyMouse_LeftButton;
	}
	if (e->buttons() & Qt::RightButton)
	{
		buttons |= Aspect_VKeyMouse_RightButton;
	}
	if (e->buttons() & Qt::MiddleButton)
	{
		buttons |= Aspect_VKeyMouse_MiddleButton;
	}

	UpdateMousePosition(pos, buttons, Aspect_VKeyFlags_NONE, false);
	FlushViewEvents(m_viewport->getContext(), m_viewport->getView(), true);
}

void
ViewportController::onWheelEvent(QWheelEvent* e)
{
	if (!m_viewport->getView())
	{
		return;
	}

	// Standard Zoom Scaling from viewer.cpp
	Graphic3d_Vec2i pos(e->position().x(), e->position().y());
	double delta = e->angleDelta().y() / 8.0 / 15.0;

	UpdateMouseScroll(Aspect_ScrollDelta(pos, delta, Aspect_VKeyFlags_NONE));
	FlushViewEvents(m_viewport->getContext(), m_viewport->getView(), true);
}

void
ViewportController::onKeyEvent(QKeyEvent* e)
{
	// PRESERVED LOGIC: Key bindings from viewerInteractor.cpp

	switch (e->key())
	{
		case Qt::Key_F:
			if (m_viewport->getContext()->NbSelected() > 0)
			{
				m_viewport->fitSelected();
			}
			else
			{
				m_viewport->fitAll();
			}
			break;

		case Qt::Key_S:
			m_viewport->setShadingMode(false); // Shaded
			break;

		case Qt::Key_W:
			m_viewport->setShadingMode(true); // Wireframe
			break;

		// --- Axonometric Views ---
		case Qt::Key_Backspace:
			m_viewport->setViewPreset(V3d_XposYnegZpos);
			break;
		case Qt::Key_T:
			m_viewport->setViewPreset(V3d_TypeOfOrientation_Zup_Top);
			break;
		case Qt::Key_B:
			m_viewport->setViewPreset(V3d_TypeOfOrientation_Zup_Bottom);
			break;
		case Qt::Key_L:
			m_viewport->setViewPreset(V3d_TypeOfOrientation_Zup_Left);
			break;
		case Qt::Key_R:
			m_viewport->setViewPreset(V3d_TypeOfOrientation_Zup_Right);
			break;
	}
}
