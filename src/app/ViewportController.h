#pragma once
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include "../render/OcctViewport.h"

// Inherit from AIS_ViewController to get standard OCCT mouse math (orbit/zoom/pan calculations)
#include <AIS_ViewController.hxx>

/**
 * @brief The Controller Layer.
 * Interprets raw Qt events and executes logic on the OcctViewport.
 * Replaces functionality previously found in 'viewerInteractor.cpp'.
 */
class ViewportController : public AIS_ViewController
{
public:
	ViewportController(OcctViewport* viewport);

	// --- Input Handlers (Called by UI) ---
	void onMouseEvent(QMouseEvent* e);
	void onWheelEvent(QWheelEvent* e);
	void onKeyEvent(QKeyEvent* e);

	// Handle window resize logic
	void onResize();

private:
	OcctViewport* m_viewport;
};