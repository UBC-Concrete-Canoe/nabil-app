#pragma once

#include <AIS_InteractiveObject.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <gp_Pnt.hxx>

#include "core/ControlPoint.h"
#include "core/ControlPointVisualStyle.h"

/**
 * @brief OCCT visual for a single selectable control point.
 *
 * VisualPoint is an AIS interactive object bound to one model ControlPoint.
 * It renders as a marker in the 3D viewport and exposes OCCT selection
 * through a sensitive point.
 *
 * Lifetime note:
 * - The ControlPoint pointer is non-owning and must remain valid while this
 *   VisualPoint is displayed.
 */
class VisualPoint : public AIS_InteractiveObject
{
	DEFINE_STANDARD_RTTIEXT(VisualPoint, AIS_InteractiveObject)

public:
	/**
	 * @brief Construct a visual bound to a model control point.
	 * @param controlPoint Non-owning pointer to model data.
	 * @param style Renderer-agnostic style tokens for this point.
	 */
	explicit VisualPoint(const ControlPoint* controlPoint, const ControlPointVisualStyle& style);

	/**
	 * @brief Synchronize cached position with the model point.
	 *
	 * Compares the model coordinate to the cached rendered coordinate.
	 * If changed, marks this AIS object for update and refreshes selection.
	 */
	void synchronize();

	/**
	 * @brief Update selected-state style flag.
	 * @param selected True when this point is selected.
	 * @return True if state changed and visual redisplay is needed.
	 */
	bool setSelectedStyle(bool selected);

	/**
	 * @brief Update hovered-state style flag.
	 * @param hovered True when the cursor is over this point.
	 * @return True if state changed and visual redisplay is needed.
	 */
	bool setHoveredStyle(bool hovered);

	/**
	 * @brief Get current cached point in OCCT coordinates.
	 */
	const gp_Pnt& point() const { return m_cachedPoint; }

	//! @name OCCT self-highlighting hooks
	//! Auto-highlight is disabled, so OCCT calls these instead; the base
	//! implementations throw. Styling is handled by Compute(), so they are no-ops.
	//! @{
	void HilightSelected(
		const Handle(PrsMgr_PresentationManager) &,
		const NCollection_Sequence<Handle(SelectMgr_EntityOwner)>&
	) override
	{}
	void HilightOwnerWithColor(
		const Handle(PrsMgr_PresentationManager) &,
		const Handle(Prs3d_Drawer) &,
		const Handle(SelectMgr_EntityOwner) &
	) override
	{}
	void ClearSelected() override {}
	void ClearDynamicHighlight(const Handle(PrsMgr_PresentationManager) &) override {}
	//! @}

protected:
	/**
	 * @brief Build point presentation for display/highlight.
	 */
	void Compute(
		const Handle(PrsMgr_PresentationManager) & thePrsMgr,
		const Handle(Prs3d_Presentation) & thePrs,
		const int theMode
	) override;

	/**
	 * @brief Build selectable entity for ray picking.
	 */
	void ComputeSelection(
		const Handle(SelectMgr_Selection) & theSelection,
		const int theMode
	) override;

private:
	/**
	 * @brief Convert model coordinate to OCCT gp_Pnt.
	 */
	gp_Pnt pointFromModel() const;

	/**
	 * @brief Compare with epsilon to detect movement.
	 */
	bool hasMoved(const gp_Pnt& p) const;

	const ControlPoint* m_controlPoint = nullptr;
	//! Selection owner reused across selection recomputes so the context's
	//! selected state survives Redisplay()/UpdateSelection().
	Handle(SelectMgr_EntityOwner) m_owner;
	gp_Pnt m_cachedPoint;
	bool m_isSelected = false;
	bool m_isHovered = false;
	ControlPointVisualStyle m_style;
};

DEFINE_STANDARD_HANDLE(VisualPoint, AIS_InteractiveObject)
