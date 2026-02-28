#pragma once

#include <cstddef>
#include <vector>
#include "ControlPoint.h"

/**
 * @brief In-memory container for hull control points.
 *
 * Stores ordered control points used by rendering and future hull logic.
 */
class HullModel
{
public:
	/**
	 * @brief Append a control point to the model.
	 * @param p Control point value to add.
	 */
	void addPoint(const ControlPoint& p);

	/**
	 * @brief Get number of stored control points.
	 * @return Count of points in the model.
	 */
	std::size_t pointCount() const;

	/**
	 * @brief Access a control point by index.
	 * @param index Zero-based point index.
	 * @return Non-owning pointer to point, or nullptr if out of range.
	 */
	ControlPoint* pointAt(std::size_t index);

	/**
	 * @brief Access a control point by index (const overload).
	 * @param index Zero-based point index.
	 * @return Non-owning const pointer to point, or nullptr if out of range.
	 */
	const ControlPoint* pointAt(std::size_t index) const;

private:
	std::vector<ControlPoint> points;
};
