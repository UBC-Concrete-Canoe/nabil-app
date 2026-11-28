#include "app/MoveTool.h"

#include <cmath>
#include <stdexcept>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Graphic3d_Vec3.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <gp_Dir.hxx>
#include <gp_Lin.hxx>
#include <gp_Pln.hxx>
#include <gp_Vec.hxx>


MoveTool::MoveTool(Handle(AIS_InteractiveContext) context, Handle(V3d_View) view, HullModel* model)
     : m_context(context), m_view(view), m_model(model)
{
}

bool MoveTool::onMousePress(int x, int y)
{
    m_context->MoveTo(x, y, m_view, Standard_True);
    m_context->SelectDetected();

    m_context->InitSelected();
    if (!m_context->MoreSelected())
    {
        m_context->ClearSelected(Standard_True);
        clearDragState();
        return false;
    }

    Handle(AIS_InteractiveObject) picked = m_context->SelectedInteractive();
    Handle(VisualPoint) vp = Handle(VisualPoint)::DownCast(picked);

    if (vp.IsNull())
    {
        // Clicked something that isn't a VisualPoint - ignore.
        clearDragState();
        return false;
    }

    // The visual owns the exact model identity, so coincident points do not
    // need to be disambiguated by comparing their coordinates.
    return beginDrag(vp->modelPointId(), {x, y});
}

bool MoveTool::onMouseMove(int x, int y)
{
    return updateDrag({x, y});
}

void MoveTool::onMouseRelease(int x, int y)
{
    finishDrag({x, y});
    // if (std::abs(m_accumulated.x()) > kEpsilon ||
    //     std::abs(m_accumulated.y()) > kEpsilon ||
    //     std::abs(m_accumulated.z()) > kEpsilon)
    // {
    //     // The model is already at the final position; store the total
    //     // displacement so MoveCommand::undo() can reverse it in one step.
    //     auto cmd = std::make_unique<MoveCommand>(
    //         m_model,
    //         m_selectedIndex,
    //         static_cast<double>(m_accumulated.x()),
    //         static_cast<double>(m_accumulated.y()),
    //         static_cast<double>(m_accumulated.z()));

    //     m_undoStack.push(std::move(cmd));

    //     // A new drag invalidates the redo history.
    //     while (!m_redoStack.empty())
    //         m_redoStack.pop();
    // }
}

bool MoveTool::beginDrag(int pointId, ScreenPoint screenPosition)
{
    if (isActive() || m_model == nullptr || m_view.IsNull())
    {
        return false;
    }

    try
    {
        m_dragStartModelPosition = m_model->point(pointId).getPosition();
    }
    catch (const std::out_of_range&)
    {
        clearDragState();
        return false;
    }

    m_activePointId = pointId;
    m_mode = Mode::Select;

    // Keep the actual press location as the first screen-space sample. A point
    // may be picked anywhere within its selection tolerance, so deriving this
    // from the point's visual centre could introduce a jump on the first move.
    m_previousScreenPosition = screenPosition;

    // Projection still needs a 3D model-space anchor, which starts at the
    // point position captured for cancellation.
    m_projectionAnchor = m_dragStartModelPosition;

    return true;
}

bool MoveTool::updateDrag(ScreenPoint screenPosition)
{
    if (!isActive() || m_model == nullptr)
    {
        return false;
    }

    if (m_mode == Mode::Select)
    {
        m_mode = Mode::Move;
    }

    // Project consecutive cursor samples onto the camera-facing plane through
    // the current projection anchor. Graphic3d_Vec3 carries world coordinates.
    Graphic3d_Vec3 previousProjectedPosition = projectOntoPlane(
        m_previousScreenPosition.x, m_previousScreenPosition.y, m_projectionAnchor);
    Graphic3d_Vec3 currentProjectedPosition = projectOntoPlane(
        screenPosition.x, screenPosition.y, m_projectionAnchor);

    // Per-frame delta as a Graphic3d_Vec3.
    Graphic3d_Vec3 worldDelta(
        currentProjectedPosition.x() - previousProjectedPosition.x(),
        currentProjectedPosition.y() - previousProjectedPosition.y(),
        currentProjectedPosition.z() - previousProjectedPosition.z());

    gp_Pnt currentModelPosition;
    try
    {
        // Resolve by stable ID for each update instead of retaining a pointer
        // to model-owned point storage for the lifetime of the drag.
        currentModelPosition = m_model->point(*m_activePointId).getPosition();
    }
    catch (const std::out_of_range&)
    {
        clearDragState();
        return false;
    }

    gp_Pnt updatedModelPosition(
        currentModelPosition.X() + static_cast<double>(worldDelta.x()),
        currentModelPosition.Y() + static_cast<double>(worldDelta.y()),
        currentModelPosition.Z() + static_cast<double>(worldDelta.z()));

    // Go through the model so observers (e.g. WireframeManager) are notified.
    m_model->updatePoint(*m_activePointId, updatedModelPosition);

    // Advance the anchor to the new position for the next frame.
    m_projectionAnchor = updatedModelPosition;
    m_previousScreenPosition = screenPosition;

    return true;  // suppress orbit
}

std::optional<MoveResult> MoveTool::finishDrag(ScreenPoint screenPosition)
{
    if (!isActive() || m_model == nullptr)
    {
        return std::nullopt;
    }

    // A release can arrive at a position not delivered by mouseMoveEvent, so
    // apply that final cursor sample before recording the completed movement.
    if (screenPosition.x != m_previousScreenPosition.x
        || screenPosition.y != m_previousScreenPosition.y)
    {
        updateDrag(screenPosition);
    }

    if (!isActive())
    {
        return std::nullopt;
    }

    const int completedPointId = *m_activePointId;
    gp_Pnt finalModelPosition;
    try
    {
        finalModelPosition = m_model->point(completedPointId).getPosition();
    }
    catch (const std::out_of_range&)
    {
        clearDragState();
        return std::nullopt;
    }

    MoveResult completedMove{
        completedPointId, m_dragStartModelPosition, finalModelPosition};
    clearDragState();
    return completedMove;
}

bool MoveTool::cancelDrag()
{
    if (!isActive() || m_model == nullptr)
    {
        return false;
    }

    try
    {
        // Restore through HullModel so renderers and other observers receive
        // the same movement notification as they do during an ordinary drag.
        m_model->updatePoint(*m_activePointId, m_dragStartModelPosition);
    }
    catch (const std::out_of_range&)
    {
        clearDragState();
        return false;
    }

    clearDragState();
    return true;
}

void MoveTool::clearDragState()
{
    // OCCT selection is intentionally left intact; only movement ownership ends.
    m_mode = Mode::View;
    m_activePointId.reset();
}

// void MoveTool::undo()
// {
//     if (m_undoStack.empty())
//         return;

//     auto& cmd = m_undoStack.top();
//     cmd->undo();
//     m_redoStack.push(std::move(m_undoStack.top()));
//     m_undoStack.pop();
// }

// void MoveTool::redo()
// {
//     if (m_redoStack.empty())
//         return;

//     auto& cmd = m_redoStack.top();
//     cmd->execute();
//     m_undoStack.push(std::move(m_redoStack.top()));
//     m_redoStack.pop();
// }

Graphic3d_Vec3 MoveTool::projectOntoPlane(
    int x, int y, const gp_Pnt& projectionAnchor) const
{
    // Shoot a ray from the camera through the screen pixel.
    Standard_Real xv, yv, zv, vx, vy, vz;
    m_view->ConvertWithProj(x, y, xv, yv, zv, vx, vy, vz);
    gp_Pnt rayOrigin(xv, yv, zv);
    gp_Dir rayDir(vx, vy, vz);

    // Plane through the projection anchor, normal = camera direction.
    gp_Dir camDir = m_view->Camera()->Direction();
    gp_Vec n(camDir);
    gp_Vec orig(rayOrigin.X(), rayOrigin.Y(), rayOrigin.Z());
    gp_Vec anchorVec(
        projectionAnchor.X(), projectionAnchor.Y(), projectionAnchor.Z());

    // Ray-plane intersection: t = (n · (anchor - origin)) / (n · dir)
    double denom = n.Dot(gp_Vec(rayDir));
    double t = 0.0;
    if (std::abs(denom) > 1e-10)
        t = n.Dot(anchorVec - orig) / denom;

    // Return the hit point as a Graphic3d_Vec3 for use in delta arithmetic.
    return Graphic3d_Vec3(
        static_cast<float>(rayOrigin.X() + t * rayDir.X()),
        static_cast<float>(rayOrigin.Y() + t * rayDir.Y()),
        static_cast<float>(rayOrigin.Z() + t * rayDir.Z()));
}
