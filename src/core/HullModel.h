#pragma once

#include <vector>
#include "ControlPoint.h"

class HullModel
{
public:
    void addPoint(const ControlPoint &p);

private:
    std::vector<ControlPoint> points;
};
