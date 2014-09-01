#include "Math.h"

#include <assert.h>
//#include <Triangle/triangle.cpp>

#include "algorithms/SGI.h"

#include "Matrix.h"

namespace d2d
{

bool Math::isAcuteAngle(const Vector& a, const Vector& center, const Vector& b)
{
	const float squareLenAC = getDistanceSquare(a, center),
		squareLenBC = getDistanceSquare(b, center),
		squareLenAB = getDistanceSquare(a, b);
	return squareLenAC + squareLenBC - squareLenAB > 0;
}

bool Math::isSegmentIntersectRect(float lx1, float ly1, float lx2, float ly2,
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
		cross_y = findYOnSeg( lx1, ly1, lx2, ly2, ax1 );
		if ( cross_y >= ay1 && cross_y <= ay2 )
			return true;
	}
	if ( comp & 0x4 )		// 0100, right edge
	{
		float cross_y;
		cross_y = findYOnSeg( lx1, ly1, lx2, ly2, ax2 );
		if ( cross_y >= ay1 && cross_y <= ay2 )
			return true;
	}
	if ( comp & 0x1 )		// 0001, down edge
	{
		float cross_x;
		cross_x = findXOnSeg( lx1, ly1, lx2, ly2, ay1 );
		if ( cross_x >= ax1 && cross_x <= ax2 )
			return true;
	}
	if ( comp & 0x2 )		// 0010, up edge
	{
		float cross_x;
		cross_x = findXOnSeg( lx1, ly1, lx2, ly2, ay2 );
		if ( cross_x >= ax1 && cross_x <= ax2 )
			return true;
	}

	return false;
}

bool Math::isPolylineIntersectRect(const std::vector<Vector>& polyline, bool isLoop, const Rect& aabb)
{
	if (polyline.size() < 2) return false;

	for (size_t i = 0, n = polyline.size() - 1; i < n; ++i)
	{
		if (isSegmentIntersectRect(polyline[i], polyline[i+1], aabb))
			return true;
	}

	if (isLoop && isSegmentIntersectRect(polyline[polyline.size() - 1], polyline[0], aabb))
		return true;

	return false;
}

bool Math::isCircleIntersectRect(const Vector& center, float radius, const Rect& aabb)
{
	if (isPointInRect(center, aabb))
		return true;

	std::vector<Vector> bounding;
	bounding.push_back(Vector(aabb.xMin, aabb.yMin));
	bounding.push_back(Vector(aabb.xMax, aabb.yMin));
	bounding.push_back(Vector(aabb.xMax, aabb.yMax));
	bounding.push_back(Vector(aabb.xMin, aabb.yMax));
	bounding.push_back(Vector(aabb.xMin, aabb.yMin));

	size_t index;
	return getDisPointToPolyline(center, bounding, &index) < radius;
}

bool Math::isPointInTriangle(const Vector& p, const Vector& a, const Vector& b, const Vector& c)
{
	const bool test0 = isPointAtSegmentLeft(p, a, b),
		test1 = isPointAtSegmentLeft(p, b, c),
		test2 = isPointAtSegmentLeft(p, c, a);
	return test0 == test1 && test1 == test2;
}

bool Math::isPointInArea(const Vector& pos, const std::vector<Vector>& area)
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

bool Math::isPointInConvexHull(const Vector& pos, const std::vector<Vector>& hull)
{
	if (hull.size() < 3) return false;

	size_t count = 0;
	for (size_t i = 0, n = hull.size(); i < n; ++i)
	{
		const Vector s = hull[i], 
			e = i == hull.size() - 1 ? hull[0] : hull[i + 1];
		if (isPointAtSegmentLeft(pos, s, e)) ++count;
	}
	return count == hull.size() || count == 0;
}

bool Math::isPointInCircle(const Vector& pos, const Vector& center, float radius)
{
	return getDistance(pos, center) < radius;
}

int Math::checkPosInTriangle(const Vector& p, const Vector& t0, const Vector& t1, const Vector& t2)
{
	if (isTheSamePos(p, t0) || isTheSamePos(p, t1) || isTheSamePos(p, t2))
		return 3;
	if (testPointOnSection(t0, t1, p) == 0)
		return 0;
	if (testPointOnSection(t1, t2, p) == 0)
		return 1;
	if (testPointOnSection(t2, t0, p) == 0)
		return 2;

	if (isPointAtSegmentLeft(p, t0, t1) != isPointAtSegmentLeft(p, t0, t2)
		&& isPointAtSegmentLeft(p, t1, t2) != isPointAtSegmentLeft(p, t1, t0)
		&& isPointAtSegmentLeft(p, t2, t1) != isPointAtSegmentLeft(p, t2, t0))
		return 4;
	else 
		return -1;
}

float Math::getDisPointToStraightLine(const Vector& p, const Vector& s, const Vector& e)
{
	float divider = getDistance(s, e);
	if (fabs(divider) < FLT_EPSILON) return getDistance(p, s);
	else return fabs((s.x - p.x) * (e.y - p.y) - (s.y - p.y) * (e.x - p.x)) / divider;
}

float Math::getDisPointToSegment(const Vector& p, const Vector& s, const Vector& e)
{
	if (!isAcuteAngle(p, s, e))
		return getDistance(p, s);
	else if (!isAcuteAngle(p, e, s))
		return getDistance(p, e);
	else
		return fabs((s.x - p.x) * (e.y - p.y) - (s.y - p.y) * (e.x - p.x)) / getDistance(s, e);
}

float Math::getDisPointToPolyline(const Vector& p, const std::vector<Vector>& polyline, size_t* iPos/* = NULL*/)
{
	if (polyline.size() < 2) return FLT_MAX;

	float nearest = FLT_MAX;
	size_t iNearest = -1;
	for (size_t i = 0, n = polyline.size() - 1; i < n; ++i)
	{
		const float dis = getDisPointToSegment(p, polyline[i], polyline[i + 1]);
		if (dis < nearest) 
		{
			nearest = dis;
			iNearest = i;
		}
	}
	if (iPos) *iPos = iNearest;
	return nearest;
}

float Math::getDisPointToMultiLines(const Vector& p, const std::vector<std::vector<Vector> >& multiLines, size_t* iLine/* = NULL*/, size_t* iPos/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest0 = -1, iNearest1 = -1;
	for (size_t i = 0, n = multiLines.size(); i < n; ++i)
	{
		size_t index;
		const float dis = getDisPointToPolyline(p, multiLines[i], &index);
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

float Math::getDisPointToMultiPos(const Vector& p, const std::vector<Vector>& pos, size_t* index/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest = -1;
	for (size_t i = 0, n = pos.size(); i < n; ++i)
	{
		const float dis = Math::getDistance(p, pos[i]);
		if (dis < nearest)
		{
			nearest = dis;
			iNearest = i;
		}
	}
	if (index) *index = iNearest;
	return nearest;
}

float Math::getDisPointToMultiPos(const Vector& p, const std::vector<std::vector<Vector> >& pos, size_t* index0/* = NULL*/, size_t* index1/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest0 = -1, iNearest1 = -1;
	for (size_t i = 0, n = pos.size(); i < n; ++i)
	{
		size_t index;
		const float dis = getDisPointToMultiPos(p, pos[i], &index);
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

float Math::getAngle(const Vector& center, const Vector& pa, const Vector& pb)
{
	const float a = getDistance(center, pa),
		b = getDistance(center, pb),
		c = getDistance(pa, pb);

	float cosVal = (a * a + b * b - c * c) / (2 * a * b);
	cosVal = std::max(std::min(cosVal, 1.0f), -1.0f);

	return acos(cosVal);
}

float Math::getAngleInDirection(const Vector& center, const Vector& start, const Vector& end)
{
	float angle = Math::getAngle(center, start, end);
	const float cross = f2Cross(start - center, end - start);
	if (cross < 0) angle = -angle;
	return angle;
}

float Math::getLineAngle(const Vector& s, const Vector& e)
{
	const Vector other(s.x + 1.0f, s.y);
	const float ang = getAngle(s, e, other);
	if (f2Cross(other - s, e - s) > 0) return ang;
	else return -ang;
}

int Math::testPointOnSection(const Vector& startPos, const Vector& endPos, const Vector& thdPos, float tolerance/* = FLT_EPSILON*/)
{
	assert(tolerance > 0.0);
	Rect aabb;
	aabb.xMin = thdPos.x - tolerance / 2.0f;
	aabb.xMax = thdPos.x + tolerance / 2.0f;
	aabb.yMin = thdPos.y - tolerance / 2.0f;
	aabb.yMax = thdPos.y + tolerance / 2.0f;

	if (!isSegmentIntersectRect(startPos, endPos, aabb)) return -1;

	int resultA = 0;
	int resultB = 0;
	if (isPointInRect(startPos, aabb)) resultA = 1;
	if (isPointInRect(endPos, aabb)) resultB = 2;

	return resultA + resultB;
}

int Math::getCircumcenter(const Vector& pos1, const Vector& pos2, const Vector& pos3, Vector* center, float tolerance/* = FLT_EPSILON*/)
{
	if (testPointOnSection(pos1, pos2, pos3, tolerance) != -1)
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

int Math::getFootOfPerpendicular(const Vector& s, const Vector& e, const Vector& out, Vector* foot)
{
	const float dx = e.x - s.x, dy = e.y - s.y;
	const float dxSquare = dx * dx, dySquare = dy * dy;

	if (dxSquare + dySquare < FLT_EPSILON)
	{
		*foot = s;
		return -1;
	}

	foot->x = (dxSquare * out.x + dySquare * s.x + dx * dy * (out.y - s.y)) / (dxSquare + dySquare);
	if (s.x == e.x)
	{
		foot->y = out.y;
	}
	else
	{
		foot->y = findYOnSeg(s, e, foot->x);
	}

	if (isBetween(s.x, e.x, foot->x) && isBetween(s.y, e.y, foot->y))
		return 0;
	else
		return -1;
}

void Math::getNearestPosOnLineToPoint(const Vector& p, const std::vector<Vector>& l, Vector* nearest, size_t* index)
{
	size_t iNearestPos = 0;
	float nearestDis = getDistance(p, l.at(iNearestPos));
	for (size_t i = 1; i < l.size(); ++i)
	{
		float dis = getDistance(p, l.at(i));
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
		Vector foot;
		if (getFootOfPerpendicular(l.at(iNearestPos), l.at(iNearestPos + 1), p, &foot) == 0)
		{
			float dis = getDistance(foot, p);
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
		Vector foot;
		if (getFootOfPerpendicular(l.at(iNearestPos), l.at(iNearestPos - 1), p, &foot) == 0)
		{
			float dis = getDistance(foot, p);
			if (dis < nearestDis)
			{
				*nearest = foot;
				*index = iNearestPos - 1;
			}
		}
	}
}

Vector Math::rotateVector(const Vector& v, float rad)
{
	Vector ret;
	ret.x = v.x * cos(rad) - v.y * sin(rad);
	ret.y = v.x * sin(rad) + v.y * cos(rad);
	return ret;
}

Vector Math::rotateVectorRightAngle(const Vector& v, bool isTurnLeft)
{
	Vector ret = v;
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

Vector Math::transVector(const Vector& v, const Matrix& m)
{
	Vector ret;
	const float* e = m.getElements();
	ret.x = e[0] * v.x + e[4] * v.y + e[12];
	ret.y = e[1] * v.x + e[5] * v.y + e[13];
	return ret;
}

Vector Math::transCoordsLocalToWorld(const Vector& origin, const Vector& xDir, const Vector& local)
{
	Vector nx(xDir);
	nx.normalize();
	Vector ny = rotateVectorRightAngle(nx, true);
	return origin + (nx * local.x + ny * local.y);
}

Vector Math::transCoordsWorldToLocal(const Vector& origin, const Vector& xDir, const Vector& world)
{
	Vector offset = world - origin;
	Vector local;
	float dis = xDir.length();
	local.x = f2Dot(offset, xDir) / dis;
	Vector perp = offset - xDir * (local.x / dis);
	if (f2Cross(xDir, perp) > 0)
		local.y = perp.length();
	else
		local.y = -perp.length();
	return local;
}

void Math::getMBR(const std::vector<Vector>& pos, Rect* mbr)
{
	if (pos.empty()) return;

	mbr->xMin = mbr->xMax = pos[0].x;
	mbr->yMin = mbr->yMax = pos[0].y;
	for (size_t i = 1, n = pos.size(); i < n; ++i)
	{
		if (pos[i].x < mbr->xMin) mbr->xMin = pos[i].x;
		if (pos[i].x > mbr->xMax) mbr->xMax = pos[i].x;
		if (pos[i].y < mbr->yMin) mbr->yMin = pos[i].y;
		if (pos[i].y > mbr->yMax) mbr->yMax = pos[i].y;
	}
}

void Math::removeDuplicatePoints(const std::vector<Vector>& src, std::vector<Vector>& dst)
{
	if (src.size() > 1)
	{
		dst.reserve(src.size());

		Vector last = src[0];
		dst.push_back(last);
		for (size_t i = 1; i < src.size(); ++i)
		{
			if (Math::getDistance(src[i], last) > 1.0f)
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

//void Math::triangulatePolygon(const std::vector<Vector>& polygon, std::vector<Vector>& result, 
//							  TriangulateType type/* = e_Constrained*/)
//{
//	struct triangulateio in, out;
//
//	in.numberofpoints = polygon.size();
//	in.numberofpointattributes = 0;
//	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
//	in.pointmarkerlist = (int *) NULL;
//	int index = 0;
//	for (size_t i = 0; i < in.numberofpoints; ++i)
//	{
//		in.pointlist[index++] = polygon[i].x;
//		in.pointlist[index++] = polygon[i].y;
//	}
//
//	in.numberofsegments = in.numberofpoints;
//	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
//	index = 0;
//	for (size_t i = 0; i < in.numberofsegments - 1; ++i)
//	{
//		in.segmentlist[index++] = i;
//		in.segmentlist[index++] = i + 1;
//	}
//	in.segmentlist[index++] = in.numberofsegments - 1;
//	in.segmentlist[index++] = 0;
//
//	in.segmentmarkerlist = (int *) NULL;
//
//	in.numberofholes = 0;
//	in.numberofregions = 0;
//
//	out.pointlist = (REAL *) NULL;
//	out.pointattributelist = (REAL *) NULL;
//	out.pointmarkerlist = (int *) NULL;
//	out.trianglelist = (int *) NULL;
//	out.triangleattributelist = (REAL *) NULL;
//	out.segmentlist = (int *) NULL;
//	out.segmentmarkerlist = (int *) NULL;
//
//	switch (type)
//	{
//	case e_Constrained:
//		triangulate("pz", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_Conforming:
//		triangulate("pzD", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingAngle:
//		triangulate("pzq", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingArea:
//		triangulate("pza10000", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingCount:
//		triangulate("pzu100", &in, &out, (struct triangulateio *) NULL);
//		break;
//	default:
//		assert(0);
//	}
//
//	index = 0;
//	for (size_t i = 0; i < out.numberoftriangles; ++i)
//	{
//		for (size_t j = 0; j < out.numberofcorners; ++j)
//		{
//			int pIndex = out.trianglelist[index++];
//
//			Vector p;
//			p.x = out.pointlist[pIndex * 2];
//			p.y = out.pointlist[pIndex * 2 + 1];
//			result.push_back(p);
//		}
//	}
//}
//
//void Math::triangulatePolygon(const std::vector<Vector>& polygon, const std::vector<Vector>& lines,
//							  std::vector<Vector>& result, TriangulateType type/* = e_Constrained*/)
//{
//	struct triangulateio in, out;
//
//	in.numberofpoints = polygon.size() + lines.size();
//	in.numberofpointattributes = 0;
//	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
//	in.pointmarkerlist = (int *) NULL;
//	int index = 0;
//	for (size_t i = 0, n = polygon.size(); i < n; ++i)
//	{
//		in.pointlist[index++] = polygon[i].x;
//		in.pointlist[index++] = polygon[i].y;
//	}
//	for (size_t i = 0, n = lines.size(); i < n; ++i)
//	{
//		in.pointlist[index++] = lines[i].x;
//		in.pointlist[index++] = lines[i].y;
//	}
//
//	in.numberofsegments = polygon.size() + lines.size() / 2;
//	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
//
//	index = 0;
//	for (size_t i = 0, n = polygon.size() - 1; i < n; ++i)
//	{
//		in.segmentlist[index++] = i;
//		in.segmentlist[index++] = i + 1;
//	}
//	in.segmentlist[index++] = polygon.size() - 1;
//	in.segmentlist[index++] = 0;
//
//	int lineIndex = polygon.size();
//	for (size_t i = 0, n = lines.size() / 2; i < n; ++i)
//	{
//		in.segmentlist[index++] = lineIndex++;
//		in.segmentlist[index++] = lineIndex++;
//	}
//
//	in.segmentmarkerlist = (int *) NULL;
//
//	in.numberofholes = 0;
//	in.numberofregions = 0;
//
//	out.pointlist = (REAL *) NULL;
//	out.pointattributelist = (REAL *) NULL;
//	out.pointmarkerlist = (int *) NULL;
//	out.trianglelist = (int *) NULL;
//	out.triangleattributelist = (REAL *) NULL;
//	out.segmentlist = (int *) NULL;
//	out.segmentmarkerlist = (int *) NULL;
//
//	switch (type)
//	{
//	case e_Constrained:
//		triangulate("pz", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_Conforming:
//		triangulate("pzD", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingAngle:
//		triangulate("pzq", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingArea:
//		triangulate("pza10000", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingCount:
//		triangulate("pzu100", &in, &out, (struct triangulateio *) NULL);
//		break;
//	default:
//		assert(0);
//	}
//
//	index = 0;
//	for (size_t i = 0; i < out.numberoftriangles; ++i)
//	{
//		std::vector<Vector> tri;
//		for (size_t j = 0; j < out.numberofcorners; ++j)
//		{
//			int pIndex = out.trianglelist[index++];
//
//			Vector p;
//			p.x = out.pointlist[pIndex * 2];
//			p.y = out.pointlist[pIndex * 2 + 1];
//			tri.push_back(p);
//		}
//
//		Vector center = Math::getTriGravityCenter(tri[0], tri[1], tri[2]);
//		if (Math::isPointInArea(center, polygon))
//			copy(tri.begin(), tri.end(), back_inserter(result));
//	}
//}
//
//void Math::triangulatePolygon(const std::vector<Vector>& polygon, const std::vector<Vector>& lines,
//							  const std::vector<std::vector<Vector> >& loops, std::vector<Vector>& result, TriangulateType type/* = e_Constrained*/)
//{
//	struct triangulateio in, out;
//
//	size_t loopSize = 0;
//	for (size_t i = 0, n = loops.size(); i < n; ++i)
//		loopSize += loops[i].size();
//
//	in.numberofpoints = polygon.size() + lines.size() + loopSize;
//	in.numberofpointattributes = 0;
//	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
//	in.pointmarkerlist = (int *) NULL;
//	int index = 0;
//	for (size_t i = 0, n = polygon.size(); i < n; ++i)
//	{
//		in.pointlist[index++] = polygon[i].x;
//		in.pointlist[index++] = polygon[i].y;
//	}
//	for (size_t i = 0, n = lines.size(); i < n; ++i)
//	{
//		in.pointlist[index++] = lines[i].x;
//		in.pointlist[index++] = lines[i].y;
//	}
//	for (size_t i = 0, n = loops.size(); i < n; ++i)
//	{
//		for (size_t j = 0, m = loops[i].size(); j < m; ++j)
//		{
//			in.pointlist[index++] = loops[i][j].x;
//			in.pointlist[index++] = loops[i][j].y;
//		}
//	}
//
//	in.numberofsegments = polygon.size() + lines.size() / 2 + loopSize;
//	in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
//
//	index = 0;
//	for (size_t i = 0, n = polygon.size() - 1; i < n; ++i)
//	{
//		in.segmentlist[index++] = i;
//		in.segmentlist[index++] = i + 1;
//	}
//	in.segmentlist[index++] = polygon.size() - 1;
//	in.segmentlist[index++] = 0;
//
//	int lineIndex = polygon.size();
//	for (size_t i = 0, n = lines.size() / 2; i < n; ++i)
//	{
//		in.segmentlist[index++] = lineIndex++;
//		in.segmentlist[index++] = lineIndex++;
//	}
//
//	int loopIndex = polygon.size() + lines.size();
//	for (size_t i = 0, n = loops.size(); i < n; ++i)
//	{
//		int start = loopIndex;
//		for (size_t j = 0, m = loops[i].size() - 1; j < m; ++j)
//		{
//			in.segmentlist[index++] = loopIndex;
//			in.segmentlist[index++] = loopIndex + 1;
//			++loopIndex;
//		}
//		in.segmentlist[index++] = loopIndex;
//		in.segmentlist[index++] = start;
//		++loopIndex;
//	}
//
//	in.segmentmarkerlist = (int *) NULL;
//
//	in.numberofholes = 0;
//	in.numberofregions = 0;
//
//	out.pointlist = (REAL *) NULL;
//	out.pointattributelist = (REAL *) NULL;
//	out.pointmarkerlist = (int *) NULL;
//	out.trianglelist = (int *) NULL;
//	out.triangleattributelist = (REAL *) NULL;
//	out.segmentlist = (int *) NULL;
//	out.segmentmarkerlist = (int *) NULL;
//
//	switch (type)
//	{
//	case e_Constrained:
//		triangulate("pz", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_Conforming:
//		triangulate("pzD", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingAngle:
//		triangulate("pzq", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingArea:
//		triangulate("pza10000", &in, &out, (struct triangulateio *) NULL);
//		break;
//	case e_ConstrainedConformingCount:
//		triangulate("pzu100", &in, &out, (struct triangulateio *) NULL);
//		break;
//	default:
//		assert(0);
//	}
//
//	index = 0;
//	for (size_t i = 0; i < out.numberoftriangles; ++i)
//	{
//		std::vector<Vector> tri;
//		for (size_t j = 0; j < out.numberofcorners; ++j)
//		{
//			int pIndex = out.trianglelist[index++];
//
//			Vector p;
//			p.x = out.pointlist[pIndex * 2];
//			p.y = out.pointlist[pIndex * 2 + 1];
//			tri.push_back(p);
//		}
//
//		Vector center = Math::getTriGravityCenter(tri[0], tri[1], tri[2]);
//		if (Math::isPointInArea(center, polygon))
//			copy(tri.begin(), tri.end(), back_inserter(result));
//	}
//}
//
//void Math::triangulateStrips(const std::vector<Vector>& triangulates, 
//							 std::vector<std::vector<Vector> >& strips)
//{
//	SGI::implement(triangulates, strips);
//}

void Math::computeQuadNodes(const Vector& center, float angle, float xScale, float yScale, 
							float width, float height, Vector quad[4])
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

} // d2d