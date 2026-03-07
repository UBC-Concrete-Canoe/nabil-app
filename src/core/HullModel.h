#ifndef HULLMODEL_H
#define HULLMODEL_H

#include <memory>
#include <unordered_map>
#include <vector>
#include "ControlPoint.h"

// OCCT Forward Declarations (keeps headers light)
#include <Standard_Handle.hxx>
#include <TColgp_Array2OfPnt.hxx>

/**
 * @brief High-performance container managing the 2D grid of control points.
 * Uses Row-Major indexing for cache efficiency.
 */
class HullModel
{
public:
	HullModel(int uCount, int vCount);

	// Getters with bounds checking
	ControlPoint& getPoint(int u, int v);
	const ControlPoint& getPoint(int u, int v) const;

	// Global Update
	void updatePoint(int id, const gp_Pnt& newPos);

	// OCCT Bridge for Surface Interpolation
	// Returns a 2D array of points for B-Spline construction (Ignore for now)
	// Handle(TColgp_Array2OfPnt) toOcctPoles() const;

	// Dimensions
	int getUCount() const { return m_uCount; }
	int getVCount() const { return m_vCount; }

private:
	// Helper for 1D indexing: index = (u * vCount) + v
	size_t getIndex(int u, int v) const;

	int m_uCount; // Number of control points in U direction
	int m_vCount; // Number of control points in V direction

	// The data layer
	std::vector<ControlPoint> m_points;

	// Mapping ID to index for fast global updates
	std::unordered_map<int, size_t> m_idToIndexMap;
};

#endif // HULLMODEL_H