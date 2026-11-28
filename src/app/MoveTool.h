#pragma once

#include <stack>
#include <memory>
#include <optional>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Graphic3d_Vec3.hxx>
#include <V3d_View.hxx>
#include <gp_Pnt.hxx>

#include "core/HullModel.h"
#include "render/VisualPoint.h"
#include "app/MoveCommand.h"

struct ScreenPoint
{
    // Coordinates are expressed in OCCT render-window pixels, not Qt logical pixels.
    int x;
    int y;
};

struct MoveResult
{
    int pointId;
    gp_Pnt originalPosition;
    gp_Pnt finalPosition;
};

/**
 * @brief Manages point selection and dragging in the 3D viewport.
 *
 * Tool modes:
 *   View   — default; no point selected; mouse events control camera orbit
 *   Select — a point has been picked but the mouse hasn't moved yet
 *   Move   — an active drag is in progress
 */
class MoveTool
{
public:
    enum class Mode { View, Select, Move };

    /**
     * @param context  OCCT interactive context used for hit-testing
     * @param view     3D view used for coordinate projection
     * @param model    Hull model whose points will be mutated
     */
    MoveTool(Handle(AIS_InteractiveContext) context, Handle(V3d_View) view, HullModel* model);

    /**
     * @brief Begin moving a model point from a screen position.
     * @return true when the point exists and the drag was started.
     */
    bool beginDrag(int pointId, ScreenPoint screenPosition);

    /**
     * @brief Update the active drag from a screen position.
     * @return true when a model update was applied.
     */
    bool updateDrag(ScreenPoint screenPosition);

    /**
     * @brief Finish the active drag and retain its final position.
     * @return The completed movement, or empty when no drag is active.
     */
    std::optional<MoveResult> finishDrag(ScreenPoint screenPosition);

    /**
     * @brief Cancel the active drag and restore its original position.
     * @return true when an active drag was restored.
     */
    bool cancelDrag();

    /**
     * @brief Handle a left-button press at screen position (x, y).
     * @return true if a point was selected (caller should suppress camera orbiting)
     */
    bool onMousePress(int x, int y);

    /**
     * @brief Handle mouse movement.
     *
     * Computes the 3D displacement using Graphic3d_Vec3 and the camera
     * projection plane, then applies it to the selected point.
     *
     * @return true if a drag was applied (caller should suppress camera orbiting)
     */
    bool onMouseMove(int x, int y);

    /**
     * @brief Handle left-button release and retain the final point position.
     */
    void onMouseRelease(int x, int y);



    // Undo / Redo - Ask if this should be moved up to a bigger structure and not specific to this
 
    // //! Reverse the most recent completed drag. No-op if stack is empty.
    // void undo();

    // //! Re-apply the most recently undone drag. No-op if redo stack is empty.
    // void redo();

    // bool canUndo() const { return !m_undoStack.empty(); }
    // bool canRedo() const { return !m_redoStack.empty(); }


    Mode mode()          const { return m_mode; }
    bool hasSelection()  const { return m_activePointId.has_value(); }
    //! True between a press that picked a point and the matching release.
    bool isActive()      const { return m_mode != Mode::View && m_activePointId.has_value(); }
    bool isDragging()    const { return isActive(); }

private:
    /**
     * @brief Project a screen pixel onto the plane through `projectionAnchor`
     *        perpendicular to the camera, returning the world-space
     *        hit point as a Graphic3d_Vec3.
     */
    Graphic3d_Vec3 projectOntoPlane(
        int x, int y, const gp_Pnt& projectionAnchor) const;

    void clearDragState();

    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;
    HullModel* m_model;

    Mode m_mode = Mode::View;

    // Stable model identity for the point owned by the active drag.
    std::optional<int> m_activePointId;

    // Original model-space position restored when the active drag is cancelled.
    gp_Pnt m_dragStartModelPosition;

    // Previous cursor sample in screen space. This is intentionally separate
    // from m_dragStartModelPosition, which is a 3D model-space coordinate.
    ScreenPoint m_previousScreenPosition{0, 0};

    // Model-space anchor used by the current projection calculation.
    gp_Pnt m_projectionAnchor;

    // // Accumulated total displacement for the drag in progress (committed on release)
    // Graphic3d_Vec3 m_accumulated { 0.f, 0.f, 0.f };

    // // Command stacks for undo/redo
    // std::stack<std::unique_ptr<MoveCommand>> m_undoStack;
    // std::stack<std::unique_ptr<MoveCommand>> m_redoStack;
};
