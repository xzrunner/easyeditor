#ifndef _EASYEDITOR_MATH2D_H_
#define _EASYEDITOR_MATH2D_H_

#include <SM_Matrix.h>
#include <SM_Rect.h>

#include <vector>

#include <float.h>

namespace ee
{

class Math2D
{
public:
	//////////////////////////////////////////////////////////////////////////
	//
	// Test if satisfy some conditions, return type is bool.
	//
	//////////////////////////////////////////////////////////////////////////

	static bool IsCircleIntersectRect(const sm::vec2& center, float radius, const sm::rect& aabb);

	static bool IsPointInTriangle(const sm::vec2& p, const sm::vec2& a, const sm::vec2& b, const sm::vec2& c);

	static bool IsTwoLineParallel(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1);

	// To check if the point in the triangle.
	// return: [0] p is on the edge t0-t1		[1] p is on the edge t1-t2
	//		   [2] p is on the edge t2-t0
	//		   [4] p is in the triangle			[-1] p is outside
	static int CheckPosInTriangle(const sm::vec2& p, const sm::vec2& t0, const sm::vec2& t1, const sm::vec2& t2);

	//////////////////////////////////////////////////////////////////////////
	//
	// Compute some geographic value, return type is float.
	//
	//////////////////////////////////////////////////////////////////////////

	static float GetDisPointToStraightLine(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e);

	static float GetDisPointToPolyline(const sm::vec2& p, const std::vector<sm::vec2>& polyline, size_t* iPos = NULL);

	static float GetDisPointToMultiLines(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& multiLines, size_t* iLine = NULL, size_t* iPos = NULL);

	static float GetDisPointToMultiPos(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& pos, size_t* index0 = NULL, size_t* index1 = NULL);

	static float GetPolygonArea(const std::vector<sm::vec2>& polygon);
	static float GetTriangleArea(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2);

	static float GetPolygonPerimeter(const std::vector<sm::vec2>& poly);

	//////////////////////////////////////////////////////////////////////////
	//
	// Test through input value, output the multiple situations.
	// Return type is int, different value denote different situations.
	//
	//////////////////////////////////////////////////////////////////////////

	// To check if the point on the segment.
	// return: [-1] outside		[0] on the middle		[1] overlap with startPos
	//		    [2] overlap with endPos		[3] the 3 point are overlap each other.
	static int TestPointOnSection(const sm::vec2& startPos, const sm::vec2& endPos, const sm::vec2& thdPos, float tolerance = FLT_EPSILON);

	// 3 point can decide a circle, get its center.
	// return: [0] success, [-1] not success, 3 points on a segment.
	static int GetCircumcenter(const sm::vec2& pos1, const sm::vec2& pos2, const sm::vec2& pos3, sm::vec2* center, float tolerance = FLT_EPSILON);

	//////////////////////////////////////////////////////////////////////////
	//
	// Get other value.
	//
	//////////////////////////////////////////////////////////////////////////

	// Get the nearest position of Line to Point.
	static void GetNearestPosOnLineToPoint(const sm::vec2& p, const std::vector<sm::vec2>& l, sm::vec2* nearest, size_t* index);

	static sm::vec2 TransCoordsLocalToWorld(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& local);

	static sm::vec2 TransCoordsWorldToLocal(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& world);

	static void GetMBR(const std::vector<sm::vec2>& pos, sm::rect* mbr);

	//////////////////////////////////////////////////////////////////////////

	// 1 2
	// 0 3
	static void ComputeQuadNodes(const sm::vec2& center, float angle, 
		float xScale, float yScale, float width, float height, sm::vec2 quad[4]);

	static void SideOffsetSegment(const sm::vec2& s, const sm::vec2& e, bool toleft, float dis,
		sm::vec2& ds, sm::vec2& de);

private:
	static int GetNextIdxInRing(int sz, int curr, int step);

	static bool IsTwoPointsSame(const sm::vec2& p0, const sm::vec2& p1);

}; // Math2D

}

#endif // _EASYEDITOR_MATH2D_H_