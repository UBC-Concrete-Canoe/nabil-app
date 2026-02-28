#pragma once

#include <cstddef>
#include <vector>
#include "ControlPoint.h"

class HullModel
{
public:
	void addPoint(const ControlPoint& p);
	std::size_t pointCount() const;
	ControlPoint* pointAt(std::size_t index);
	const ControlPoint* pointAt(std::size_t index) const;

private:
	std::vector<ControlPoint> points;
};
