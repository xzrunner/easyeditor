#ifndef _EASYEDITOR_MATH2D_H_
#define _EASYEDITOR_MATH2D_H_

#include <vector>

#include <float.h>

namespace ee
{

class Matrix;
class Vector;
class Rect;

static const float PI = 3.1415926f;
//	static const float FLOAT_MAX = FLT_MAX;
//	static const float FLT_EPSILON = FLT_EPSILON;
static const float LARGE_EPSILON = 0.001f;
static const float TRANS_DEG_TO_RAD = PI / 180.0f;
static const float TRANS_RAD_TO_DEG = 180.0f / PI;

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
	static bool IsTheSamePos(const Vector& p0, const Vector& p1, const float tolerance = FLT_EPSILON);

	// Attention: It can't handle the point on segment.
	//			  Before use it must make sure the point is not on the segment.
	static bool IsPointAtSegmentLeft(const Vector& p, const Vector& s, const Vector& e);

	// To check if angle a-center-b is acute.
	static bool IsAcuteAngle(const Vector& a, const Vector& center, const Vector& b);

	static bool IsRectIntersectRect(const Rect& r0, const Rect& r1);

	// To check if r1 in r0
	static bool IsRectContainRect(const Rect& r0, const Rect& r1);

	// To check if the segment intersect with the Rect.
	static bool IsSegmentIntersectRect(float lx1, float ly1, float lx2, float ly2,
		float ax1, float ay1, float ax2, float ay2);
	static bool IsSegmentIntersectRect(const Vector& b, const Vector& e, const Rect& aabb);

	static bool IsPolygonIntersectPolygon(const std::vector<Vector>& poly0, const std::vector<Vector>& poly1);
	static bool IsPolygonInPolygon(const std::vector<Vector>& in, const std::vector<Vector>& out);

	static bool IsPolygonIntersectRect(const std::vector<Vector>& poly, const Rect& rect);

	static bool IsSegmentIntersectPolyline(const Vector& s, const Vector& e, const std::vector<Vector>& poly);

	static bool IsPolylineIntersectRect(const std::vector<Vector>& polyline, bool isLoop, const Rect& aabb);
	static bool isPolylineIntersectPolylinI(const std::vector<Vector>& polyline0, const std::vector<Vector>& polyline1);

	static bool IsCircleIntersectRect(const Vector& center, float radius, const Rect& aabb);

	static bool IsPointInTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c);

	static bool IsPointInRect(const Vector& pos, const Rect& aabb);

	static bool IsPointInRect(const Vector& pos, const Vector& center, float hw, float hh);

	static bool IsPointInArea(const Vector& pos, const std::vector<Vector>& area);
	static bool IsPointOnPolyline(const Vector& pos, const std::vector<Vector>& poly);

	static bool IsPointInConvexHull(const Vector& pos, const std::vector<Vector>& hull);

	static bool IsPointInCircle(const Vector& pos, const Vector& center, float radius);

	static bool IsTwoLineParallel(const Vector& s0, const Vector& e0, const Vector& s1, const Vector& e1);
	static bool IsTwoSegmentIntersect(const Vector& s0, const Vector& e0, const Vector& s1, const Vector& e1);

	// To check if the point in the triangle.
	// return: [0] p is on the edge t0-t1		[1] p is on the edge t1-t2
	//		   [2] p is on the edge t2-t0
	//		   [4] p is in the triangle			[-1] p is outside
	static int CheckPosInTriangle(const Vector& p, const Vector& t0, const Vector& t1, const Vector& t2);

	// To check angle a-center-b turn left or right.
	static bool IsTurnLeft(const Vector& a, const Vector& center, const Vector& b);
	static bool IsTurnRight(const Vector& a, const Vector& center, const Vector& b);

	//////////////////////////////////////////////////////////////////////////
	//
	// Compute some geographic value, return type is float.
	//
	//////////////////////////////////////////////////////////////////////////

	// Get the coordinate of a segment.
	static float FindXOnSeg(float x1, float y1, float x2, float y2, float y);
	static float FindYOnSeg(float x1, float y1, float x2, float y2, float x);
	static float FindXOnSeg(const Vector& b, const Vector& e, float y);
	static float FindYOnSeg(const Vector& b, const Vector& e, float x);

	static float GetDistance(const Vector& p0, const Vector& p1);

	static float GetDistanceSquare(const Vector& p0, const Vector& p1);

	static float GetDisPointToStraightLine(const Vector& p, const Vector& s, const Vector& e);

	static float GetDisPointToSegment(const Vector& p, const Vector& s, const Vector& e);

	static float GetDisPointToPolyline(const Vector& p, const std::vector<Vector>& polyline, size_t* iPos = NULL);

	static float GetDisPointToMultiLines(const Vector& p, const std::vector<std::vector<Vector> >& multiLines, size_t* iLine = NULL, size_t* iPos = NULL);

	static float GetDisPointToMultiPos(const Vector& p, const std::vector<Vector>& pos, size_t* index = NULL);

	static float GetDisPointToMultiPos(const Vector& p, const std::vector<std::vector<Vector> >& pos, size_t* index0 = NULL, size_t* index1 = NULL);

	static float GetAngle(const Vector& center, const Vector& pa, const Vector& pb);

	static float GetAngleInDirection(const Vector& center, const Vector& start, const Vector& end);

	static float GetLineAngle(const Vector& s, const Vector& e);

	static float GetPolygonArea(const std::vector<Vector>& polygon);
	static float GetTriangleArea(const Vector& p0, const Vector& p1, const Vector& p2);

	static float GetPolygonPerimeter(const std::vector<Vector>& poly);

	// Get the cross point of two segment.
	// If they are not crossed, direct return false withnot compute the cross point.
	static bool GetTwoLineCross(const Vector& s0, const Vector& e0, const Vector& s1, const Vector& e1, Vector* cross);

	static bool GetTwoSegmentCross(const Vector& s0, const Vector& e0, const Vector& s1, const Vector& e1, Vector* cross);

	//////////////////////////////////////////////////////////////////////////
	//
	// Test through input value, output the multiple situations.
	// Return type is int, different value denote different situations.
	//
	//////////////////////////////////////////////////////////////////////////

	// To check if the point on the segment.
	// return: [-1] outside		[0] on the middle		[1] overlap with startPos
	//		    [2] overlap with endPos		[3] the 3 point are overlap each other.
	static int TestPointOnSection(const Vector& startPos, const Vector& endPos, const Vector& thdPos, float tolerance = FLT_EPSILON);

	// 3 point can decide a circle, get its center.
	// return: [0] success, [-1] not success, 3 points on a segment.
	static int GetCircumcenter(const Vector& pos1, const Vector& pos2, const Vector& pos3, Vector* center, float tolerance = FLT_EPSILON);

	//////////////////////////////////////////////////////////////////////////
	//
	// Get other value.
	//
	//////////////////////////////////////////////////////////////////////////

	// Get the foot of out at line(s, e).
	// Is return -1 the foot is outside the line(s, e), return 0 the foot on the line(s, e).
	static int GetFootOfPerpendicular(const Vector& s, const Vector& e, const Vector& out, Vector* foot);

	// Get the nearest position of Line to Point.
	static void GetNearestPosOnLineToPoint(const Vector& p, const std::vector<Vector>& l, Vector* nearest, size_t* index);

	static Vector RotateVector(const Vector& v, float rad);

	static Vector RotateVectorRightAngle(const Vector& v, bool isTurnLeft);

	static Vector TransVector(const Vector& v, const Matrix& m);
	static void TransVertices(const Matrix& mt, const std::vector<Vector>& src,
		std::vector<Vector>& dst);

	static Vector TransCoordsLocalToWorld(const Vector& origin, const Vector& xDir, const Vector& local);

	static Vector TransCoordsWorldToLocal(const Vector& origin, const Vector& xDir, const Vector& world);

	static void GetMBR(const std::vector<Vector>& pos, Rect* mbr);

	static Vector GetTriGravityCenter(const Vector& p0, const Vector& p1, const Vector& p2);

	//////////////////////////////////////////////////////////////////////////

	static void RemoveDuplicatePoints(const std::vector<Vector>& src, std::vector<Vector>& dst);

	// 1 2
	// 0 3
	static void ComputeQuadNodes(const Vector& center, float angle, 
		float xScale, float yScale, float width, float height, Vector quad[4]);

	static void SideOffsetSegment(const Vector& s, const Vector& e, bool toleft, float dis,
		Vector& ds, Vector& de);

private:
	// if clockwise return true
	static bool IsPolygonColckwise(const std::vector<Vector>& poly);

	static int GetNextIdxInRing(int sz, int curr, int step);

	static bool IsTwoPointsSame(const Vector& p0, const Vector& p1);

}; // Math2D

}

#include "Math2D.inl"

#endif // _EASYEDITOR_MATH2D_H_