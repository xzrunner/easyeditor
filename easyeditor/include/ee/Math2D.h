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

	// To check if test at middle of bound0 and bound1
	static bool IsBetween(float bound0, float bound1, float test);

	// To check if the 2 point overlapped with each other.
	static bool IsTheSamePos(const sm::vec2& p0, const sm::vec2& p1, const float tolerance = FLT_EPSILON);

	// Attention: It can't handle the point on segment.
	//			  Before use it must make sure the point is not on the segment.
	static bool IsPointAtSegmentLeft(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e);

	// To check if angle a-center-b is acute.
	static bool IsAcuteAngle(const sm::vec2& a, const sm::vec2& center, const sm::vec2& b);

	static bool IsRectIntersectRect(const sm::rect& r0, const sm::rect& r1);

	// To check if r1 in r0
	static bool IsRectContainRect(const sm::rect& r0, const sm::rect& r1);

	// To check if the segment intersect with the sm::rect.
	static bool IsSegmentIntersectRect(float lx1, float ly1, float lx2, float ly2,
		float ax1, float ay1, float ax2, float ay2);
	static bool IsSegmentIntersectRect(const sm::vec2& b, const sm::vec2& e, const sm::rect& aabb);

	static bool IsPolygonIntersectPolygon(const std::vector<sm::vec2>& poly0, const std::vector<sm::vec2>& poly1);
	static bool IsPolygonInPolygon(const std::vector<sm::vec2>& in, const std::vector<sm::vec2>& out);

	static bool IsPolygonIntersectRect(const std::vector<sm::vec2>& poly, const sm::rect& rect);

	static bool IsSegmentIntersectPolyline(const sm::vec2& s, const sm::vec2& e, const std::vector<sm::vec2>& poly);

	static bool IsPolylineIntersectRect(const std::vector<sm::vec2>& polyline, bool isLoop, const sm::rect& aabb);
	static bool isPolylineIntersectPolylinI(const std::vector<sm::vec2>& polyline0, const std::vector<sm::vec2>& polyline1);

	static bool IsCircleIntersectRect(const sm::vec2& center, float radius, const sm::rect& aabb);

	static bool IsPointInTriangle(const sm::vec2& p, const sm::vec2& a, const sm::vec2& b, const sm::vec2& c);

	static bool IsPointInRect(const sm::vec2& pos, const sm::rect& aabb);

	static bool IsPointInRect(const sm::vec2& pos, const sm::vec2& center, float hw, float hh);

	static bool IsPointInArea(const sm::vec2& pos, const std::vector<sm::vec2>& area);
	static bool IsPointOnPolyline(const sm::vec2& pos, const std::vector<sm::vec2>& poly);

	static bool IsPointInConvexHull(const sm::vec2& pos, const std::vector<sm::vec2>& hull);

	static bool IsPointInCircle(const sm::vec2& pos, const sm::vec2& center, float radius);

	static bool IsTwoLineParallel(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1);
	static bool IsTwoSegmentIntersect(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1);

	// To check if the point in the triangle.
	// return: [0] p is on the edge t0-t1		[1] p is on the edge t1-t2
	//		   [2] p is on the edge t2-t0
	//		   [4] p is in the triangle			[-1] p is outside
	static int CheckPosInTriangle(const sm::vec2& p, const sm::vec2& t0, const sm::vec2& t1, const sm::vec2& t2);

	// To check angle a-center-b turn left or right.
	static bool IsTurnLeft(const sm::vec2& a, const sm::vec2& center, const sm::vec2& b);
	static bool IsTurnRight(const sm::vec2& a, const sm::vec2& center, const sm::vec2& b);

	//////////////////////////////////////////////////////////////////////////
	//
	// Compute some geographic value, return type is float.
	//
	//////////////////////////////////////////////////////////////////////////

	// Get the coordinate of a segment.
	static float FindXOnSeg(float x1, float y1, float x2, float y2, float y);
	static float FindYOnSeg(float x1, float y1, float x2, float y2, float x);
	static float FindXOnSeg(const sm::vec2& b, const sm::vec2& e, float y);
	static float FindYOnSeg(const sm::vec2& b, const sm::vec2& e, float x);

	static float GetDistance(const sm::vec2& p0, const sm::vec2& p1);

	static float GetDistanceSquare(const sm::vec2& p0, const sm::vec2& p1);

	static float GetDisPointToStraightLine(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e);

	static float GetDisPointToSegment(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e);

	static float GetDisPointToPolyline(const sm::vec2& p, const std::vector<sm::vec2>& polyline, size_t* iPos = NULL);

	static float GetDisPointToMultiLines(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& multiLines, size_t* iLine = NULL, size_t* iPos = NULL);

	static float GetDisPointToMultiPos(const sm::vec2& p, const std::vector<sm::vec2>& pos, size_t* index = NULL);

	static float GetDisPointToMultiPos(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& pos, size_t* index0 = NULL, size_t* index1 = NULL);

	static float GetAngle(const sm::vec2& center, const sm::vec2& pa, const sm::vec2& pb);

	static float GetAngleInDirection(const sm::vec2& center, const sm::vec2& start, const sm::vec2& end);

	static float GetLineAngle(const sm::vec2& s, const sm::vec2& e);

	static float GetPolygonArea(const std::vector<sm::vec2>& polygon);
	static float GetTriangleArea(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2);

	static float GetPolygonPerimeter(const std::vector<sm::vec2>& poly);

	// Get the cross point of two segment.
	// If they are not crossed, direct return false withnot compute the cross point.
	static bool GetTwoLineCross(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1, sm::vec2* cross);

	static bool GetTwoSegmentCross(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1, sm::vec2* cross);

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

	// Get the foot of out at line(s, e).
	// Is return -1 the foot is outside the line(s, e), return 0 the foot on the line(s, e).
	static int GetFootOfPerpendicular(const sm::vec2& s, const sm::vec2& e, const sm::vec2& out, sm::vec2* foot);

	// Get the nearest position of Line to Point.
	static void GetNearestPosOnLineToPoint(const sm::vec2& p, const std::vector<sm::vec2>& l, sm::vec2* nearest, size_t* index);

	static sm::vec2 RotateVector(const sm::vec2& v, float rad);

	static sm::vec2 RotateVectorRightAngle(const sm::vec2& v, bool isTurnLeft);

	static float TransLen(float len, const sm::mat4& m);
	static sm::vec2 TransVector(const sm::vec2& v, const sm::mat4& m);
	static void TransVertices(const sm::mat4& mt, const std::vector<sm::vec2>& src,
		std::vector<sm::vec2>& dst);

	static sm::vec2 TransCoordsLocalToWorld(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& local);

	static sm::vec2 TransCoordsWorldToLocal(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& world);

	static void GetMBR(const std::vector<sm::vec2>& pos, sm::rect* mbr);

	static sm::vec2 GetTriGravityCenter(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2);

	//////////////////////////////////////////////////////////////////////////

	static void RemoveDuplicatePoints(const std::vector<sm::vec2>& src, std::vector<sm::vec2>& dst);

	// 1 2
	// 0 3
	static void ComputeQuadNodes(const sm::vec2& center, float angle, 
		float xScale, float yScale, float width, float height, sm::vec2 quad[4]);

	static void SideOffsetSegment(const sm::vec2& s, const sm::vec2& e, bool toleft, float dis,
		sm::vec2& ds, sm::vec2& de);

private:
	// if clockwise return true
	static bool IsPolygonColckwise(const std::vector<sm::vec2>& poly);

	static int GetNextIdxInRing(int sz, int curr, int step);

	static bool IsTwoPointsSame(const sm::vec2& p0, const sm::vec2& p1);

}; // Math2D

}

#include "Math2D.inl"

#endif // _EASYEDITOR_MATH2D_H_