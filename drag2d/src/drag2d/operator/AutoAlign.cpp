#include "AutoAlign.h"

#include "dataset/BoundingBox.h"
#include "dataset/Symbol.h"
#include "dataset/Sprite.h"
#include "dataset/sprite_utility.h"
#include "view/MultiSpritesImpl.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

AutoAlign::AutoAlign(MultiSpritesImpl* sprites_impl)
	: m_sprites_impl(sprites_impl)
	, m_open(true)
{
}

void AutoAlign::Align(const std::vector<Sprite*>& sprites)
{
	m_hor[0].Set(0, 0);
	m_hor[1].Set(0, 0);
	m_ver[0].Set(0, 0);
	m_ver[1].Set(0, 0);

	std::vector<Sprite*> sources;
	m_sprites_impl->TraverseSprites(FetchAllVisitor<Sprite>(sources));

	// not support multi src now
	if (sprites.size() > 1)
		return;

	const float DIS = 5;

	Sprite *hor_nearest = NULL, *ver_nearest = NULL;
	float dis_hor = DIS, dis_ver = DIS;
	// hor
	for (size_t i = 0, n = sources.size(); i < n; ++i)
	{
		Sprite *dst = sprites[0], *src = sources[i];

		if (src == dst) continue;

		float src_cy = get_spr_center_y(src);
		float src_down = get_spr_down(src);
		float src_up = get_spr_up(src);

		float dst_cy = get_spr_center_y(dst);
		float dst_down = get_spr_down(dst);
		float dst_up = get_spr_up(dst);

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
		Sprite *dst = sprites[0], *src = sources[i];

		if (src == dst) continue;

		float src_cx = get_spr_center_x(src);
		float src_left = get_spr_left(src);
		float src_right = get_spr_right(src);

		float dst_cx = get_spr_center_x(dst);
		float dst_left = get_spr_left(dst);
		float dst_right = get_spr_right(dst);

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
		Align(hor_nearest, sprites[0]);
	if (ver_nearest && ver_nearest != hor_nearest)
		Align(ver_nearest, sprites[0]);
}

void AutoAlign::Align(const Sprite* src, Sprite* dst)
{
	const float DIS = 5;
	const float LEN = 400;

	float src_left = get_spr_left(src),
		src_right = get_spr_right(src),
		src_down = get_spr_down(src),
		src_up = get_spr_up(src);
	float src_cx = get_spr_center_x(src),
		src_cy = get_spr_center_y(src);

	// up
	float nearest = DIS;
	float dst_up = get_spr_up(dst);
	if (float dis = fabs(dst_up - src_up) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x, dst->GetPosition().y + src_up - dst_up), dst->GetAngle());
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_up - src_down) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x, dst->GetPosition().y + src_down - dst_up), dst->GetAngle());
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}		
	// down
	float dst_down = get_spr_down(dst);;
	if (float dis = fabs(dst_down - src_up) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x, dst->GetPosition().y + src_up - dst_down), dst->GetAngle());
		m_hor[0].Set(src_cx - LEN, src_up);
		m_hor[1].Set(src_cx + LEN, src_up);
	}
	else if (float dis = fabs(dst_down - src_down) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x, dst->GetPosition().y + src_down - dst_down), dst->GetAngle());
		m_hor[0].Set(src_cx - LEN, src_down);
		m_hor[1].Set(src_cx + LEN, src_down);
	}	
	// left
	nearest = DIS;
	float dst_left = get_spr_left(dst);
	if (float dis = fabs(dst_left - src_left) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x + src_left - dst_left, dst->GetPosition().y), dst->GetAngle());
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_left - src_right) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x + src_right - dst_left, dst->GetPosition().y), dst->GetAngle());
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
	// right
	float dst_right = get_spr_right(dst);
	if (float dis = fabs(dst_right - src_left) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x + src_left - dst_right, dst->GetPosition().y), dst->GetAngle());
		m_ver[0].Set(src_left, src_cy - LEN);
		m_ver[1].Set(src_left, src_cy + LEN);
	}
	else if (float dis = fabs(dst_right - src_right) < nearest)
	{
		nearest = dis;
		dst->SetTransform(Vector(dst->GetPosition().x + src_right - dst_right, dst->GetPosition().y), dst->GetAngle());
		m_ver[0].Set(src_right, src_cy - LEN);
		m_ver[1].Set(src_right, src_cy + LEN);
	}
}

void AutoAlign::Draw() const
{
	if (m_open)
	{
		if (m_hor[0] != m_hor[1]) {
			PrimitiveDraw::DrawDashLine(m_hor[0], m_hor[1], Colorf(0, 0, 0));
		}
		if (m_ver[0] != m_ver[1]) {
			PrimitiveDraw::DrawDashLine(m_ver[0], m_ver[1], Colorf(0, 0, 0));
		}
	}
}

void AutoAlign::SetInvisible() 
{ 
	m_hor[0] = m_hor[1]; 
	m_ver[0] = m_ver[1]; 
}

}