#ifndef OCCTVIEWPORT_H
#define OCCTVIEWPORT_H

#include <AIS_InteractiveContext.hxx>
#include <QWindow>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

/**
 * @brief The Engine Layer.
 * Manages the raw OpenCascade rendering window, lighting, and geometric context.
 */
class OcctViewport
{
public:
	OcctViewport();
	~OcctViewport() = default;

	/**
	 * @brief Initializes the OCCT viewer and binds it to the OS window handle.
	 * Contains the critical WNT_Window/Cocoa_Window logic.
	 * @param windowHandle The native OS window ID (WId) from Qt.
	 */
	void initialize(WId windowHandle);

	// --- Geometry Actions ---

	/**
	 * @brief Displays a generic TopoDS_Shape in the context.
	 */
	void displayShape(const TopoDS_Shape& shape);

	/**
	 * @brief Clears all objects from the viewer.
	 */
	void removeAll();

	// --- Camera & View Actions ---
	// These methods are called by the Controller in response to user input.

	void fitAll();
	void fitSelected();

	/**
	 * @brief Sets the camera projection (Top, Bottom, Left, etc.).
	 */
	void setViewPreset(V3d_TypeOfOrientation orientation);

	/**
	 * @brief Toggles between Shaded and Wireframe rendering modes.
	 * @param wireframe If true, sets mode to AIS_WireFrame.
	 */
	void setShadingMode(bool wireframe);

	/**
	 * @brief Forces a redraw of the view.
	 */
	void redraw();

	// --- Accessors for Controller ---
	// The Controller (AIS_ViewController) needs direct access to these handles
	// to perform math for panning/zooming/selecting.
	Handle(AIS_InteractiveContext) getContext() { return myContext; }
	Handle(V3d_View) getView() { return myView; }

private:
	Handle(V3d_Viewer) myViewer;
	Handle(V3d_View) myView;
	Handle(AIS_InteractiveContext) myContext;
};

#endif // OCCTVIEWPORT_H