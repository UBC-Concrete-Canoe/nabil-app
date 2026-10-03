#pragma once

#include "core/HullModel.h"

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Standard_Handle.hxx>

#include <memory>
#include <unordered_map>

/**
 * @brief Renders every control-mesh edge as an independently updateable shape.
 */
class WireframeManager : public IHullModelObserver
{
public:
	/**
	 * @brief Observe a hull model and render its edges in an OCCT context.
	 * @param context OCCT interactive context used for display.
	 * @param hullModel Model whose control edges are rendered.
	 */
	WireframeManager(
		const Handle(AIS_InteractiveContext)& context,
		const std::shared_ptr<HullModel>& hullModel
	);

	//! Remove rendered edges and detach from the model observer list.
	~WireframeManager() override;

	//! Rebuild all rendered edges from the current model topology.
	void build();

	//! Compatibility alias for callers from the lattice prototype.
	void BuildLattice() { build(); }

	//! Refresh edges incident to a moved control point.
	void onControlPointMoved(int pointId) override;
	//! Render a newly added control edge.
	void onEdgeAdded(int edgeId) override;
	//! Remove a deleted control edge from the viewer.
	void onEdgeRemoved(int edgeId) override;
	//! Rebuild all rendered edges after a model reset.
	void onModelReset() override;

private:
	void updateEdge(int edgeId);
	void removeRenderedEdge(int edgeId, bool updateViewer);

	Handle(AIS_InteractiveContext) m_context;
	std::shared_ptr<HullModel> m_hullModel;
	std::unordered_map<int, Handle(AIS_Shape)> m_edgeShapes;
	bool m_isVisible = true;
};
