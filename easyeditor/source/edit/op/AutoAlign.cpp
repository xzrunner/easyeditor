#include "AutoAlign.h"
#include "Symbol.h"
#include "Sprite.h"
#include "MultiSpritesImpl.h"
#include "FetchAllVisitor.h"

#include <sprite2/S2_RVG.h>
#include <sprite2/Color.h>
#include <sprite2/BoundingBox.h>

namespace ee
{

AutoAlign::AutoAlign(MultiSpritesImpl* sprites_impl)
	: m_sprites_impl(sprites_impl)
	, m_open(true)
{
}

void AutoAlign::Align(const std::vector<Sprite*>& sprs)
{
	m_hor[0].Set(0, 0);
	m_hor[1].Set(0, 0);
	m_ver[0].Set(0, 0);
	m_ver[1].Set(0, 0);

	std::vector<Sprite*> sources;
	m_sprites_impl->TraverseSprites(FetchAllVisitor<Sprite>(sources));

	// not support mul src now
	if (sprs.size() > 1)
		return;

	const float DIS = 5;

	Sprite *hor_nearest = NULL, *ver_nearest = NULL;
	float dis_hor = DIS, dis_ver = DIS;
	// hor
	for (size_t i = 0, n = sources.size(); i < n; ++i)
	{
		Sprite *dst = sprs[0], *src = sources[i];
		if (src == dst) continue;

		sm::rect src_rect = src->GetBounding()->GetSize(),
			     dst_rect = dst->GetBounding()->GetSize();

		float src_cy = src_rect.Center().y;
		float src_down = src_rect.ymin;
		float src_up = src_rect.ymax;

		float dst_cy = dst_rect.Center().y;
		float dst_down = dst_rect.ymin;
		float dst_up = dst_rect.ymax;

		// up
		if (float dis = fabs(dst_up - src_up) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}
		else if (float dis = fabs(dst_up - src_down) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}
		// down
		if (float dis = fabs(dst_down - src_up) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}
		else if (float dis = fabs(dst_down - src_down) < dis_hor)
		{
			dis_hor = dis;
			hor_nearest = src;
		}	
	}
	// ver
	for (size_t i = 0, n = sources.size(); i < n; ++i)
	{
		Sprite *dst = sprs[0], *src = sources[i];
		if (src == dst) continue;

		sm::rect src_rect = src->GetBounding()->GetSize(),
			     dst_rect = dst->GetBounding()->GetSize();

		float src_cx = src_rect.Center().x;
		float src_left = src_rect.xmin;
		float src_right = src_rect.xmax;

		float dst_cx = dst_rect.Center().x;
		float dst_left = dst_rect.xmin;
		float dst_right = dst_rect.xmax;

		// left
		if (float dis = fabs(dst_left - src_left) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
		else if (float dis = fabs(dst_left - src_right) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
		// right
		if (float dis = fabs(dst_right - src_left) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
		else if (float dis = fabs(dst_right - src_right) < dis_ver)
		{
			dis_ver = dis;
			ver_nearest = src;
		}
	}

	if (hor_nearest)
		Align(hor_nearest, sprs[0]);
	if (ver_nearest && ver_nearest != hor_nearest)
		Align(ver_nearest, sprs[0]);
}

void AutoAlign::Align(const Sprite* src, Sprite* dst)
{
	const float DIS = 5;
	const float LEN = 400;

	sm::rect src_rect = src->GetBounding()->GetSize(),
		     dst_rect = dst->GetBounding()->GetSize();

	float src_left	= src_rect.xmin,
		  src_right = src_rect.xmax,
		  src_down	= src_rect.ymin,
		  src_up	= src_rect.ymax;
	float src_cx	= src_rect.Center().x,
		  src_cy	= src_rect.Center().y;

	// up
	float nearest = DIS;
	if (float dis = fabs(dst_rect.ymax - src_up) < nearest)
	{
		float dy = src_up - dst_rect.ymax;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x, dst->GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_rect.ymax - src_down) < nearest)
	{
		float dy = src_down - dst_rect.ymax;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x, dst->GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}		
	// down
	if (float dis = fabs(dst_rect.ymin - src_up) < nearest)
	{
		float dy = src_up - dst_rect.ymin;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x, dst->GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_rect.ymin - src_down) < nearest)
	{
		float dy = src_down - dst_rect.ymin;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x, dst->GetPosition().y + dy));
		dst_rect.Translate(sm::vec2(0, dy));
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}	
	// left
	nearest = DIS;
	if (float dis = fabs(dst_rect.xmin - src_left) < nearest)
	{
		float dx = src_left - dst_rect.xmin;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x + dx, dst->GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_rect.xmin - src_right) < nearest)
	{
		float dx = src_right - dst_rect.xmin;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x + dx, dst->GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
	// right
	if (float dis = fabs(dst_rect.xmax - src_left) < nearest)
	{
		float dx = src_left - dst_rect.xmax;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x + dx, dst->GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_rect.xmax - src_right) < nearest)
	{
		float dx = src_right - dst_rect.xmax;
		nearest = dis;
		dst->SetPosition(sm::vec2(dst->GetPosition().x + dx, dst->GetPosition().y));
		dst_rect.Translate(sm::vec2(dx, 0));
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
}

void AutoAlign::Draw() const
{
	if (m_open)
	{
		s2::RVG::SetColor(s2::Color(0, 0, 0));
		if (m_hor[0] != m_hor[1]) {
			s2::RVG::DashLine(m_hor[0], m_hor[1]);
		}
		if (m_ver[0] != m_ver[1]) {
			s2::RVG::DashLine(m_ver[0], m_ver[1]);
		}
	}
}

void AutoAlign::SetInvisible() 
{ 
	m_hor[0] = m_hor[1]; 
	m_ver[0] = m_ver[1]; 
}

}