#include "Math2D.h"

#include <SM_Calc.h>
#include <SM_Test.h>

#include <assert.h>

namespace ee
{

bool Math2D::IsCircleIntersectRect(const sm::vec2& center, float radius, const sm::rect& aabb)
{
	if (sm::is_point_in_rect(center, aabb))
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
	const bool test0 = sm::is_point_at_line_left(p, a, b),
		test1 = sm::is_point_at_line_left(p, b, c),
		test2 = sm::is_point_at_line_left(p, c, a);
	return test0 == test1 && test1 == test2;
}

bool Math2D::IsTwoLineParallel(const sm::vec2& s0, const sm::vec2& e0, const sm::vec2& s1, const sm::vec2& e1)
{
	float denominatorX = (e1.y - s1.y) * (e0.x - s0.x) - (e0.y - s0.y) * (e1.x - s1.x),
		  denominatorY = (e1.x - s1.x) * (e0.y - s0.y) - (e0.x - s0.x) * (e1.y - s1.y);
	return fabs(denominatorX) < FLT_EPSILON || fabs(denominatorY) < FLT_EPSILON;
}

int Math2D::CheckPosInTriangle(const sm::vec2& p, const sm::vec2& t0, const sm::vec2& t1, const sm::vec2& t2)
{
	if (p == t0 || p == t1 || p == t2)
		return 3;
	if (TestPointOnSection(t0, t1, p) == 0)
		return 0;
	if (TestPointOnSection(t1, t2, p) == 0)
		return 1;
	if (TestPointOnSection(t2, t0, p) == 0)
		return 2;

	if (sm::is_point_at_line_left(p, t0, t1) != sm::is_point_at_line_left(p, t0, t2)
		&& sm::is_point_at_line_left(p, t1, t2) != sm::is_point_at_line_left(p, t1, t0)
		&& sm::is_point_at_line_left(p, t2, t1) != sm::is_point_at_line_left(p, t2, t0))
		return 4;
	else 
		return -1;
}

float Math2D::GetDisPointToStraightLine(const sm::vec2& p, const sm::vec2& s, const sm::vec2& e)
{
	float divider = sm::dis_pos_to_pos(s, e);
	if (fabs(divider) < FLT_EPSILON) return sm::dis_pos_to_pos(p, s);
	else return fabs((s.x - p.x) * (e.y - p.y) - (s.y - p.y) * (e.x - p.x)) / divider;
}

float Math2D::GetDisPointToPolyline(const sm::vec2& p, const std::vector<sm::vec2>& polyline, size_t* iPos/* = NULL*/)
{
	if (polyline.size() < 2) return FLT_MAX;

	float nearest = FLT_MAX;
	size_t iNearest = -1;
	for (size_t i = 0, n = polyline.size() - 1; i < n; ++i)
	{
		const float dis = sm::dis_pos_to_seg(p, polyline[i], polyline[i + 1]);
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

float Math2D::GetDisPointToMultiPos(const sm::vec2& p, const std::vector<std::vector<sm::vec2> >& pos, size_t* index0/* = NULL*/, size_t* index1/* = NULL*/)
{
	float nearest = FLT_MAX;
	size_t iNearest0 = -1, iNearest1 = -1;
	for (size_t i = 0, n = pos.size(); i < n; ++i)
	{
		int index;
		const float dis = sm::dis_pos_to_multi_pos(p, pos[i], &index);
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
		perimeter += sm::dis_pos_to_pos(poly[i], poly[i+1]);
	}
	perimeter += sm::dis_pos_to_pos(poly[0], poly[poly.size()-1]);

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

int Math2D::TestPointOnSection(const sm::vec2& startPos, const sm::vec2& endPos, const sm::vec2& thdPos, float tolerance/* = FLT_EPSILON*/)
{
	assert(tolerance > 0.0);
	sm::rect aabb;
	aabb.xmin = thdPos.x - tolerance / 2.0f;
	aabb.xmax = thdPos.x + tolerance / 2.0f;
	aabb.ymin = thdPos.y - tolerance / 2.0f;
	aabb.ymax = thdPos.y + tolerance / 2.0f;

	if (!sm::is_rect_intersect_segment(aabb, startPos, endPos)) return -1;

	int resultA = 0;
	int resultB = 0;
	if (sm::is_point_in_rect(startPos, aabb)) resultA = 1;
	if (sm::is_point_in_rect(endPos, aabb)) resultB = 2;

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

void Math2D::GetNearestPosOnLineToPoint(const sm::vec2& p, const std::vector<sm::vec2>& l, sm::vec2* nearest, size_t* index)
{
	size_t iNearestPos = 0;
	float nearestDis = sm::dis_pos_to_pos(p, l.at(iNearestPos));
	for (size_t i = 1; i < l.size(); ++i)
	{
		float dis = sm::dis_pos_to_pos(p, l.at(i));
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
		if (sm::get_foot_of_perpendicular(l.at(iNearestPos), l.at(iNearestPos + 1), p, &foot) == 0)
		{
			float dis = sm::dis_pos_to_pos(foot, p);
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
		if (sm::get_foot_of_perpendicular(l.at(iNearestPos), l.at(iNearestPos - 1), p, &foot) == 0)
		{
			float dis = sm::dis_pos_to_pos(foot, p);
			if (dis < nearestDis)
			{
				*nearest = foot;
				*index = iNearestPos - 1;
			}
		}
	}
}

sm::vec2 Math2D::TransCoordsLocalToWorld(const sm::vec2& origin, const sm::vec2& xDir, const sm::vec2& local)
{
	sm::vec2 nx(xDir);
	nx.Normalize();
	sm::vec2 ny = sm::rotate_vector_right_angle(nx, true);
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

	float len = sm::dis_pos_to_pos(s, e);
	float t = dis / len;
	
	if (toleft) {
		ds = sm::rotate_vector_right_angle(e - s, true) * t + s;
		de = sm::rotate_vector_right_angle(s - e, false) * t + e;
	} else {
		ds = sm::rotate_vector_right_angle(e - s, false) * t + s;
		de = sm::rotate_vector_right_angle(s - e, true) * t + e;
	}
}

}