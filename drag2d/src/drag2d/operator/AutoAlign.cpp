#include "AutoAlign.h"

#include "dataset/AbstractBV.h"
#include "dataset/ISymbol.h"
#include "view/MultiSpritesImpl.h"
#include "render/PrimitiveDraw.h"

namespace d2d
{

AutoAlign::AutoAlign(MultiSpritesImpl* sprites_impl)
	: m_sprites_impl(sprites_impl)
	, m_open(true)
{
}

void AutoAlign::Align(const std::vector<ISprite*>& sprites)
{
	m_hor[0].set(0, 0);
	m_hor[1].set(0, 0);
	m_ver[0].set(0, 0);
	m_ver[1].set(0, 0);

	std::vector<ISprite*> sources;
	m_sprites_impl->traverseSprites(FetchAllVisitor<ISprite>(sources));

	// not support multi src now
	if (sprites.size() > 1)
		return;

	const float DIS = 5;

	ISprite *horNearest = NULL, *verNearest = NULL;
	float disHor = DIS, disVer = DIS;
	// hor
	for (size_t i = 0, n = sources.size(); i < n; ++i)
	{
		ISprite *dst = sprites[0], *src = sources[i];

		if (src == dst) continue;

		const float srcHalfHeight = src->getBounding()->height() * 0.5f;
		const float dstHalfHeight = dst->getBounding()->height() * 0.5f;

		float src_cy = src->getPosition().y + src->getSymbol().getSize().yCenter();
		float srcDown = src_cy - srcHalfHeight;
		float srcUp = src_cy + srcHalfHeight;

		float dst_cy = dst->getPosition().y + dst->getSymbol().getSize().yCenter();
		float dstDown = dst_cy - dstHalfHeight;
		float dstUp = dst_cy + dstHalfHeight;

		// up
		if (float dis = fabs(dstUp - srcUp) < disHor)
		{
			disHor = dis;
			horNearest = src;
		}
		else if (float dis = fabs(dstUp - srcDown) < disHor)
		{
			disHor = dis;
			horNearest = src;
		}
		// down
		if (float dis = fabs(dstDown - srcUp) < disHor)
		{
			disHor = dis;
			horNearest = src;
		}
		else if (float dis = fabs(dstDown - srcDown) < disHor)
		{
			disHor = dis;
			horNearest = src;
		}	
	}
	// ver
	for (size_t i = 0, n = sources.size(); i < n; ++i)
	{
		ISprite *dst = sprites[0], *src = sources[i];

		if (src == dst) continue;

		const float srcHalfWidth = src->getBounding()->width() * 0.5f;
		const float dstHalfWidth = dst->getBounding()->width() * 0.5f;

		float src_cx = src->getPosition().x + src->getSymbol().getSize().xCenter();
		float srcLeft = src_cx - srcHalfWidth;
		float srcRight = src_cx + srcHalfWidth;

		float dst_cx = dst->getPosition().x + dst->getSymbol().getSize().xCenter();
		float dstLeft = dst_cx - dstHalfWidth;
		float dstRight = dst_cx + dstHalfWidth;

		// left
		if (float dis = fabs(dstLeft - srcLeft) < disVer)
		{
			disVer = dis;
			verNearest = src;
		}
		else if (float dis = fabs(dstLeft - srcRight) < disVer)
		{
			disVer = dis;
			verNearest = src;
		}
		// right
		if (float dis = fabs(dstRight - srcLeft) < disVer)
		{
			disVer = dis;
			verNearest = src;
		}
		else if (float dis = fabs(dstRight - srcRight) < disVer)
		{
			disVer = dis;
			verNearest = src;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	if (horNearest)
		Align(horNearest, sprites[0]);
	if (verNearest && verNearest != horNearest)
		Align(verNearest, sprites[0]);

	//////////////////////////////////////////////////////////////////////////

	// 		for (size_t i = 0, n = sprites.size(); i < n; ++i)
	// 		{
	// 			for (size_t j = 0, m = sources.size(); j < m; ++j)
	// 			{
	// 				if (sprites[i] != sources[j])
	// 					AutoAlign(sources[j], sprites[i]);
	// 			}
	// 		}
}

void AutoAlign::Align(const ISprite* src, ISprite* dst)
{
	const float DIS = 5;
	const float LEN = 400;

	Rect sr = src->getSymbol().getSize();
	Rect dr = dst->getSymbol().getSize();

	const float srcHalfWidth = sr.xLength() * src->getScale().x * 0.5f,
		srcHalfHeight = sr.yLength() * src->getScale().y * 0.5f;
	const float dstHalfWidth = dr.xLength() * dst->getScale().x * 0.5f,
		dstHalfHeight = dr.yLength() * dst->getScale().y * 0.5f;

	float src_dx = sr.xCenter();
	float src_dy = sr.yCenter();
	float dst_dx = dr.xCenter();
	float dst_dy = dr.yCenter();

	float srcLeft = src->getPosition().x + src_dx - srcHalfWidth;
	float srcRight = src->getPosition().x + src_dx + srcHalfWidth;
	float srcDown = src->getPosition().y + src_dy - srcHalfHeight;
	float srcUp = src->getPosition().y + src_dy + srcHalfHeight;

	// up
	float nearest = DIS;
	float dstUp = dst->getPosition().y + dst_dy + dstHalfHeight;
	if (float dis = fabs(dstUp - srcUp) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(dst->getPosition().x, srcUp - dstHalfHeight - dst_dy), dst->getAngle());
		m_hor[0].set(dst->getPosition().x + dst_dx - LEN, srcUp);
		m_hor[1].set(dst->getPosition().x + dst_dx + LEN, srcUp);
	}
	else if (float dis = fabs(dstUp - srcDown) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(dst->getPosition().x, srcDown - dstHalfHeight - dst_dy), dst->getAngle());
		m_hor[0].set(dst->getPosition().x + dst_dx - LEN, srcDown);
		m_hor[1].set(dst->getPosition().x + dst_dx + LEN, srcDown);
	}		
	// down
	float dstDown = dst->getPosition().y + dst_dy - dstHalfHeight;
	if (float dis = fabs(dstDown - srcUp) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(dst->getPosition().x, srcUp + dstHalfHeight - dst_dy), dst->getAngle());
		m_hor[0].set(dst->getPosition().x + dst_dx - LEN, srcUp);
		m_hor[1].set(dst->getPosition().x + dst_dx + LEN, srcUp);
	}
	else if (float dis = fabs(dstDown - srcDown) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(dst->getPosition().x, srcDown + dstHalfHeight - dst_dy), dst->getAngle());
		m_hor[0].set(dst->getPosition().x + dst_dx - LEN, srcDown);
		m_hor[1].set(dst->getPosition().x + dst_dx + LEN, srcDown);
	}	
	// left
	nearest = DIS;
	float dstLeft = dst->getPosition().x + dst_dx - dstHalfWidth;
	if (float dis = fabs(dstLeft - srcLeft) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(srcLeft + dstHalfWidth - dst_dx, dst->getPosition().y), dst->getAngle());
		m_ver[0].set(srcLeft, dst->getPosition().y + dst_dy - LEN);
		m_ver[1].set(srcLeft, dst->getPosition().y + dst_dy + LEN);
	}
	else if (float dis = fabs(dstLeft - srcRight) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(srcRight + dstHalfWidth - dst_dx, dst->getPosition().y), dst->getAngle());
		m_ver[0].set(srcRight, dst->getPosition().y + dst_dy - LEN);
		m_ver[1].set(srcRight, dst->getPosition().y + dst_dy + LEN);
	}
	// right
	float dstRight = dst->getPosition().x + dst_dx + dstHalfWidth;
	if (float dis = fabs(dstRight - srcLeft) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(srcLeft - dstHalfWidth - dst_dx, dst->getPosition().y), dst->getAngle());
		m_ver[0].set(srcLeft, dst->getPosition().y + dst_dy - LEN);
		m_ver[1].set(srcLeft, dst->getPosition().y + dst_dy + LEN);
	}
	else if (float dis = fabs(dstRight - srcRight) < nearest)
	{
		nearest = dis;
		dst->setTransform(Vector(srcRight - dstHalfWidth - dst_dx, dst->getPosition().y), dst->getAngle());
		m_ver[0].set(srcRight, dst->getPosition().y + dst_dy - LEN);
		m_ver[1].set(srcRight, dst->getPosition().y + dst_dy + LEN);
	}
}

void AutoAlign::Draw(const Screen& scr) const
{
	if (m_open)
	{
		if (m_hor[0] != m_hor[1]) {
			PrimitiveDraw::drawDashLine(scr, m_hor[0], m_hor[1], Colorf(0, 0, 0));
		}
		if (m_ver[0] != m_ver[1]) {
			PrimitiveDraw::drawDashLine(scr, m_ver[0], m_ver[1], Colorf(0, 0, 0));
		}
	}
}

void AutoAlign::SetInvisible() 
{ 
	m_hor[0] = m_hor[1]; 
	m_ver[0] = m_ver[1]; 
}

}