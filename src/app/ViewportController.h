#pragma once
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include "render/OcctViewport.h"
#include "app/MoveTool.h"
#include "core/HullModel.h"
#include <memory>

// Inherit from AIS_ViewController to get standard OCCT mouse math (orbit/zoom/pan calculations)
#include <AIS_ViewController.hxx>
#include <NCollection_Vec2.hxx>

/**
 * @brief Controller layer for viewport interaction.
 *
 * Interprets Qt user input (mouse, keyboard) and translates it into OpenCascade view
 * transformations (orbit, pan, zoom). Inherits from AIS_ViewController to leverage
 * OCCT's built-in interaction mathematics.
 */
class ViewportController : public AIS_ViewController
{
public:
	/**
	 * @brief Constructor.
	 * @param viewport The viewport this controller will manipulate (ownership with caller)
	 */
	ViewportController(OcctViewport* viewport);

	/**
	 * @brief Handle mouse button press.
	 * Initiates interaction (rotation, zoom, or pan) based on button.
	 */
	void onMousePressEvent(QMouseEvent* e, qreal devicePixelRatio);

	/**
	 * @brief Handle mouse button release.
	 * Ends the current interaction.
	 */
	void onMouseReleaseEvent(QMouseEvent* e, qreal devicePixelRatio);

	/**
	 * @brief Handle mouse movement.
	 * Updates view transformation while button is held.
	 */
	void onMouseMoveEvent(QMouseEvent* e, qreal devicePixelRatio);

	/**
	 * @brief Handle mouse wheel scroll.
	 * Zooms the view in/out.
	 */
	void onWheelEvent(QWheelEvent* e, qreal devicePixelRatio);

	/**
	 * @brief Handle keyboard input.
	 * Performs view presets and display mode toggles (F=fit, S=shaded, W=wireframe, etc).
	 */
	void onKeyEvent(QKeyEvent* e);

	/**
	 * @brief Cancel an active point drag and restore its starting position.
	 * @return true when a drag was cancelled.
	 */
	bool cancelActiveDrag();

	/**
	 * @brief Handle window resize.
	 * Notifies the view of geometry changes.
	 */
	void onResize();

	/**
	 * @brief Enable or disable mouse-driven orbit rotation.
	 * @param enabled False to block left-button orbit while keeping pan/zoom active.
	 */
	void setRotationEnabled(bool enabled);

	/**
	 * @brief Attach a hull model to enable point picking and dragging.
	 *
	 * Creates the internal MoveTool using the viewport's current AIS context
	 * and view. Must be called after initialize() so the context and view
	 * are valid.
	 *
	 * @param model Hull model whose control points will be selectable and draggable.
	 */
	void setModel(HullModel* model);

	/**
	 * @brief Scale from Qt logical widget coordinates to OCCT window coordinates.
	 *
	 * OCCT's window may be sized in logical points (e.g. macOS Cocoa) or in
	 * physical pixels (e.g. Windows/X11 high-DPI), so derive the factor from the
	 * actual OCCT window width instead of assuming the device pixel ratio.
	 *
	 * @param logicalWidth Widget width in Qt logical units.
	 * @param devicePixelRatio Fallback when the OCCT window size is unavailable.
	 */
	qreal inputScale(int logicalWidth, qreal devicePixelRatio) const;

private:
	void synchronizeAndFlush();
	bool m_leftButtonPressed = false;
	bool m_leftButtonDragged = false;
	NCollection_Vec2<int> m_leftPressPos = NCollection_Vec2<int>(0, 0);
	OcctViewport* m_viewport;
	bool m_rotationEnabled = true;
	std::unique_ptr<MoveTool> m_moveTool;
};
