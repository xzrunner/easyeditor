#include "RectMgr.h"

#include <ee/EE_RVG.h>
#include <ee/color_config.h>
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
		sm::rect* r = new sm::rect;
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
		const sm::rect* r = m_rects[i];
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
		sm::vec2 min(m_rects[i]->xmin, m_rects[i]->ymin),
			max(m_rects[i]->xmax, m_rects[i]->ymax);
		ee::RVG::Color(ee::LIGHT_RED);
		ee::RVG::Rect(min, max, true);
		ee::RVG::LineWidth(1);
		ee::RVG::Rect(min, max, true);
		ee::RVG::LineWidth(2);
	}
}

void RectMgr::Insert(const sm::rect& rect, bool force)
{
	if (!force) {
		for (int i = 0, n = m_rects.size(); i < n; ++i) {
			if (ee::Math2D::IsRectIntersectRect(*m_rects[i], rect)) {
				return;
			}
		}
	}

	m_rects.push_back(new sm::rect(rect));
}

bool RectMgr::Remove(const sm::vec2& pos)
{
	std::vector<sm::rect*>::iterator itr = m_rects.begin();
	for ( ; itr != m_rects.end(); ++itr)
	{
		if (ee::Math2D::IsPointInRect(pos, **itr)) {
			m_rects.erase(itr);
			return true;
		}
	}
	return false;
}

sm::vec2 RectMgr::QueryNearestAxis(const sm::vec2& pos, const sm::rect* except) const
{
	sm::vec2 ret;

	float minx = FLT_MAX,
		  miny = FLT_MAX;
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const sm::rect* r = m_rects[i];
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

RectMgr::Node RectMgr::QueryNode(const sm::vec2& pos) const
{
	Node ret;
	ret.rect = NULL;

	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		const sm::rect* r = m_rects[i];

		sm::vec2 selected;
		bool selected_valid = false;

		if (fabs(pos.x - r->xmin) < RADIUS) {
			selected.x = r->xmin;
			selected_valid = true;
		} else if (fabs(pos.x - r->xmax) < RADIUS) {
			selected.x = r->xmax;
			selected_valid = true;
		}

		if (fabs(pos.y - r->ymin) < RADIUS) {
			selected.y = r->ymin;
			selected_valid = true;
		} else if (fabs(pos.y - r->ymax) < RADIUS) {
			selected.y = r->ymax;
			selected_valid = true;
		}

		if (selected_valid) {
			ret.rect = r;
			ret.pos = selected;
			return ret;
		}
	}

	return ret;
}

sm::rect* RectMgr::QueryRect(const sm::vec2& pos) const
{
	for (int i = 0, n = m_rects.size(); i < n; ++i)
	{
		if (ee::Math2D::IsPointInRect(pos, *m_rects[i])) {
			return m_rects[i];
		}
	}
	return NULL;
}

bool RectMgr::MoveNode(const Node& node, const sm::vec2& to)
{
	if (!node.rect) {
		return false;
	}

	sm::rect rect = *node.rect;

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
		sm::rect* r = const_cast<sm::rect*>(node.rect);
		*r = rect;
		return true;
	}
}

void RectMgr::MoveRect(const sm::rect* rect, const sm::vec2& from, const sm::vec2& to)
{
	if (!rect) {
		return;
	}

	sm::rect* r = const_cast<sm::rect*>(rect);
	float dx = to.x - from.x,
		  dy = to.y - from.y;
	r->xmin += dx;
	r->xmax += dx;
	r->ymin += dy;
	r->ymax += dy;
}

void RectMgr::Clear()
{
	for_each(m_rects.begin(), m_rects.end(), ee::DeletePointerFunctor<sm::rect>());
	m_rects.clear();
}

}