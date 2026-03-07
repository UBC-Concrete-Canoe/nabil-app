#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include <gp_Pnt.hxx> // OCCT 3D Point primitive

/**
 * @brief Lightweight object representing a single vertex in the hull lattice.
 */
class ControlPoint
{
public:
	ControlPoint(int id, const gp_Pnt& pos, double weight = 1.0);

	// Coordinate Accessors
	const gp_Pnt& getPosition() const { return m_position; }
	void setPosition(const gp_Pnt& pos);

	// NURBS Properties
	double getWeight() const { return m_weight; }
	void setWeight(double w);

	// State Management
	int getId() const { return m_id; }
	bool isDirty() const { return m_isDirty; }
	void clearDirty() { m_isDirty = false; }

private:
	int m_id;
	gp_Pnt m_position;
	double m_weight;
	bool m_isDirty; // Observer flag for change tracking
};

#endif // CONTROLPOINT_H