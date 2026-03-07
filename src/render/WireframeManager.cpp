// filepath: c:\Users\RemoteAccess\Documents\ConcreteCanoe\placeholder-software\src\render\WireframeManager.cpp

#include "WireframeManager.h"
#include <AIS_InteractiveContext.hxx>
#include <AIS_Polyline.hxx>

WireframeManager::WireframeManager(const Handle(AIS_InteractiveContext)& context, const std::shared_ptr<HullModel>& hullModel)
    : m_context(context), m_hullModel(hullModel)
{
}

WireframeManager::~WireframeManager()
{
}

void WireframeManager::BuildLattice()
{
    // TODO: Implement lattice building from hull model
}

void WireframeManager::onControlPointMoved(int uIndex, int vIndex)
{
    UpdateURowPolylines(uIndex);
    UpdateVRowPolylines(vIndex);
}

void WireframeManager::UpdateURowPolylines(int uIndex)
{
    // TODO: Update AIS_Polyline objects for U row at uIndex
}

void WireframeManager::UpdateVRowPolylines(int vIndex)
{
    // TODO: Update AIS_Polyline objects for V row at vIndex
}