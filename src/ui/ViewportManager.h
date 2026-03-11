#include <QWindow>
#include "MainWindow.h"
#include "app/Application.h"
#include "app/ViewportController.h"
#include "render/OcctViewport.h"

// OCCT helper for creating a test box
#include <BRepPrimAPI_MakeBox.hxx>

/**
 * @brief Manages viewport creation and integration with geometry.
 * 
 * Manages the four viewports and their respective controllers. 
 * Responsible for syncing the viewports to the same geometry and 
 * rendering from different perspectives.
 */
class ViewportManager {
    public:
    //Constructor
    ViewportManager();
    //Destructor
    ~ViewportManager();

    /**
     * @brief Connects the Qt widgets to their respective OCCT viewports.
     * @param MainWindow The main application window.
     */
    void initializeViewport(MainWindow* MainWindow);

    /**
     * @brief Displays sample geometry for testing.
     * @param shape The geometry to be rendered in the viewports.
     */
    void create_shape(TopoDS_Shape shape);

    /**
     * @brief Orient the planar views with respect to the geometry.
     */
    void set_planars();

    private:
    OcctViewport* persp_viewport;
    OcctViewport* plan_viewport;
    OcctViewport* profile_viewport;
    OcctViewport* bodyplan_viewport;

    ViewportController* persp_controller;
    ViewportController* plan_controller;
    ViewportController* profile_controller;
    ViewportController* bodyplan_controller;
};