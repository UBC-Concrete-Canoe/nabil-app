#include "VisualPoint.h"
#include "render/OcctStyleMapper.h"

#include <Graphic3d_ArrayOfPoints.hxx>
#include <Prs3d_Drawer.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Prs3d_Presentation.hxx>
#include <Quantity_Color.hxx>
#include <Select3D_SensitivePoint.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <SelectMgr_Selection.hxx>

IMPLEMENT_STANDARD_RTTIEXT(VisualPoint, AIS_InteractiveObject)

namespace
{
constexpr double kSyncEpsilon = 1.0e-7;
} // namespace

VisualPoint::VisualPoint(const ControlPoint* controlPoint, const ControlPointVisualStyle& style)
  : m_controlPoint(controlPoint)
  , m_cachedPoint(pointFromModel())
  , m_style(style)
{
	SetInfiniteState(false);
	SetMutable(true);
	// Hover/selected looks are drawn by Compute() from m_isHovered/m_isSelected.
	// OCCT's automatic highlight colour override leaked onto every other point
	// in the planar views, so it is disabled for control points.
	SetAutoHilight(false);
	SetHilightMode(0);
	const Handle(Prs3d_Drawer) selectedDrawer =
		OcctStyleMapper::makePointDrawer(m_style.selectedColor, m_style.selectedMarkerScale);
	const Handle(Prs3d_Drawer) hoverDrawer =
		OcctStyleMapper::makePointDrawer(m_style.hoverColor, m_style.selectedMarkerScale);
	SetHilightAttributes(selectedDrawer);
	SetDynamicHilightAttributes(hoverDrawer);
}

void
VisualPoint::synchronize()
{
	const gp_Pnt current = pointFromModel();
	if (!hasMoved(current))
	{
		return;
	}

	m_cachedPoint = current;
	SetToUpdate();
	UpdateSelection();
}

bool
VisualPoint::setSelectedStyle(bool selected)
{
	if (m_isSelected == selected)
	{
		return false;
	}

	m_isSelected = selected;
	SetToUpdate();
	return true;
}

bool
VisualPoint::setHoveredStyle(bool hovered)
{
	if (m_isHovered == hovered)
	{
		return false;
	}

	m_isHovered = hovered;
	SetToUpdate();
	return true;
}

void
VisualPoint::Compute(
	const Handle(PrsMgr_PresentationManager) &,
	const Handle(Prs3d_Presentation) & thePrs,
	const int theMode
)
{
	thePrs->Clear();

	RgbColor color = m_style.defaultColor;
	double scale = m_style.defaultMarkerScale;
	if (m_isSelected)
	{
		color = m_style.selectedColor;
		scale = m_style.selectedMarkerScale;
	}
	else if (m_isHovered || theMode == 1)
	{
		color = m_style.hoverColor;
		scale = m_style.selectedMarkerScale;
	}

	Handle(Prs3d_PointAspect) pointAspect = OcctStyleMapper::makePointAspect(color, scale);
	myDrawer->SetPointAspect(pointAspect);
	thePrs->CurrentGroup()->SetGroupPrimitivesAspect(pointAspect->Aspect());

	Handle(Graphic3d_ArrayOfPoints) array = new Graphic3d_ArrayOfPoints(1);
	array->AddVertex(m_cachedPoint);
	thePrs->CurrentGroup()->AddPrimitiveArray(array);
}

void
VisualPoint::ComputeSelection(
	const Handle(SelectMgr_Selection) & theSelection,
	const int
)
{
	// Reuse one owner: a fresh owner per recompute would drop the selection
	// every time the point is redisplayed (e.g. when its selected style changes).
	if (m_owner.IsNull())
	{
		m_owner = new SelectMgr_EntityOwner(this, 10);
	}
	Handle(Select3D_SensitivePoint) sensitivePoint =
		new Select3D_SensitivePoint(m_owner, m_cachedPoint);
	theSelection->Add(sensitivePoint);
}

gp_Pnt
VisualPoint::pointFromModel() const
{
	if (m_controlPoint == nullptr)
	{
		return gp_Pnt(0.0, 0.0, 0.0);
	}

	return m_controlPoint->getPosition();
}

bool
VisualPoint::hasMoved(const gp_Pnt& p) const
{
	return p.Distance(m_cachedPoint) > kSyncEpsilon;
}
