#include "HullModel.h"

void
HullModel::addPoint(const ControlPoint& p)
{
	points.push_back(p);
}

std::size_t
HullModel::pointCount() const
{
	return points.size();
}

ControlPoint*
HullModel::pointAt(std::size_t index)
{
	if (index >= points.size())
	{
		return nullptr;
	}

	return &points[index];
}

const ControlPoint*
HullModel::pointAt(std::size_t index) const
{
	if (index >= points.size())
	{
		return nullptr;
	}

	return &points[index];
}
