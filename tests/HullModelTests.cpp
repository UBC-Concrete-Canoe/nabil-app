#include "core/HullModel.h"

#include <gp_Pnt.hxx>
#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace
{
void
requireValid(const HullModel& model)
{
	const std::vector<std::string> errors = model.validateTopology();
	ASSERT_TRUE(errors.empty()) << "Topology validation failed: " << errors.front();
}
} // namespace

TEST(HullModel, CreateFaceAndAdjacency)
{
	HullModel model;
	const int p0 = model.addPoint(gp_Pnt(0, 0, 0));
	const int p1 = model.addPoint(gp_Pnt(10, 0, 0));
	const int p2 = model.addPoint(gp_Pnt(10, 10, 0));
	const int p3 = model.addPoint(gp_Pnt(0, 10, 0));

	const int faceId = model.addFace({ p0, p1, p2, p3 });
	EXPECT_EQ(model.pointCount(), 4) << "Expected four control points";
	EXPECT_EQ(model.edgeCount(), 4) << "A quad should create four edges";
	EXPECT_EQ(model.faceCount(), 1) << "Expected one control face";
	EXPECT_EQ(model.face(faceId).edgeIds.size(), 4) << "Face must retain its edge loop";
	requireValid(model);
}

TEST(HullModel, SplitEdgeRewritesFace)
{
	HullModel model;
	const int p0 = model.addPoint(gp_Pnt(0, 0, 0));
	const int p1 = model.addPoint(gp_Pnt(10, 0, 0));
	const int p2 = model.addPoint(gp_Pnt(10, 10, 0));
	const int p3 = model.addPoint(gp_Pnt(0, 10, 0));
	const int faceId = model.addFace({ p0, p1, p2, p3 });
	const int edgeId = model.face(faceId).edgeIds.front();

	const EdgeSplitResult result = model.splitEdge(edgeId);
	EXPECT_EQ(model.pointCount(), 5) << "Splitting an edge should add one point";
	EXPECT_EQ(model.edgeCount(), 5) << "Splitting a quad edge should add one net edge";
	EXPECT_EQ(model.faceCount(), 1) << "Splitting an edge should preserve adjacent face count";
	ASSERT_EQ(result.replacementFaceIds.size(), 1) << "One adjacent face should be rewritten";
	EXPECT_EQ(model.face(result.replacementFaceIds.front()).pointIds.size(), 5)
		<< "Rewritten face should contain the midpoint";
	EXPECT_LT(model.point(result.pointId).getPosition().Distance(gp_Pnt(5, 0, 0)), 1.0e-9)
		<< "Split point should be located at the edge midpoint";
	requireValid(model);
}

TEST(HullModel, InsertEdgeSplitsFace)
{
	HullModel model;
	const int p0 = model.addPoint(gp_Pnt(0, 0, 0));
	const int p1 = model.addPoint(gp_Pnt(10, 0, 0));
	const int p2 = model.addPoint(gp_Pnt(10, 10, 0));
	const int p3 = model.addPoint(gp_Pnt(0, 10, 0));
	const int faceId = model.addFace({ p0, p1, p2, p3 });
	const EdgeSplitResult split = model.splitEdge(model.face(faceId).edgeIds.front());

	const auto replacementFaces =
		model.insertEdge(split.replacementFaceIds.front(), split.pointId, p3);
	EXPECT_EQ(model.edgeCount(), 6) << "Inserting a diagonal should add one edge";
	EXPECT_EQ(model.faceCount(), 2) << "Inserting a diagonal should split one face into two";
	EXPECT_GE(model.face(replacementFaces.first).pointIds.size(), 3) << "First face is valid";
	EXPECT_GE(model.face(replacementFaces.second).pointIds.size(), 3) << "Second face is valid";
	requireValid(model);
}

TEST(HullModel, RejectNonManifoldEdge)
{
	HullModel model;
	const int a = model.addPoint(gp_Pnt(0, 0, 0));
	const int b = model.addPoint(gp_Pnt(10, 0, 0));
	const int c = model.addPoint(gp_Pnt(0, 10, 0));
	const int d = model.addPoint(gp_Pnt(0, -10, 0));
	const int e = model.addPoint(gp_Pnt(0, 0, 10));
	model.addFace({ a, b, c });
	model.addFace({ b, a, d });

	EXPECT_THROW(model.addFace({ a, b, e }), std::invalid_argument)
		<< "A third face on one edge must be rejected";
	EXPECT_EQ(model.faceCount(), 2) << "Rejected operation must not modify face topology";
	requireValid(model);
}
