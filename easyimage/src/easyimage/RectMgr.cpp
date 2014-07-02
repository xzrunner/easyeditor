#include "RectMgr.h"

namespace eimage
{

static const int RADIUS = 5;

RectMgr::~RectMgr()
{
	clear();
}

void RectMgr::load(const Json::Value& value)
{
	clear();

	int i = 0;
	Json::Value val = value["rect"][i++];
	while (!val.isNull()) {
		d2d::Rect* r = new d2d::Rect;
		r->xMin = val["xmin"].asDouble();
		r->xMax = val["xmax"].asDouble();
		r->yMin = val["ymin"].asDouble();
		r->yMax = val["ymax"].asDouble();
		m_rects.push_back(r);

		val = value["rect"][i++];
	}
}

void RectMgr::store(Json::Value& value) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const d2d::Rect* r = m_rects[i];
		value["rect"][i]["xmin"] = r->xMin;
		value["rect"][i]["xmax"] = r->xMax;
		value["rect"][i]["ymin"] = r->yMin;
		value["rect"][i]["ymax"] = r->yMax;
	}
}

void RectMgr::draw(const d2d::Screen& scr) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		d2d::PrimitiveDraw::rect(scr, *m_rects[i], d2d::LIGHT_RED_FACE);
		d2d::PrimitiveDraw::rect(scr, *m_rects[i], d2d::LIGHT_RED_THIN_LINE);
	}
}

void RectMgr::insert(const d2d::Rect& rect)
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		if (d2d::Math::isRectIntersectRect(*m_rects[i], rect)) 
		{
			return;
		}
	}

	m_rects.push_back(new d2d::Rect(rect));
}

bool RectMgr::remove(const d2d::Vector& pos)
{
	std::vector<d2d::Rect*>::iterator itr = m_rects.begin();
	for ( ; itr != m_rects.end(); ++itr)
	{
		if (d2d::Math::isPointInRect(pos, **itr)) {
			m_rects.erase(itr);
			return true;
		}
	}
	return false;
}

d2d::Vector RectMgr::queryNearestAxis(const d2d::Vector& pos,
									  const d2d::Rect* except) const
{
	d2d::Vector ret;
	ret.setInvalid();

	float minx = FLT_MAX,
		  miny = FLT_MAX;
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const d2d::Rect* r = m_rects[i];
		if (r == except) {
			continue;
		}

		float dx, dy;

		dx = fabs(pos.x - r->xMin);
		if (dx < RADIUS && dx < minx)
		{
			minx = dx;
			ret.x = r->xMin;
		}

		dx = fabs(pos.x - r->xMax);
		if (dx < RADIUS && dx < minx)
		{
			minx = dx;
			ret.x = r->xMax;
		}

		dy = fabs(pos.y - r->yMin);
		if (dy < RADIUS && dy < miny)
		{
			miny = dy;
			ret.y = r->yMin;
		}

		dy = fabs(pos.y - r->yMax);
		if (dy < RADIUS && dy < miny)
		{
			miny = dy;
			ret.y = r->yMax;
		}
	}

	return ret;
}

RectMgr::Node RectMgr::queryNode(const d2d::Vector& pos) const
{
	Node ret;
	ret.rect = NULL;

	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const d2d::Rect* r = m_rects[i];

		d2d::Vector selected;
		selected.setInvalid();

		if (fabs(pos.x - r->xMin) < RADIUS) {
			selected.x = r->xMin;
		} else if (fabs(pos.x - r->xMax) < RADIUS) {
			selected.x = r->xMax;
		}

		if (fabs(pos.y - r->yMin) < RADIUS) {
			selected.y = r->yMin;
		} else if (fabs(pos.y - r->yMax) < RADIUS) {
			selected.y = r->yMax;
		}

		if (selected.isValid()) {
			ret.rect = r;
			ret.pos = selected;
			return ret;
		}
	}

	return ret;
}

d2d::Rect* RectMgr::queryRect(const d2d::Vector& pos) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		if (d2d::Math::isPointInRect(pos, *m_rects[i])) {
			return m_rects[i];
		}
	}
	return NULL;
}

bool RectMgr::moveNode(const Node& node, const d2d::Vector& to)
{
	if (!node.rect) {
		return false;
	}

	d2d::Rect rect = *node.rect;

	float* ptr_x = NULL;
	if (rect.xMin == node.pos.x) {
		ptr_x = &rect.xMin;
	} else if (rect.xMax == node.pos.x) {
		ptr_x = &rect.xMax;
	}

	float* ptr_y = NULL;
	if (rect.yMin == node.pos.y) {
		ptr_y = &rect.yMin;
	} else if (rect.yMax == node.pos.y) {
		ptr_y = &rect.yMax;
	}

	if (ptr_x && ptr_y)
	{
		*ptr_x = to.x;
		*ptr_y = to.y;
		if (rect.xMin > rect.xMax) {
			std::swap(rect.xMin, rect.xMax);
		}
		if (rect.yMin > rect.yMax) {
			std::swap(rect.yMin, rect.yMax);
		}
	}

	if (rect.xMin == rect.xMax || rect.yMin == rect.yMax) 
	{
		return false;
	}
	else
	{
		d2d::Rect* r = const_cast<d2d::Rect*>(node.rect);
		*r = rect;
		return true;
	}
}

void RectMgr::moveRect(const d2d::Rect* rect, const d2d::Vector& from, const d2d::Vector& to)
{
	if (!rect) {
		return;
	}

	d2d::Rect* r = const_cast<d2d::Rect*>(rect);
	float dx = to.x - from.x,
		  dy = to.y - from.y;
	r->xMin += dx;
	r->xMax += dx;
	r->yMin += dy;
	r->yMax += dy;
}

void RectMgr::clear()
{
	for_each(m_rects.begin(), m_rects.end(), DeletePointerFunctor<d2d::Rect>());
	m_rects.clear();
}

}