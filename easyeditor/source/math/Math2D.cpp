#include "Math2D.h"

#include <assert.h>

namespace ee
{

bool Math2D::IsAcuteAngle(const sm::vec2& a, const sm::vec2& center, const sm::vec2& b)
{
	const float squareLenAC = GetDistanceSquare(a, center),
		squareLenBC = GetDistanceSquare(b, center),
		squareLenAB = GetDistanceSquare(a, b);
	return squareLenAC + squareLenBC - squareLenAB > 0;
}

bool Math2D::IsSegmentIntersectRect(float lx1, float ly1, float lx2, float ly2,
									float ax1, float ay1, float ax2, float ay2)
{
	unsigned char typeS, typeE;
	typeS = typeE = 0;
	if ( lx1 < ax1 )      // left:  1000
		typeS |= 0x8; 
	else if ( lx1 > ax2 ) // right: 0100
		typeS |= 0x4; 
	if ( ly1 < ay1 )      // down:  0001
		typeS |= 0x1; 
	else if ( ly1 > ay2 ) // up:    0010
		typeS |= 0x2; 

	if ( lx2 < ax1 )      // left:  1000
		typeE |= 0x8; 
	else if ( lx2 > ax2 ) // right: 0100
		typeE |= 0x4; 
	if ( ly2 < ay1 )      // down:  0001
		typeE |= 0x1; 
	else if ( ly2 > ay2 ) // up:    0010
		typeE |= 0x2; 

	unsigned char comp;
	comp = typeS & typeE;
	if ( comp != 0 )		// must be outside, so must intersect
		return false;
	comp = typeS | typeE;
	if ( comp == 0 )		// must be inside, so must not intersect
		return true;

	// test each edge
	if ( comp & 0x8 )		// 1000, left edge
	{
		float cross_y;
		cross_y = FindYOnSeg( lx1, ly1, lx2, ly2, ax1 );
		if ( cross_y >= ay1 && cross_y <= ay2 )
			return true;
	}
	if ( comp & 0x4 )		// 0100, right edge
	{
		float cross_y;
		cross_y = FindYOnSeg( lx1, ly1, lx2, ly2, ax2 );
		if ( cross_y >= ay1 && cross_y <= ay2 )
			return true;
	}
	if ( comp & 0x1 )		// 0001, down edge
	{
		float cross_x;
		cross_x = FindXOnSeg( lx1, ly1, lx2, ly2, ay1 );
		if ( cross_x >= ax1 && cross_x <= ax2 )
			return true;
	}
	if ( comp & 0x2 )		// 0010, up edge
	{
		float cross_x;
		cross_x = FindXOnSeg( lx1, ly1, lx2, ly2, ay2 );
		if ( cross_x >= ax1 && cross_x <= ax2 )
			return true;
	}

	return false;
}

bool Math2D::IsPolygonIntersectPolygon(const std::vector<sm::vec2>& poly0, const std::vector<sm::vec2>& poly1)
{
	if (poly0.size() < 3 || poly1.size() < 3) {
		return false;
	}

	int sz0 = poly0.size(),
		sz1 = poly1.size();

	int step0 = IsPolygonColckwise(poly0) ? 1 : -1,
		step1 = IsPolygonColckwise(poly1) ? 1 : -1;
	int idx0 = 0;
	for (int i = 0; i < sz0; ++i) {
		int idx1 = 0;
		const sm::vec2& start0 = poly0[idx0];
		int next_idx0 = GetNextIdxInRing(sz0, idx0, step0);
		const sm::vec2& end0 = poly0[next_idx0];
		for (int i = 0; i < sz1; ++i) {
			const sm::vec2& start1 = poly1[idx1];
			int next_idx1 = GetNextIdxInRing(sz1, idx1, step1);
			const sm::vec2& end1 = poly1[next_idx1];

			sm::vec2 cross;
			if (GetTwoSegmentCross(start0, end0, start1, end1, &cross)) {
				// test if cross is end
				bool is_cross0 = IsTwoPointsSame(cross, end0),
					is_cross1 = IsTwoPointsSame(cross, end1);
				if (is_cross0 && is_cross1) {
					const sm::vec2& end_next0 = poly0[GetNextIdxInRing(sz0, next_idx0, step0)];
					const sm::vec2& end_next1 = poly1[GetNextIdxInRing(sz1, next_idx1, step1)];
					float angle0 = GetAngle(end0, end_next0, start0);
					if (angle0 > GetAngle(end0, end_next0, start1) ||
						angle0 > GetAngle(end0, end_next0, end_next1)) {
							return true;
					}
					float angle1 = GetAngle(end1, end_next1, start1);
					if (angle1 > GetAngle(end1, end_next1, start0) ||
						angle1 > GetAngle(end1, end_next1, end_next0)) {
							return true;
					}

					//////////////////////////////////////////////////////////////////////////

					// 					sm::vec2 seg00 = start0 - end0;
					// 					seg00.normalize();
					// 					const sm::vec2& end_next0 = poly0[GetNextIdxInRing(sz0, next_idx0, step0)];
					// 					sm::vec2 seg01 = end_next0 - end0;
					// 					seg01.normalize();
					// 
					// 					sm::vec2 seg10 = start1 - end1;
					// 					seg10.normalize();
					// 					const sm::vec2& end_next1 = poly0[GetNextIdxInRing(sz1, next_idx1, step1)];
					// 					sm::vec2 seg11 = end_next1 - end1;
					// 					seg11.normalize();
					// 
					// 					if (IsTwoSegmentIntersect(seg00, seg01, seg10, seg11) &&
					// 						!IsTwoPointsSame(seg00, seg10) && !IsTwoPointsSame(seg00, seg11) && 
					// 						!IsTwoPointsSame(seg01, seg10) && !IsTwoPointsSame(seg01, seg11)) {
					// 							return true;
					// 					}
				} else if (is_cross0) {
					const sm::vec2& end_next0 = poly0[GetNextIdxInRing(sz0, next_idx0, step0)];
					if (IsTurnLeft(end_next0, end0, end1)) { 
						return true; 
					}
				} else if (is_cross1) {
					const sm::vec2& end_next1 = poly0[GetNextIdxInRing(sz1, next_idx1, step1)];
					if (IsTurnLeft(end_next1, end1, end0)) { 
						return true; 
					}
				} else if (!IsTwoPointsSame(cross, start0) 
					&& !IsTwoPointsSame(cross, start1)) {
						return true;
				}
			}
			idx1 = next_idx1;
		}
		idx0 = next_idx0;
	}

	for (int i = 0; i < sz0; ++i) {
		if (IsPointOnPolyline(poly0[i], poly1)) { continue; }
		return IsPointInArea(poly0[i], poly1);
	}
	for (int i = 0; i < sz1; ++i) {
		if (IsPointOnPolyline(poly1[i], poly0)) { continue; }
		return IsPointInArea(poly1[i], poly0);
	}
	return false;
}

bool Math2D::IsPolygonInPolygon(const std::vector<sm::vec2>& in, const std::vector<sm::vec2>& out)
{
	if (in.size() < 3 || out.size() < 3) {
		return false;
	}

	int sz0 = in.size(),
		sz1 = out.size();
	for (int i = 0; i < sz0; ++i) {
		if (IsPointOnPolyline(in[i], out)) { continue; }
		if (!IsPointInArea(in[i], out)) {
			return false;
		} else {
			break;
		}
	}

	int step0 = IsPolygonColckwise(in) ? 1 : -1,
		step1 = IsPolygonColckwise(out) ? 1 : -1;
	int idx0 = 0;
	for (int i = 0; i < sz0; ++i) {
		int idx1 = 0;
		const sm::vec2& start0 = in[idx0];
		int next_idx0 = GetNextIdxInRing(sz0, idx0, step0);
		const sm::vec2& end0 = in[next_idx0];
		for (int i = 0; i < sz1; ++i) {
			const sm::vec2& start1 = out[idx1];
			int next_idx1 = GetNextIdxInRing(sz1, idx1, step1);
			const sm::vec2& end1 = out[next_idx1];

			sm::vec2 cross;
			if (GetTwoSegmentCross(start0, end0, start1, end1, &cross)) {
				// test if cross is end
				bool is_cross0 = IsTwoPointsSame(cross, end0),
					is_cross1 = IsTwoPointsSame(cross, end1);
				if (is_cross0 && is_cross1) {
					const sm::vec2& end_next0 = in[GetNextIdxInRing(sz0, next_idx0, step0)];
					const sm::vec2& end_next1 = out[GetNextIdxInRing(sz1, next_idx1, step1)];
					float angle0 = GetAngle(end0, end_next0, start0);

					float angle_start1 = GetAngle(end0, end_next0, start1),
						angle_end_next1 = GetAngle(end0, end_next0, end_next1);
					if (angle0 > angle_start1 && angle_start1 != 0 ||
						angle0 > angle_end_next1 && angle_end_next1 != 0) {
						return false;
					}
				} else if (is_cross0) {
					const sm::vec2& end_next0 = in[GetNextIdxInRing(sz0, next_idx0, step0)];
					if (IsTurnLeft(end_next0, end0, end1)) { 
						return false; 
					}
				} else if (is_cross1) {
					const sm::vec2& end_next1 = in[GetNextIdxInRing(sz1, next_idx1, step1)];
					if (IsTurnLeft(end_next1, end1, end0)) { 
						return false; 
					}
				} else if (!IsTwoPointsSame(cross, start0) 
					&& !IsTwoPointsSame(cross, start1)) {
					return false;
				}
			}
			idx1 = next_idx1;
		}
		idx0 = next_idx0;
	}

	return true;	
}

bool Math2D::IsPolygonIntersectRect(const std::vector<sm::vec2>& poly, const sm::rect& rect)
{
	if (poly.size() < 3) {
		return false;
	}

	if (IsPointInArea(rect.Center(), poly) || IsPointInRect(poly[0], rect)) {
		return true;
	}

 	std::vector<sm::vec2> poly2;
 	poly2.push_back(sm::vec2(rect.xmin, rect.ymin));
 	poly2.push_back(sm::vec2(rect.xmax, rect.ymin));
 	poly2.push_back(sm::vec2(rect.xmax, rect.ymax));
 	poly2.push_back(sm::vec2(rect.xmin, rect.ymax));
 
 	return IsPolygonIntersectPolygon(poly, poly2);
}

bool Math2D::IsSegmentIntersectPolyline(const sm::vec2& s, const sm::vec2& e, const std::vector<sm::vec2>& poly)
{
	if (poly.size() < 2) {
		return false;
	} else if (poly.size() < 3) {
		sm::vec2 cross;
		if (GetTwoSegmentCross(s, e, poly[0], poly[1], &cross)) {
			if (!IsTwoPointsSame(s, cross) && !IsTwoPointsSame(e, cross) &&
				!IsTwoPointsSame(poly[0], cross) && !IsTwoPointsSame(poly[1], cross)) {
				return true;
			}
		}
		return false;
	}

	int sz = poly.size();
	for (int i = 0; i < sz; ++i) 
	{
		const sm::vec2& start = poly[i];
		int end_idx = GetNextIdxInRing(sz, i, 1);
		const sm::vec2& end = poly[end_idx];
		sm::vec2 cross; 
		if (GetTwoSegmentCross(s, e, start, end, &cross)) {
			if (IsTwoPointsSame(s, cross) || IsTwoPointsSame(e, cross)) {
				continue;
			}

			if (IsTwoPointsSame(start, cross)) {
				const sm::vec2& start_prev = poly[GetNextIdxInRing(sz, i, -1)];
				float angle = GetAngle(start, end, start_prev);
				if (angle > GetAngle(start, end, e) ||
					angle > GetAngle(start, end, s)) {
					return true;
				}
			} else if (IsTwoPointsSame(end, cross)) {
				const sm::vec2& end_next = poly[GetNextIdxInRing(sz, end_idx, 1)];
				float angle = GetAngle(end, end_next, start);
				if (angle > GetAngle(end, end_next, e) ||
					angle > GetAngle(end, end_next, s)) {
					return true;
				}
				
			} else {
				return true;
			}
		}
	}

	return false;
}

bool Math2D::IsPolylineIntersectRect(const std::vector<sm::vec2>& polyline, bool isLoop, const sm::rect& aabb)
{
	if (polyline.size() < 2) return false;

	for (size_t i = 0, n = polyline.size() - 1; i < n; ++i)
	{
		if (IsSegmentIntersectRect(polyline[i], polyline[i+1], aabb))
			return true;
	}

	if (isLoop && IsSegmentIntersectRect(polyline[polyline.size() - 1], polyline[0], aabb))
		return true;

	return false;
}

bool Math2D::isPolylineIntersectPolylinI(const std::vector<sm::vec2>& poly0, const std::vector<sm::vec2>& poly1)
{
	if (poly0.size() < 3 || poly1.size() < 3) {
		return false;
	}

	int sz0 = poly0.size(),
		sz1 = poly1.size();

	int step0 = IsPolygonColckwise(poly0) ? 1 : -1,
		step1 = IsPolygonColckwise(poly1) ? 1 : -1;
	int idx0 = 0;
	for (int i = 0; i < sz0; ++i) {
		int idx1 = 0;
		const sm::vec2& start0 = poly0[idx0];
		int next_idx0 = GetNextIdxInRing(sz0, idx0, step0);
		const sm::vec2& end0 = poly0[next_idx0];
		for (int i = 0; i < sz1; ++i) {
			const sm::vec2& start1 = poly1[idx1];
			int next_idx1 = GetNextIdxInRing(sz1, idx1, step1);
			const sm::vec2& end1 = poly1[next_idx1];

			sm::vec2 cross;
			if (GetTwoSegmentCross(start0, end0, start1, end1, &cross)) {
				// test if cross is end
				bool is_cross0 = IsTwoPointsSame(cross, end0),
					is_cross1 = IsTwoPointsSame(cross, end1);
				if (is_cross0 && is_cross1) {
					const sm::vec2& end_next0 = poly0[GetNextIdxInRing(sz0, next_idx0, step0)];
					const sm::vec2& end_next1 = poly1[GetNextIdxInRing(sz1, next_idx1, step1)];
					float angle0 = GetAngle(end0, end_next0, start0);
					if (angle0 > GetAngle(end0, end_next0, start1) ||
						angle0 > GetAngle(end0, end_next0, end_next1)) {
							return true;
					}
					float angle1 = GetAngle(end1, end_next1, start1);
					if (angle1 > GetAngle(end1, end_next1, start0) ||
						angle1 > GetAngle(end1, end_next1, end_next0)) {
							return true;
					}

					//////////////////////////////////////////////////////////////////////////

					// 					sm::vec2 seg00 = start0 - end0;
					// 					seg00.normalize();
					// 					const sm::vec2& end_next0 = poly0[GetNextIdxInRing(sz0, next_idx0, step0)];
					// 					sm::vec2 seg01 = end_next0 - end0;
					// 					seg01.normalize();
					// 
					// 					sm::vec2 seg10 = start1 - end1;
					// 					seg10.normalize();
					// 					const sm::vec2& end_next1 = poly0[GetNextIdxInRing(sz1, next_idx1, step1)];
					// 					sm::vec2 seg11 = end_next1 - end1;
					// 					seg11.normalize();
					// 
					// 					if (IsTwoSegmentIntersect(seg00, seg01, seg10, seg11) &&
					// 						!IsTwoPointsSame(seg00, seg10) && !IsTwoPointsSame(seg00, seg11) && 
					// 						!IsTwoPointsSame(seg01, seg10) && !IsTwoPointsSame(seg01, seg11)) {
					// 							return true;
					// 					}
				} else if (is_cross0) {
					const sm::vec2& end_next0 = poly0[GetNextIdxInRing(sz0, next_idx0, step0)];
					if (IsTurnLeft(end_next0, end0, end1)) { 
						return true; 
					}
				} else if (is_cross1) {
					const sm::vec2& end_next1 = poly0[GetNextIdxInRing(sz1, next_idx1, step1)];
					if (IsTurnLeft(end_next1, end1, end0)) { 
						return true; 
					}
				} else if (!IsTwoPointsSame(cross, start0) 
					&& !IsTwoPointsSame(cross, start1)) {
						return true;
				}
			}
			idx1 = next_idx1;
		}
		idx0 = next_idx0;
	}

	return false;	
}

bool Math2D::IsCircleIntersectRect(const sm::vec2& center, float radius, const sm::rect& aabb)
{
	if (IsPointInRect(center, aabb))
		return true;

	std::vector<sm::vec2> bounding;
	bounding.push_back(sm::vec2(aabb.xmin, aabb.ymin));
	bounding.push_back(sm::vec2(aabb.xmax, aabb.ymin));
	bounding.push_back(sm::vec2(aabb.xmax, aabb.ymax));
	bounding.push_back(sm::vec2(aabb.xmin, aabb.ymax));
	bounding.push_back(sm::vec2(aabb.xmin, aabb.ymin));

	size_t index;
	return GetDisPointToPolyline(center, bounding, &index) < radius;
}

bool Math2D::IsPointInTriangle(const sm::vec2& p, const sm::vec2& a, const sm::vec2& b, const sm::vec2& c)
{
	const bool test0 = IsPointAtSegmentLeft(p, a, b),
		test1 = IsPointAtSegmentLeft(p, b, c),
		test2 = IsPointAtSegmentLeft(p, c, a);
	return test0 == test1 && test1 == test2;
}

bool Math2D::IsPointInArea(const sm::vec2& pos, const std::vector<sm::vec2>& area)
{
	bool oddNodes = false;
	for (size_t i = 0, n = area.size(), j = n - 1; i < n; ++i)
	{
		if ((area[i].y < pos.y && area[j].y >= pos.y ||
			area[j].y < pos.y && area[i].y >= pos.y) &&
			(area[i].x <= pos.x || area[j].x <= pos.x))
		{
			oddNodes ^= (area[i].x + (pos.y - area[i].y) / (area[j].y - area[i].y) * (area[j].x - area[i].x) < pos.x);
		}
		j = i;
	}
	return oddNodes;
}

bool Math2D::IsPointOnPolyline(const sm::vec2& pos, const std::vector<sm::vec2>& poly)
{
	sm::rect r(pos, SM_RAD_TO_DEG, SM_RAD_TO_DEG);
	return IsPolylineIntersectRect(poly, true, r);
}

bool Math2D::IsPointInConvexHull(const sm::vec2& pos, const std::vector<sm::vec2>& hull)
{
	if (hull.size() < 3) return false;

	size_t count = 0;
	for (size_t i = 0, n = hull.size(); i < n; ++i)
	{
		const sm::vec2 s = hull[i], 
			e = i == hull.size() - 1 ? hull[0] : hull[i + 1];
		if (IsPointAtSegmentLeft(pos, s, e)) ++count;
	}
	return count == hull.size() || count == 0;
}

bool Math2D::IsPointInCircle(const sm::vec2& pos, const sm::vec2& center, float radius)
{
	return GetDistance(pos, center) < radius;
}

bool Math2D::IsTwoLineParallel(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1)
{
	float denominatorX = (e1.y - s1.y) * (e0.x - s0.x) - (e0.y - s0.y) * (e1.x - s1.x),
		  denominatorY = (e1.x - s1.x) * (e0.y - s0.y) - (e0.x - s0.x) * (e1.y - s1.y);
	return fabs(denominatorX) < FLT_EPSILON || fabs(denominatorY) < FLT_EPSILON;
}

bool Math2D::IsTwoSegmentIntersect(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1)
{
	return IsPointAtSegmentLeft(s0, s1, e1) != IsPointAtSegmentLeft(e0, s1, e1)
		&& IsPointAtSegmentLeft(s1, s0, e0) != IsPointAtSegmentLeft(e1, s0, e0);
}

int Math2D::CheckPosInTriangle(const sm::vec2& p, const sm::vec2& t0, const sm::vec2& t1, const sm::vec2& t2)
{
	if (IsTheSamePos(p, t0) || IsTheSamePos(p, t1) || IsTheSamePos(p, t2))
		return 3;
	if (TestPointOnSection(t0, t1, p) == 0)
		return 0;
	if (TestPointOnSection(t1, t2, p) == 0)
		return 1;
	if (TestPointOnSection(t2, t0, p) == 0)
		return 2;

	if (IsPointAtSegmentLeft(p, t0, t1) != IsPointAtSegmentLeft(p, t0, t2)
		&& IsPointAtSegmentLeft(p, t1, t2) != IsPointAtSegmentLeft(p, t1, t0)
		&& IsPointAtSegmentLeft(p, t2, t1) != IsPointAtSegmentLeft(p, t2, t0))
		return 4;
	else 
		return -1;
}

bool Math2D::IsTurnLeft(const sm::vec2& a, const sm::vec2& center, const sm::vec2& b)
{
	return (center.x - a.x) * (b.y - center.y) - (b.x - center.x) * (center.y - a.y) > 0;
}

bool Math2D::IsTurnRight(const sm::vec2& a, const sm::vec2& center, const sm::vec2& b)
{
	return (center.x - a.x) * (b.y - center.y) - (b.x - center.x) * (center.y - a.y) < 0;
}

float Math2D::GetDisPointToStraightLine(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e)
{
	float divider = GetDistance(s, e);
	if (fabs(divider) < FLT_EPSILON) return GetDistance(p, s);
	else return fabs((s.x - p.x) * (e.y - p.y) - (s.y - p.y) * (e.x - p.x)) / divider;
}

float Math2D::GetDisPointToSegment(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e)
{
	if (!IsAcuteAngle(p, s, e))
		return GetDistance(p, s);
	else if (!IsAcuteAngle(p, e, s))
		return GetDistance(p, e);
	else
		return fabs((s.x - p.x) * (e.y - p.y) - (s.y - p.y) * (e.x - p.x)) / GetDistance(s, e);
}

float Math2D::GetDisPointToPolyline(const sm::vec2& p, const std::vector<sm::vec2>& polyline, size_t* iPos/* = NULL*/)
{
	if (polyline.size() < 2) return FLT_MAX;

	float nearest = FLT_MAX;
	size_t iNearest = -1;
	for (size_t i = 0, n = polyline.size() - 1; i < n; ++i)
	{
		const float dis = GetDisPointToSegment(p, polyline[i], polyline[i + 1]);
		if (dis < nearest) 
		{
			nearest = dis;
			iNearest = i;
		}
	}
	if (iPos) *iPos = iNearest;
	return nearest;
}

float Math2D::GetDisPointToMultiLines(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& multiLines, size_t* iLine/* = NULL*/, size_t* iPos/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest0 = -1, iNearest1 = -1;
	for (size_t i = 0, n = multiLines.size(); i < n; ++i)
	{
		size_t index;
		const float dis = GetDisPointToPolyline(p, multiLines[i], &index);
		if (dis < nearest)
		{
			nearest = dis;
			iNearest0 = i;
			iNearest1 = index;
		}
	}
	if (iLine && iPos)
	{
		*iLine = iNearest0;
		*iPos = iNearest1;
	}
	return nearest;
}

float Math2D::GetDisPointToMultiPos(const sm::vec2& p, const std::vector<sm::vec2>& pos, size_t* index/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest = -1;
	for (size_t i = 0, n = pos.size(); i < n; ++i)
	{
		const float dis = Math2D::GetDistance(p, pos[i]);
		if (dis < nearest)
		{
			nearest = dis;
			iNearest = i;
		}
	}
	if (index) *index = iNearest;
	return nearest;
}

float Math2D::GetDisPointToMultiPos(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& pos, size_t* index0/* = NULL*/, size_t* index1/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest0 = -1, iNearest1 = -1;
	for (size_t i = 0, n = pos.size(); i < n; ++i)
	{
		size_t index;
		const float dis = GetDisPointToMultiPos(p, pos[i], &index);
		if (dis < nearest)
		{
			nearest = dis;
			iNearest0 = i;
			iNearest1 = index;
		}
	}
	if (index0 && index1)
	{
		*index0 = iNearest0;
		*index1 = iNearest1;
	}
	return nearest;
}

float Math2D::GetAngle(const sm::vec2& center, const sm::vec2& pa, const sm::vec2& pb)
{
	const float a = GetDistance(center, pa),
		b = GetDistance(center, pb),
		c = GetDistance(pa, pb);

	float cosVal = (a * a + b * b - c * c) / (2 * a * b);
	cosVal = std::max(std::min(cosVal, 1.0f), -1.0f);

	return acos(cosVal);

// 	float angle = acos(cosVal);
// 	return IsTurnRight(pa, center, pb) ? angle : -angle;
}

float Math2D::GetAngleInDirection(const sm::vec2& center, const sm::vec2& start, const sm::vec2& end)
{
	float angle = Math2D::GetAngle(center, start, end);
	const float cross = (start - center).Cross(end - start);
	if (cross < 0) angle = -angle;
	return angle;
}

float Math2D::GetLineAngle(const sm::vec2& s, const sm::vec2& e)
{
	const sm::vec2 other(s.x + 1.0f, s.y);
	const float ang = GetAngle(s, e, other);
	if ((other - s).Cross(e - s) > 0) return ang;
	else return -ang;
}

float Math2D::GetPolygonArea(const std::vector<sm::vec2>& polygon)
{
	if(polygon.size() < 3)
		return 0;

	float s = 0;
	for(int i = 0, n = polygon.size(); i < n; i++) {
		int next = (i+1)%n;
		s += (polygon[i].y + polygon[next].y) * (polygon[i].x - polygon[next].x);		
	}

	return fabs(s/2.0f);
}

float Math2D::GetPolygonPerimeter(const std::vector<sm::vec2>& poly)
{
	if (poly.size() < 2) {
		return 0;
	}

	float perimeter = 0.0f;
	for (int i = 0, n = poly.size()-1; i < n; ++i) {
		perimeter += GetDistance(poly[i], poly[i+1]);
	}
	perimeter += GetDistance(poly[0], poly[poly.size()-1]);

	return perimeter;
}

float Math2D::GetTriangleArea(const sm::vec2& p0, const sm::vec2& p1, const sm::vec2& p2)
{
	float s = 0;
	s += (p1.y + p0.y) * (p1.x - p0.x);
	s += (p2.y + p1.y) * (p2.x - p1.x);
	s += (p0.y + p2.y) * (p0.x - p2.x);
	return fabs(s/2.0f);
}

bool Math2D::GetTwoLineCross(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1, sm::vec2* cross)
{
	// If they are parallel ?
	float denominatorX = (e1.y - s1.y) * (e0.x - s0.x) - (e0.y - s0.y) * (e1.x - s1.x),
		  denominatorY = (e1.x - s1.x) * (e0.y - s0.y) - (e0.x - s0.x) * (e1.y - s1.y);
	if (fabs(denominatorX) < FLT_EPSILON || fabs(denominatorY) < FLT_EPSILON)
	{
		cross->x = cross->y = FLT_MAX;
		return false;
	} else {
		cross->x = ( (e0.x * s0.y - s0.x * e0.y) * (e1.x - s1.x) - (e1.x * s1.y - s1.x * e1.y) * (e0.x - s0.x) ) / denominatorX;
		cross->y = ( (e0.y * s0.x - s0.y * e0.x) * (e1.y - s1.y) - (e1.y * s1.x - s1.y * e1.x) * (e0.y - s0.y) ) / denominatorY;
		return true;
	}
}

bool Math2D::GetTwoSegmentCross(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1, sm::vec2* cross)
{
	bool line_intersect = GetTwoLineCross(s0, e0, s1, e1, cross);
	if (line_intersect) {
		return IsTwoSegmentIntersect(s0, e0, s1, e1);
	} else {
		return false;
	}
}

int Math2D::TestPointOnSection(const sm::vec2& startPos, const sm::vec2& endPos, const sm::vec2& thdPos, float tolerance/* = FLT_EPSILON*/)
{
	assert(tolerance > 0.0);
	sm::rect aabb;
	aabb.xmin = thdPos.x - tolerance / 2.0f;
	aabb.xmax = thdPos.x + tolerance / 2.0f;
	aabb.ymin = thdPos.y - tolerance / 2.0f;
	aabb.ymax = thdPos.y + tolerance / 2.0f;

	if (!IsSegmentIntersectRect(startPos, endPos, aabb)) return -1;

	int resultA = 0;
	int resultB = 0;
	if (IsPointInRect(startPos, aabb)) resultA = 1;
	if (IsPointInRect(endPos, aabb)) resultB = 2;

	return resultA + resultB;
}

int Math2D::GetCircumcenter(const sm::vec2& pos1, const sm::vec2& pos2, const sm::vec2& pos3, sm::vec2* center, float tolerance/* = FLT_EPSILON*/)
{
	if (TestPointOnSection(pos1, pos2, pos3, tolerance) != -1)
		return -1;

	float x1, x2, x3, y1, y2, y3;
	float a, b, c, d, e, f;
	x1 = pos1.x; y1 = pos1.y;
	x2 = pos2.x; y2 = pos2.y;
	x3 = pos3.x; y3 = pos3.y;

	a = 2 * (x2 - x1);
	b = 2 * (y2 - y1);
	c = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1;
	d = 2 * (x3 - x2);
	e = 2 * (y3 - y2);
	f = x3 * x3 + y3 * y3 - x2 * x2 - y2 * y2;

	center->x = (b * f - e * c) / (b * d - e * a);
	center->y = (d * c - a * f) / (b * d - e * a);

	return 0;
}

int Math2D::GetFootOfPerpendicular(const sm::vec2& s, const sm::vec2& e, const sm::vec2& out, sm::vec2* foot)
{
	const float dx = e.x - s.x, dy = e.y - s.y;
	const float dxSquare = dx * dx, dySquare = dy * dy;

	if (dxSquare + dySquare < FLT_EPSILON)
	{
		*foot = s;
		return -1;
	}

	if (fabs(s.x - e.x) > fabs(s.y - e.y))
	{
		foot->x = (dxSquare * out.x + dySquare * s.x + dx * dy * (out.y - s.y)) / (dxSquare + dySquare);
		if (s.x == e.x)
		{
			foot->y = out.y;
		}
		else
		{
			foot->y = FindYOnSeg(s, e, foot->x);
		}
	}
	else
	{
		foot->y = (dySquare * out.y + dxSquare * s.y + dx * dy * (out.x - s.x)) / (dxSquare + dySquare);
		if (s.y == e.y)
		{
			foot->x = out.x;
		}
		else
		{
			foot->x = FindXOnSeg(s, e, foot->y);
		}
	}

	if (IsBetween(s.x, e.x, foot->x) && IsBetween(s.y, e.y, foot->y))
		return 0;
	else
		return -1;
}

void Math2D::GetNearestPosOnLineToPoint(const sm::vec2& p, const std::vector<sm::vec2>& l, sm::vec2* nearest, size_t* index)
{
	size_t iNearestPos = 0;
	float nearestDis = GetDistance(p, l.at(iNearestPos));
	for (size_t i = 1; i < l.size(); ++i)
	{
		float dis = GetDistance(p, l.at(i));
		if (dis < nearestDis)
		{
			nearestDis = dis;
			iNearestPos = i;
		}
	}
	*nearest = l.at(iNearestPos);
	*index = iNearestPos;

	if (iNearestPos != l.size() - 1)
	{
		sm::vec2 foot;
		if (GetFootOfPerpendicular(l.at(iNearestPos), l.at(iNearestPos + 1), p, &foot) == 0)
		{
			float dis = GetDistance(foot, p);
			if (dis < nearestDis)
			{
				nearestDis = dis;
				*nearest = foot;
				*index = iNearestPos;
			}
		}
	}

	if (iNearestPos != 0)
	{
		sm::vec2 foot;
		if (GetFootOfPerpendicular(l.at(iNearestPos), l.at(iNearestPos - 1), p, &foot) == 0)
		{
			float dis = GetDistance(foot, p);
			if (dis < nearestDis)
			{
				*nearest = foot;
				*index = iNearestPos - 1;
			}
		}
	}
}

sm::vec2 Math2D::RotateVector(const sm::vec2& v, float rad)
{
	sm::vec2 ret;
	ret.x = v.x * cos(rad) - v.y * sin(rad);
	ret.y = v.x * sin(rad) + v.y * cos(rad);
	return ret;
}

sm::vec2 Math2D::RotateVectorRightAngle(const sm::vec2& v, bool isTurnLeft)
{
	sm::vec2 ret = v;
	if (isTurnLeft)
	{
		ret.x = -v.y;
		ret.y = v.x;
	}
	else
	{
		ret.x = v.y;
		ret.y = -v.x;
	}
	return ret;
}

float Math2D::TransLen(float len, const sm::mat4& m)
{
	sm::vec2 pos0(0, 0);
	pos0 = TransVector(pos0, m);

	sm::vec2 pos1(len, 0.0f);
	pos1 = TransVector(pos1, m);

	return GetDistance(pos0, pos1);
}

sm::vec2 Math2D::TransVector(const sm::vec2& v, const sm::mat4& m)
{
	sm::vec2 ret;
	ret.x = m.x[0] * v.x + m.x[4] * v.y + m.x[12];
	ret.y = m.x[1] * v.x + m.x[5] * v.y + m.x[13];
	return ret;
}

void Math2D::TransVertices(const sm::mat4& mt, const std::vector<sm::vec2>& src, std::vector<sm::vec2>& dst)
{
	dst.clear();
	dst.reserve(src.size());
	for (int i = 0, n = src.size(); i < n; ++i) {
		dst.push_back(TransVector(src[i], mt));
	}
}

sm::vec2 Math2D::TransCoordsLocalToWorld(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& local)
{
	sm::vec2 nx(xDir);
	nx.Normalize();
	sm::vec2 ny = RotateVectorRightAngle(nx, true);
	return origin + (nx * local.x + ny * local.y);
}

sm::vec2 Math2D::TransCoordsWorldToLocal(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& world)
{
	sm::vec2 offset = world - origin;
	sm::vec2 local;
	float dis = xDir.Length();
	local.x = offset.Dot(xDir) / dis;
	sm::vec2 perp = offset - xDir * (local.x / dis);
	if (xDir.Cross(perp) > 0)
		local.y = perp.Length();
	else
		local.y = -perp.Length();
	return local;
}

void Math2D::GetMBR(const std::vector<sm::vec2>& pos, sm::rect* mbr)
{
	if (pos.empty()) return;

	mbr->xmin = mbr->xmax = pos[0].x;
	mbr->ymin = mbr->ymax = pos[0].y;
	for (size_t i = 1, n = pos.size(); i < n; ++i)
	{
		if (pos[i].x < mbr->xmin) mbr->xmin = pos[i].x;
		if (pos[i].x > mbr->xmax) mbr->xmax = pos[i].x;
		if (pos[i].y < mbr->ymin) mbr->ymin = pos[i].y;
		if (pos[i].y > mbr->ymax) mbr->ymax = pos[i].y;
	}
}

void Math2D::RemoveDuplicatePoints(const std::vector<sm::vec2>& src, std::vector<sm::vec2>& dst)
{
	if (src.size() > 1)
	{
		dst.reserve(src.size());

		sm::vec2 last = src[0];
		dst.push_back(last);
		for (size_t i = 1; i < src.size(); ++i)
		{
			if (Math2D::GetDistance(src[i], last) > 1.0f)
			{
 				last = src[i];
 				dst.push_back(last);			
			}
		}
	}
	else
	{
		dst = src;
	}
}

void Math2D::ComputeQuadNodes(const sm::vec2& center, float angle, float xScale, float yScale, 
							float width, float height, sm::vec2 quad[4])
{
	const float hWidth = width * 0.5f * xScale,
		hHeight = height * 0.5f * yScale;

	float x1 = - hWidth;
	float y1 = - hHeight;
	float x2 = - hWidth;
	float y2 =   hHeight;
	float x3 =   hWidth;
	float y3 =   hHeight;

	const float cosVal = cos(angle),
		sinVal = sin(angle);

	quad[0].x = cosVal * x1 - sinVal * y1 + center.x;
	quad[0].y = sinVal * x1 + cosVal * y1 + center.y;

	quad[1].x = cosVal * x2 - sinVal * y2 + center.x;
	quad[1].y = sinVal * x2 + cosVal * y2 + center.y;

	quad[2].x = cosVal * x3 - sinVal * y3 + center.x;
	quad[2].y = sinVal * x3 + cosVal * y3 + center.y;

	quad[3].x = quad[0].x + (quad[2].x - quad[1].x);
	quad[3].y = quad[2].y - (quad[1].y - quad[0].y);
}

void Math2D::SideOffsetSegment(const sm::vec2& s, const sm::vec2& e, bool toleft, float dis,
							 sm::vec2& ds, sm::vec2& de)
{
	if (s == e) {
		return;
	}

	float len = GetDistance(s, e);
	float t = dis / len;
	
	if (toleft) {
		ds = RotateVectorRightAngle(e - s, true) * t + s;
		de = RotateVectorRightAngle(s - e, false) * t + e;
	} else {
		ds = RotateVectorRightAngle(e - s, false) * t + s;
		de = RotateVectorRightAngle(s - e, true) * t + e;
	}
}

bool Math2D::IsPolygonColckwise(const std::vector<sm::vec2>& poly)
{
	if (poly.size() < 3) {
		return false;
	}

	float left = FLT_MAX;
	int left_idx;
	int sz = poly.size();
	for (int i = 0; i < sz; ++i) {
		if (poly[i].x < left) {
			left = poly[i].x;
			left_idx = i;
		}
	}

	const sm::vec2& curr = poly[left_idx];
	const sm::vec2& next = poly[(left_idx+1)%sz];
	const sm::vec2& prev = poly[(left_idx+sz-1)%sz];
	sm::vec2 up(curr.x, curr.y + 1);
	return GetAngle(curr, up, next) < GetAngle(curr, up, prev);
}

}