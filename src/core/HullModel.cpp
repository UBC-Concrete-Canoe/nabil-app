#include "HullModel.h"

HullModel::HullModel(int uCount, int vCount)
  : m_uCount(uCount)
  , m_vCount(vCount)
{
	// Pre-allocate control points with default values
	m_points.reserve(uCount * vCount);
	int idCounter = 0;
	for (int u = 0; u < uCount; ++u)
	{
		for (int v = 0; v < vCount; ++v)
		{
			m_points.emplace_back(idCounter++, gp_Pnt(0, 0, 0));
		}
	}

	// Build ID to index map
	for (size_t i = 0; i < m_points.size(); ++i)
	{
		m_idToIndexMap[m_points[i].getId()] = i;
	}
}

ControlPoint&
HullModel::getPoint(int u, int v)
{
	size_t index = getIndex(u, v);
	return m_points[index];
}

const ControlPoint&
HullModel::getPoint(int u, int v) const
{
	size_t index = getIndex(u, v);
	return m_points[index];
}

void
HullModel::updatePoint(int id, const gp_Pnt& newPos)
{
	auto it = m_idToIndexMap.find(id);
	if (it != m_idToIndexMap.end())
	{
		m_points[it->second].setPosition(newPos);
	}
}

// Handle(TColgp_Array2OfPnt) HullModel::toOcctPoles() const
// {
// 	Handle(TColgp_Array2OfPnt) poles = new TColgp_Array2OfPnt(1, m_uCount, 1, m_vCount);
// 	for (int u = 0; u < m_uCount; ++u)
// 	{
// 		for (int v = 0; v < m_vCount; ++v)
// 		{
// 			const ControlPoint& cp = getPoint(u, v);
// 			poles->SetValue(u + 1, v + 1, cp.getPosition());
// 		}
// 	}
// 	return poles;
// }

size_t
HullModel::getIndex(int u, int v) const
{
	return (u * m_vCount) + v;
}
