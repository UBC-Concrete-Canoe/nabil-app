#pragma once

#include "ControlPoint.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

struct ControlEdge
{
	//! Stable edge identifier.
	int id;
	//! Identifier of the first endpoint.
	int firstPointId;
	//! Identifier of the second endpoint.
	int secondPointId;
};

struct ControlFace
{
	//! Stable face identifier.
	int id;
	//! Ordered boundary point identifiers.
	std::vector<int> pointIds;
	//! Edge identifiers corresponding to the point loop.
	std::vector<int> edgeIds;
};

struct EdgeSplitResult
{
	//! Identifier of the inserted midpoint.
	int pointId;
	//! Identifier of the edge from the first original endpoint to the midpoint.
	int firstEdgeId;
	//! Identifier of the edge from the midpoint to the second original endpoint.
	int secondEdgeId;
	//! Faces recreated after the edge split.
	std::vector<int> replacementFaceIds;
};

class IHullModelObserver
{
public:
	//! Virtual destructor for observer implementations.
	virtual ~IHullModelObserver() = default;
	//! Called after a control point is added.
	virtual void onPointAdded(int) {}
	//! Called after a control point position changes.
	virtual void onControlPointMoved(int) {}
	//! Called after a control edge is added.
	virtual void onEdgeAdded(int) {}
	//! Called after a control edge is removed.
	virtual void onEdgeRemoved(int) {}
	//! Called after a control face is added.
	virtual void onFaceAdded(int) {}
	//! Called after a control face is removed.
	virtual void onFaceRemoved(int) {}
	//! Called after the model is reset.
	virtual void onModelReset() {}
};

/**
 * @brief Editable, non-grid control mesh for a hull.
 *
 * Vertices, edges, and polygon faces have stable IDs. Adjacency maps make
 * topology edits independent of any rectangular U/V layout.
 */
class HullModel
{
public:
	//! Construct an empty control mesh.
	HullModel() = default;

	/**
	 * @brief Add a control point.
	 * @param position Point position in model space.
	 * @param weight Rational control-point weight.
	 * @return Stable identifier assigned to the point.
	 */
	int addPoint(const gp_Pnt& position, double weight = 1.0);

	/**
	 * @brief Add an edge between two existing points.
	 * @return Stable identifier assigned to the edge.
	 * @throws std::invalid_argument for identical or duplicate endpoints.
	 * @throws std::out_of_range when either point does not exist.
	 */
	int addEdge(int firstPointId, int secondPointId);

	/**
	 * @brief Add a polygon face and create any missing boundary edges.
	 * @param pointIds Ordered point identifiers forming the face boundary.
	 * @return Stable identifier assigned to the face.
	 */
	int addFace(const std::vector<int>& pointIds);

	/**
	 * @brief Update a point position and notify observers.
	 * @param pointId Identifier of the point to update.
	 * @param newPosition Replacement position in model space.
	 */
	void updatePoint(int pointId, const gp_Pnt& newPosition);

	/**
	 * @brief Replace an edge by two edges meeting at a new midpoint vertex.
	 *
	 * Every adjacent face loop is rewritten to include the new vertex.
	 */
	EdgeSplitResult splitEdge(int edgeId);

	/**
	 * @brief Insert an edge between two non-adjacent vertices of one face.
	 *
	 * The original face is replaced by the two resulting polygon faces.
	 * @return IDs of the two replacement faces.
	 */
	std::pair<int, int> insertEdge(
		int faceId,
		int firstPointId,
		int secondPointId
	);

	/**
	 * @brief Access a mutable control point by identifier.
	 * @throws std::out_of_range when the point does not exist.
	 */
	ControlPoint& point(int pointId);

	/**
	 * @brief Access an immutable control point by identifier.
	 * @throws std::out_of_range when the point does not exist.
	 */
	const ControlPoint& point(int pointId) const;

	/**
	 * @brief Access an edge by identifier.
	 * @throws std::out_of_range when the edge does not exist.
	 */
	const ControlEdge& edge(int edgeId) const;

	/**
	 * @brief Access a face by identifier.
	 * @throws std::out_of_range when the face does not exist.
	 */
	const ControlFace& face(int faceId) const;

	//! Return the number of control points.
	std::size_t pointCount() const { return m_pointOrder.size(); }
	//! Return the number of control edges.
	std::size_t edgeCount() const { return m_edgeOrder.size(); }
	//! Return the number of control faces.
	std::size_t faceCount() const { return m_faceOrder.size(); }

	//! Return a mutable point by insertion-order index, or null when out of range.
	ControlPoint* pointAt(std::size_t index);
	//! Return an immutable point by insertion-order index, or null when out of range.
	const ControlPoint* pointAt(std::size_t index) const;
	//! Return an edge by insertion-order index, or null when out of range.
	const ControlEdge* edgeAt(std::size_t index) const;
	//! Return a face by insertion-order index, or null when out of range.
	const ControlFace* faceAt(std::size_t index) const;

	/**
	 * @brief Return the edges incident to a point.
	 * @throws std::out_of_range when the point does not exist.
	 */
	std::vector<int> incidentEdgeIds(int pointId) const;

	/**
	 * @brief Return the faces adjacent to an edge.
	 * @throws std::out_of_range when the edge does not exist.
	 */
	std::vector<int> adjacentFaceIds(int edgeId) const;

	/**
	 * @return Empty when all references, loops, and manifold constraints hold.
	 */
	std::vector<std::string> validateTopology() const;

	/**
	 * @brief Register an observer if it is not already registered.
	 * @param observer Non-owning observer pointer.
	 */
	void addObserver(IHullModelObserver* observer);

	/**
	 * @brief Remove an observer if it is registered.
	 * @param observer Observer pointer to remove.
	 */
	void removeObserver(IHullModelObserver* observer);

private:
	int findEdgeId(int firstPointId, int secondPointId) const;
	void removeEdge(int edgeId);
	void removeFace(int faceId);
	bool faceLoopExists(const std::vector<int>& pointIds) const;

	static std::vector<int> pathAlongFace(
		const std::vector<int>& loop,
		std::size_t start,
		std::size_t finish
	);

	int m_nextPointId = 0;
	int m_nextEdgeId = 0;
	int m_nextFaceId = 0;

	// unordered_map references remain valid across rehash, keeping VisualPoint
	// bindings stable while new control points are inserted.
	std::unordered_map<int, ControlPoint> m_points;
	std::unordered_map<int, ControlEdge> m_edges;
	std::unordered_map<int, ControlFace> m_faces;
	std::vector<int> m_pointOrder;
	std::vector<int> m_edgeOrder;
	std::vector<int> m_faceOrder;

	std::unordered_map<int, std::unordered_set<int>> m_pointToEdges;
	std::unordered_map<int, std::unordered_set<int>> m_edgeToFaces;
	std::vector<IHullModelObserver*> m_observers;
};
