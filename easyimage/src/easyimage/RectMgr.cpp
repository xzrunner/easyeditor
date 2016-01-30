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
		r->xmin = val["xmin"].asDouble();
		r->xmax = val["xmax"].asDouble();
		r->ymin = val["ymin"].asDouble();
		r->ymax = val["ymax"].asDouble();
		m_rects.push_back(r);

		val = value["rect"][i++];
	}
}

void RectMgr::store(Json::Value& value) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const d2d::Rect* r = m_rects[i];
		value["rect"][i]["xmin"] = r->xmin;
		value["rect"][i]["xmax"] = r->xmax;
		value["rect"][i]["ymin"] = r->ymin;
		value["rect"][i]["ymax"] = r->ymax;
	}
}

void RectMgr::draw() const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		d2d::PrimitiveDraw::DrawRect(*m_rects[i], d2d::LIGHT_RED_FACE);
		d2d::PrimitiveDraw::DrawRect(*m_rects[i], d2d::LIGHT_RED_THIN_LINE);
	}
}

void RectMgr::insert(const d2d::Rect& rect, bool force)
{
	if (!force) {
		for (int i = 0, n = m_rects.size(); i < n; ++i) {
			if (d2d::Math2D::IsRectIntersectRect(*m_rects[i], rect)) {
				return;
			}
		}
	}

	m_rects.push_back(new d2d::Rect(rect));
}

bool RectMgr::remove(const d2d::Vector& pos)
{
	std::vector<d2d::Rect*>::iterator itr = m_rects.begin();
	for ( ; itr != m_rects.end(); ++itr)
	{
		if (d2d::Math2D::IsPointInRect(pos, **itr)) {
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
	ret.SetInvalid();

	float minx = FLT_MAX,
		  miny = FLT_MAX;
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const d2d::Rect* r = m_rects[i];
		if (r == except) {
			continue;
		}

		float dx, dy;

		dx = fabs(pos.x - r->xmin);
		if (dx < RADIUS && dx < minx)
		{
			minx = dx;
			ret.x = r->xmin;
		}

		dx = fabs(pos.x - r->xmax);
		if (dx < RADIUS && dx < minx)
		{
			minx = dx;
			ret.x = r->xmax;
		}

		dy = fabs(pos.y - r->ymin);
		if (dy < RADIUS && dy < miny)
		{
			miny = dy;
			ret.y = r->ymin;
		}

		dy = fabs(pos.y - r->ymax);
		if (dy < RADIUS && dy < miny)
		{
			miny = dy;
			ret.y = r->ymax;
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
		selected.SetInvalid();

		if (fabs(pos.x - r->xmin) < RADIUS) {
			selected.x = r->xmin;
		} else if (fabs(pos.x - r->xmax) < RADIUS) {
			selected.x = r->xmax;
		}

		if (fabs(pos.y - r->ymin) < RADIUS) {
			selected.y = r->ymin;
		} else if (fabs(pos.y - r->ymax) < RADIUS) {
			selected.y = r->ymax;
		}

		if (selected.IsValid()) {
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
		if (d2d::Math2D::IsPointInRect(pos, *m_rects[i])) {
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
	if (rect.xmin == node.pos.x) {
		ptr_x = &rect.xmin;
	} else if (rect.xmax == node.pos.x) {
		ptr_x = &rect.xmax;
	}

	float* ptr_y = NULL;
	if (rect.ymin == node.pos.y) {
		ptr_y = &rect.ymin;
	} else if (rect.ymax == node.pos.y) {
		ptr_y = &rect.ymax;
	}

	if (ptr_x && ptr_y)
	{
		*ptr_x = to.x;
		*ptr_y = to.y;
		if (rect.xmin > rect.xmax) {
			std::swap(rect.xmin, rect.xmax);
		}
		if (rect.ymin > rect.ymax) {
			std::swap(rect.ymin, rect.ymax);
		}
	}

	if (rect.xmin == rect.xmax || rect.ymin == rect.ymax) 
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
	r->xmin += dx;
	r->xmax += dx;
	r->ymin += dy;
	r->ymax += dy;
}

void RectMgr::clear()
{
	for_each(m_rects.begin(), m_rects.end(), DeletePointerFunctor<d2d::Rect>());
	m_rects.clear();
}

}