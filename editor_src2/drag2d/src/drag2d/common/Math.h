#pragma once

#include <float.h>
#include <vector>

#include "common/Vector.h"
#include "common/Rect.h"

namespace love { class Matrix; }

namespace d2d
{
	static const float PI = 3.1415926f;
//	static const float FLOAT_MAX = FLT_MAX;
//	static const float FLT_EPSILON = FLT_EPSILON;
	static const float LARGE_EPSILON = 0.001f;
	static const float TRANS_DEG_TO_RAD = PI / 180.0f;
	static const float TRANS_RAD_TO_DEG = 180.0f / PI;
	static const float BOX2D_SCALE_FACTOR = 20.0f;

	class ChainShape;

	class Math
	{
	public:
		//////////////////////////////////////////////////////////////////////////
		//
		// Test if satisfy some conditions, return type is bool.
		//
		//////////////////////////////////////////////////////////////////////////

		// To check if test at middle of bound0 and bound1
		static bool isBetween(float bound0, float bound1, float test)
		{
			if (bound0 < bound1)
				return test < bound1 + FLT_EPSILON && test > bound0 - FLT_EPSILON;
			else
				return test < bound0 + FLT_EPSILON && test > bound1 - FLT_EPSILON;
		}

		// To check if the 2 point overlapped with each other.
		static bool isTheSamePos(const Vector& p0, const Vector& p1, const float tolerance = FLT_EPSILON)
		{
			return fabs(p0.x - p1.x) <= tolerance
				&& fabs(p0.y - p1.y) <= tolerance;
		}

		// Attention: It can't handle the point on segment.
		//			  Before use it must make sure the point is not on the segment.
		static bool isPointAtSegmentLeft(const Vector& p, const Vector& s, const Vector& e)
		{
			return (p.y - s.y) * (e.x - s.x) - (p.x - s.x) * (e.y - s.y) > FLT_EPSILON;
		}

		// To check if angle a-center-b is acute.
		static bool isAcuteAngle(const Vector& a, const Vector& center, const Vector& b);

		static bool isRectIntersectRect(const Rect& r0, const Rect& r1)
		{
			return !(r0.xMin > r1.xMax || r0.xMax < r1.xMin || r0.yMin > r1.yMax || r0.yMax < r1.yMin);
		}

		// To check if r1 in r0
		static bool isRectContainRect(const Rect& r0, const Rect& r1)
		{
			return r1.xMin >= r0.xMin && r1.xMax <= r0.xMax 
				&& r1.yMin >= r0.yMin && r1.yMax <= r0.yMax;
		}

		// To check if the segment intersect with the Rect.
		static bool isSegmentIntersectRect(float lx1, float ly1, float lx2, float ly2,
			float ax1, float ay1, float ax2, float ay2);
		static bool isSegmentIntersectRect(const Vector& b, const Vector& e, const Rect& aabb)
		{
			return isSegmentIntersectRect(b.x, b.y, e.x, e.y, aabb.xMin, aabb.yMin, aabb.xMax, aabb.yMax);
		}

		static bool isPolylineIntersectRect(const std::vector<Vector>& polyline, bool isLoop, const Rect& aabb);

		static bool isCircleIntersectRect(const Vector& center, float radius, const Rect& aabb);

		static bool isPointInTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c);

		static bool isPointInRect(const Vector& pos, const Rect& aabb)
		{
			return pos.x > aabb.xMin && pos.x < aabb.xMax
				&& pos.y > aabb.yMin && pos.y < aabb.yMax;
		}

 		static bool isPointInRect(const Vector& pos, const Vector& center, float hw, float hh)
 		{
 			return pos.x > center.x - hw && pos.x < center.x + hw 
 				&& pos.y > center.y - hh && pos.y < center.y + hh;
 		}

		static bool isPointInArea(const Vector& pos, const std::vector<Vector>& area);

		static bool isPointInConvexHull(const Vector& pos, const std::vector<Vector>& hull);

		static bool isPointInCircle(const Vector& pos, const Vector& center, float radius);

		// To check if the point in the triangle.
		// return: [0] p is on the edge t0-t1		[1] p is on the edge t1-t2
		//		   [2] p is on the edge t2-t0
		//		   [4] p is in the triangle			[-1] p is outside
		static int checkPosInTriangle(const Vector& p, const Vector& t0, const Vector& t1, const Vector& t2);

		//////////////////////////////////////////////////////////////////////////
		//
		// Compute some geographic value, return type is float.
		//
		//////////////////////////////////////////////////////////////////////////

		// Get the coordinate of a segment.
		static float findXOnSeg(float x1, float y1, float x2, float y2, float y)
		{
			if (y1 == y2) 
				return std::min(x1, x2);
			return (y - y2) * (x1 - x2) / (y1 - y2) + x2;
		}
		static float findYOnSeg(float x1, float y1, float x2, float y2, float x)
		{
			if (x1 == x2) 
				return std::min(y1, y2);
			return (x - x2) * (y1 - y2) / (x1 - x2) + y2;
		}
		static float findXOnSeg(const Vector& b, const Vector& e, float y)
		{
			return findXOnSeg(b.x, b.y, e.x, e.y, y);
		}
		static float findYOnSeg(const Vector& b, const Vector& e, float x)
		{
			return findYOnSeg(b.x, b.y, e.x, e.y, x);
		}

		static float getDistance(const Vector& p0, const Vector& p1)
		{
			const float dx = p0.x - p1.x,
				dy = p0.y - p1.y;
			return sqrt(dx * dx + dy * dy);
		}

		static float getDistanceSquare(const Vector& p0, const Vector& p1)
		{
			const float dx = p0.x - p1.x,
				dy = p0.y - p1.y;
			return dx * dx + dy * dy;
		}

		static float getDisPointToStraightLine(const Vector& p, const Vector& s, const Vector& e);

		static float getDisPointToSegment(const Vector& p, const Vector& s, const Vector& e);

		static float getDisPointToPolyline(const Vector& p, const std::vector<Vector>& polyline, size_t* iPos = NULL);

		static float getDisPointToMultiLines(const Vector& p, const std::vector<std::vector<Vector> >& multiLines, size_t* iLine = NULL, size_t* iPos = NULL);

		static float getDisPointToMultiPos(const Vector& p, const std::vector<Vector>& pos, size_t* index = NULL);

		static float getDisPointToMultiPos(const Vector& p, const std::vector<std::vector<Vector> >& pos, size_t* index0 = NULL, size_t* index1 = NULL);

		static float getAngle(const Vector& center, const Vector& pa, const Vector& pb);

		static float getAngleInDirection(const Vector& center, const Vector& start, const Vector& end);

		static float getLineAngle(const Vector& s, const Vector& e);

		//////////////////////////////////////////////////////////////////////////
		//
		// Test through input value, output the multiple situations.
		// Return type is int, different value denote different situations.
		//
		//////////////////////////////////////////////////////////////////////////

		// To check if the point on the segment.
		// return: [-1] outside		[0] on the middle		[1] overlap with startPos
		//		    [2] overlap with endPos		[3] the 3 point are overlap each other.
		static int testPointOnSection(const Vector& startPos, const Vector& endPos, const Vector& thdPos, float tolerance = FLT_EPSILON);

		// 3 point can decide a circle, get its center.
		// return: [0] success, [-1] not success, 3 points on a segment.
		static int getCircumcenter(const Vector& pos1, const Vector& pos2, const Vector& pos3, Vector* center, float tolerance = FLT_EPSILON);

		//////////////////////////////////////////////////////////////////////////
		//
		// Get other value.
		//
		//////////////////////////////////////////////////////////////////////////

		// Get the foot of out at line(s, e).
		// Is return -1 the foot is outside the line(s, e), return 0 the foot on the line(s, e).
		static int getFootOfPerpendicular(const Vector& s, const Vector& e, const Vector& out, Vector* foot);

		// Get the nearest position of Line to Point.
		static void getNearestPosOnLineToPoint(const Vector& p, const std::vector<Vector>& l, Vector* nearest, size_t* index);

		static Vector rotateVector(const Vector& v, float rad);

		static Vector rotateVectorRightAngle(const Vector& v, bool isTurnLeft);

		static Vector transVector(const Vector& v, const love::Matrix& m);

		static Vector transCoordsLocalToWorld(const Vector& origin, const Vector& xDir, const Vector& local);

		static Vector transCoordsWorldToLocal(const Vector& origin, const Vector& xDir, const Vector& world);

		static void getMBR(const std::vector<Vector>& pos, Rect* mbr);

		static Vector getTriGravityCenter(const Vector& p0, const Vector& p1, const Vector& p2)
		{
			return Vector((p0.x + p1.x + p2.x) / 3, (p0.y + p1.y + p2.y) / 3);
		}

		//////////////////////////////////////////////////////////////////////////

		static void mergeTwoChains(const ChainShape& src0, const ChainShape& src1,
			std::vector<Vector>& dst);

		static void mergeMultiChains(const std::vector<ChainShape*>& src, std::vector<Vector>& dst);

		static void removeDuplicatePoints(const std::vector<Vector>& src, std::vector<Vector>& dst);

// 		enum TriangulateType
// 		{
// 			e_Constrained,
// 			e_Conforming,
// 			e_ConstrainedConformingAngle,
// 			e_ConstrainedConformingArea,
// 			e_ConstrainedConformingCount
// 		};
// 		static void triangulatePolygon(const std::vector<Vector>& polygon, std::vector<Vector>& result, 
// 			TriangulateType type = e_Constrained);
// 		static void triangulatePolygon(const std::vector<Vector>& polygon, const std::vector<Vector>& lines,
// 			std::vector<Vector>& result, TriangulateType type = e_Constrained);
// 		static void triangulatePolygon(const std::vector<Vector>& polygon, const std::vector<Vector>& lines,
// 			const std::vector<std::vector<Vector> >& loops, std::vector<Vector>& result, TriangulateType type = e_Constrained);
// 
// 		static void triangulateStrips(const std::vector<Vector>& triangulates, 
// 			std::vector<std::vector<Vector> >& strips);

		// 1 2
		// 0 3
		static void computeQuadNodes(const Vector& center, float angle, 
			float xScale, float yScale, float width, float height, Vector quad[4]);

	private:
		// for mergeMultiChains
		static void findNearestPair(const std::vector<ChainShape*>& chains, int& index0, int& index1);
		static float getDistanceOfChains(const ChainShape& chain0, const ChainShape& chain1);

	}; // Math
}

