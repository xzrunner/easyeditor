#include "RectMgr.h"

#include <ee/Rect.h>
#include <ee/PrimitiveDraw.h>
#include <ee/style_config.h>
#include <ee/Math2D.h>
#include <ee/std_functor.h>

#include <algorithm>

namespace eimage
{

static const int RADIUS = 5;

RectMgr::~RectMgr()
{
	Clear();
}

void RectMgr::Load(const Json::Value& value)
{
	Clear();

	int i = 0;
	Json::Value val = value["rect"][i++];
	while (!val.isNull()) {
		ee::Rect* r = new ee::Rect;
		r->xmin = static_cast<float>(val["xmin"].asDouble());
		r->xmax = static_cast<float>(val["xmax"].asDouble());
		r->ymin = static_cast<float>(val["ymin"].asDouble());
		r->ymax = static_cast<float>(val["ymax"].asDouble());
		m_rects.push_back(r);

		val = value["rect"][i++];
	}
}

void RectMgr::Store(Json::Value& value) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const ee::Rect* r = m_rects[i];
		value["rect"][i]["xmin"] = r->xmin;
		value["rect"][i]["xmax"] = r->xmax;
		value["rect"][i]["ymin"] = r->ymin;
		value["rect"][i]["ymax"] = r->ymax;
	}
}

void RectMgr::Draw() const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		ee::PrimitiveDraw::DrawRect(*m_rects[i], ee::LIGHT_RED_FACE);
		ee::PrimitiveDraw::DrawRect(*m_rects[i], ee::LIGHT_RED_THIN_LINE);
	}
}

void RectMgr::Insert(const ee::Rect& rect, bool force)
{
	if (!force) {
		for (int i = 0, n = m_rects.size(); i < n; ++i) {
			if (ee::Math2D::IsRectIntersectRect(*m_rects[i], rect)) {
				return;
			}
		}
	}

	m_rects.push_back(new ee::Rect(rect));
}

bool RectMgr::Remove(const ee::Vector& pos)
{
	std::vector<ee::Rect*>::iterator itr = m_rects.begin();
	for ( ; itr != m_rects.end(); ++itr)
	{
		if (ee::Math2D::IsPointInRect(pos, **itr)) {
			m_rects.erase(itr);
			return true;
		}
	}
	return false;
}

ee::Vector RectMgr::QueryNearestAxis(const ee::Vector& pos,
									  const ee::Rect* except) const
{
	ee::Vector ret;
	ret.SetInvalid();

	float minx = FLT_MAX,
		  miny = FLT_MAX;
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const ee::Rect* r = m_rects[i];
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

RectMgr::Node RectMgr::QueryNode(const ee::Vector& pos) const
{
	Node ret;
	ret.rect = NULL;

	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const ee::Rect* r = m_rects[i];

		ee::Vector selected;
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

ee::Rect* RectMgr::QueryRect(const ee::Vector& pos) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		if (ee::Math2D::IsPointInRect(pos, *m_rects[i])) {
			return m_rects[i];
		}
	}
	return NULL;
}

bool RectMgr::MoveNode(const Node& node, const ee::Vector& to)
{
	if (!node.rect) {
		return false;
	}

	ee::Rect rect = *node.rect;

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
		ee::Rect* r = const_cast<ee::Rect*>(node.rect);
		*r = rect;
		return true;
	}
}

void RectMgr::MoveRect(const ee::Rect* rect, const ee::Vector& from, const ee::Vector& to)
{
	if (!rect) {
		return;
	}

	ee::Rect* r = const_cast<ee::Rect*>(rect);
	float dx = to.x - from.x,
		  dy = to.y - from.y;
	r->xmin += dx;
	r->xmax += dx;
	r->ymin += dy;
	r->ymax += dy;
}

void RectMgr::Clear()
{
	for_each(m_rects.begin(), m_rects.end(), ee::DeletePointerFunctor<ee::Rect>());
	m_rects.clear();
}

}