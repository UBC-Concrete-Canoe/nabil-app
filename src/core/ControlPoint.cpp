#include "ControlPoint.h"

ControlPoint::ControlPoint(int id, const gp_Pnt& pos, double weight)
  : m_id(id)
  , m_position(pos)
  , m_weight(weight)
  , m_isDirty(false)
{
}