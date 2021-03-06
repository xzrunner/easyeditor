#include "ImageSprite.h"
#include "ImageSymbol.h"
#include "FileHelper.h"
#include "SymbolFile.h"
#include "Math2D.h"

#include <sprite2/ImageSprite.h>
#include <painting2/OBB.h>
#include <painting2/FilterMode.h>

namespace ee
{

ImageSprite::ImageSprite(const ImageSprite& spr)
	: s2::Sprite(spr)
	, s2::ImageSprite(spr)
	, Sprite(spr)
{
}

ImageSprite& ImageSprite::operator = (const ImageSprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::ImageSprite::operator = (spr);
	Sprite::operator = (spr);
	return *this;
}

ImageSprite::ImageSprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::ImageSprite(sym)
	, Sprite(sym)
{
}

void ImageSprite::BuildBoundingFromTexCoords(float* texCoords)
{
	if (!m_sym) return;

	sm::vec2 sz = m_sym->GetBounding().Size();
 	sm::rect rect;
 	rect.xmin = sz.x * (texCoords[0] - 0.5f);
 	rect.xmax = sz.x * (texCoords[2] - 0.5f);
 	rect.ymin = sz.y * (texCoords[1] - 0.5f);
 	rect.ymax = sz.y * (texCoords[3] - 0.5f);
 
 	sm::vec2 center = rect.Center();
	const sm::vec2& scale = GetScale();
 	float hw = (rect.xmax - rect.xmin) * 0.5f * scale.x,
 		  hh = (rect.ymax - rect.ymin) * 0.5f * scale.y;
 	rect.xmin = -hw;
 	rect.xmax = hw;
 	rect.ymin = -hh;
 	rect.ymax = hh;

	auto& bb = GetBounding();
	const_cast<pt2::BoundingBox&>(bb).Build(
		rect, GetPosition(), GetAngle(), scale, GetShear(), GetOffset());
}

}