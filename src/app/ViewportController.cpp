#include "ViewportController.h"

#include <cmath>

namespace
{
NCollection_Vec2<int>
toNativePixels(const QPointF& pos, qreal devicePixelRatio)
{
	const qreal dpr = devicePixelRatio > 0.0 ? devicePixelRatio : 1.0;
	return NCollection_Vec2<int>(
		static_cast<int>(std::lround(pos.x() * dpr)), static_cast<int>(std::lround(pos.y() * dpr))
	);
}
} // namespace

ViewportController::ViewportController(OcctViewport* viewport)
  : m_viewport(viewport)
{
}

void
ViewportController::synchronizeAndFlush()
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	m_viewport->synchronizeVisualPoints();
	FlushViewEvents(m_viewport->getContext(), m_viewport->getView(), true);
}

void
ViewportController::onResize()
{
	if (m_viewport && m_viewport->getView())
	{
		// MustBeResized only updates the view's size; without a redraw the old frame
		// stays on screen, stretched to the new size, until the next interaction.
		m_viewport->getView()->MustBeResized();
		m_viewport->getView()->Redraw();
	}
}

void
ViewportController::onMousePressEvent(QMouseEvent* e, qreal devicePixelRatio)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	// Map Qt button enum to OCCT enum
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

	NCollection_Vec2<int> pos = toNativePixels(e->position(), devicePixelRatio);

	// Let MoveTool attempt point selection first on left-click.
	// If it claims the event (a VisualPoint was hit), skip rest of function.
	if (e->button() == Qt::LeftButton && m_moveTool && m_moveTool->onMousePress(pos.x(), pos.y()))
	{
		synchronizeAndFlush();
		return;
	}

	if (e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;
		m_leftButtonDragged = false;
		m_leftPressPos = pos;

		// Planar views can block orbit while still allowing click selection.
		if (!m_rotationEnabled)
		{
			m_viewport->getContext()->MoveTo(
				pos.x(), pos.y(), m_viewport->getView(), false
			);
			synchronizeAndFlush();
			return;
		}
	}
	PressMouseButton(pos, btn, Aspect_VKeyFlags_NONE, false);
	synchronizeAndFlush();
}

void
ViewportController::onMouseReleaseEvent(QMouseEvent* e, qreal devicePixelRatio)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	NCollection_Vec2<int> pos = toNativePixels(e->position(), devicePixelRatio);

	// Notify MoveTool on release so it ends the drag.
	if (e->button() == Qt::LeftButton && m_moveTool)
	{
		m_moveTool->onMouseRelease(pos.x(), pos.y());
	}

	if (!m_rotationEnabled && e->button() == Qt::LeftButton)
	{
		if (m_leftButtonPressed && !m_leftButtonDragged)
		{
			m_viewport->getContext()->MoveTo(
				pos.x(), pos.y(), m_viewport->getView(), false
			);
			m_viewport->getContext()->SelectDetected(AIS_SelectionScheme_Replace);
		}
		m_leftButtonPressed = false;
		m_leftButtonDragged = false;
		synchronizeAndFlush();
		return;
	}

	// Map Qt button enum to OCCT enum
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
	ReleaseMouseButton(pos, btn, Aspect_VKeyFlags_NONE, false);

	// Explicit click-selection for AIS objects (e.g., VisualPoint).
	if (e->button() == Qt::LeftButton)
	{
		if (m_leftButtonPressed && !m_leftButtonDragged)
		{
			m_viewport->getContext()->MoveTo(
				pos.x(), pos.y(), m_viewport->getView(), false
			);
			m_viewport->getContext()->SelectDetected(AIS_SelectionScheme_Replace);
		}
		m_leftButtonPressed = false;
		m_leftButtonDragged = false;
	}

	synchronizeAndFlush();
}

void
ViewportController::onMouseMoveEvent(QMouseEvent* e, qreal devicePixelRatio)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	NCollection_Vec2<int> pos = toNativePixels(e->position(), devicePixelRatio);

	// While MoveTool holds a picked point, it owns the left-drag and orbit is skipped.
	if (m_moveTool && m_moveTool->isActive())
	{
		if (e->buttons() & Qt::LeftButton)
		{
			m_moveTool->onMouseMove(pos.x(), pos.y());
			synchronizeAndFlush();
			return;
		}

		// The left button is no longer held but no release reached us (e.g. it
		// happened outside the window), so end the drag here.
		m_moveTool->onMouseRelease(pos.x(), pos.y());
	}

	m_viewport->getContext()->MoveTo(pos.x(), pos.y(), m_viewport->getView(), false);

	if (m_leftButtonPressed)
	{
		const int dx = pos.x() - m_leftPressPos.x();
		const int dy = pos.y() - m_leftPressPos.y();
		if (dx * dx + dy * dy > 9)
		{
			m_leftButtonDragged = true;
		}
	}

	// Aggregate all pressed buttons during motion
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

	if (!m_rotationEnabled)
	{
		buttons &= ~Aspect_VKeyMouse_LeftButton;
	}

	UpdateMousePosition(pos, buttons, Aspect_VKeyFlags_NONE, false);
	synchronizeAndFlush();
}

void
ViewportController::onWheelEvent(QWheelEvent* e, qreal devicePixelRatio)
{
	if (!m_viewport || !m_viewport->getView())
	{
		return;
	}

	NCollection_Vec2<int> pos = toNativePixels(e->position(), devicePixelRatio);
	// Convert wheel delta to normalized zoom speed (angleDelta ~ 120 per tick)
	double delta = e->angleDelta().y() / 8.0 / 15.0;

	UpdateMouseScroll(Aspect_ScrollDelta(pos, delta, Aspect_VKeyFlags_NONE));
	synchronizeAndFlush();
}

void
ViewportController::onKeyEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Escape)
	{
		// Escape only cancels point movement; it should not fall through to
		// unrelated viewport shortcuts after restoring the model position.
		cancelActiveDrag();
		return;
	}

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
			m_viewport->setShadingMode(false);
			break;

		case Qt::Key_W:
			m_viewport->setShadingMode(true);
			break;

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

	synchronizeAndFlush();
}

bool
ViewportController::cancelActiveDrag()
{
	if (!m_moveTool || !m_moveTool->cancelDrag())
	{
		return false;
	}

	// Clear controller-side pointer state as well as MoveTool state so the next
	// mouse sequence starts normally after cancellation.
	m_leftButtonPressed = false;
	m_leftButtonDragged = false;
	synchronizeAndFlush();
	return true;
}

void
ViewportController::setRotationEnabled(bool enabled)
{
	m_rotationEnabled = enabled;
}

void
ViewportController::setModel(HullModel* model)
{
	m_moveTool = std::make_unique<MoveTool>(m_viewport->getContext(), m_viewport->getView(), model);
}

qreal
ViewportController::inputScale(int logicalWidth, qreal devicePixelRatio) const
{
	if (m_viewport && m_viewport->getView() && !m_viewport->getView()->Window().IsNull()
		&& logicalWidth > 0)
	{
		int windowWidth = 0;
		int windowHeight = 0;
		m_viewport->getView()->Window()->Size(windowWidth, windowHeight);
		if (windowWidth > 0)
		{
			return static_cast<qreal>(windowWidth) / static_cast<qreal>(logicalWidth);
		}
	}
	return devicePixelRatio;
}
