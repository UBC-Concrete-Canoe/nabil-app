#pragma once

#include <AIS_InteractiveObject.hxx>
#include <gp_Pnt.hxx>

#include "core/ControlPoint.h"

class VisualPoint : public AIS_InteractiveObject
{
	DEFINE_STANDARD_RTTIEXT(VisualPoint, AIS_InteractiveObject)

public:
	explicit VisualPoint(const ControlPoint* controlPoint);

	void synchronize();
	bool setSelectedStyle(bool selected);
	const gp_Pnt& point() const { return m_cachedPoint; }

protected:
	void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
	             const Handle(Prs3d_Presentation)& thePrs,
	             const Standard_Integer theMode) override;

	void ComputeSelection(const Handle(SelectMgr_Selection)& theSelection,
	                      const Standard_Integer theMode) override;

private:
	gp_Pnt pointFromModel() const;
	bool hasMoved(const gp_Pnt& p) const;

	const ControlPoint* m_controlPoint = nullptr;
	gp_Pnt m_cachedPoint;
	bool m_isSelected = false;
};

DEFINE_STANDARD_HANDLE(VisualPoint, AIS_InteractiveObject)
