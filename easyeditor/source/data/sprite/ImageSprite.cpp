#include "ImageSprite.h"
#include "SpriteFactory.h"
#include "FileHelper.h"
#include "FileType.h"
#include "Math2D.h"
#include "BBFactory.h"
#include "BoundingBox.h"

#include <sprite2/RenderShader.h>
#include <sprite2/ImageSprite.h>
#include <sprite2/FilterMode.h>
#include <sprite2/RenderFilter.h>

namespace ee
{

ImageSprite::ImageSprite()
	: m_symbol(NULL)
{
	m_core = new s2::ImageSprite(this);
}

ImageSprite::ImageSprite(const ImageSprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::ImageSprite(*static_cast<s2::ImageSprite*>(sprite.m_core), this);

	m_symbol->Retain();
}

ImageSprite::ImageSprite(ImageSymbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::ImageSprite(this);
	m_symbol->Retain();
	BuildBounding();
}

ImageSprite::~ImageSprite()
{
	m_core->RemoveReference();
	if (m_symbol) {
		m_symbol->Release();
	}
}

ImageSprite* ImageSprite::Clone() const
{
	ImageSprite* sprite = new ImageSprite(*this);
	SpriteFactory::Instance()->Insert(sprite);
	return sprite;
}

bool ImageSprite::Update(const s2::RenderParams& params, float dt)
{
	if (m_core->Update(dt)) return true;

	s2::FilterMode filter = GetShader().filter->GetMode();
	return filter == s2::FM_HEAT_HAZE 
		|| filter == s2::FM_SHOCK_WAVE 
		|| filter == s2::FM_SWIRL
		|| filter == s2::FM_BURNING_MAP;
}

const ImageSymbol& ImageSprite::GetSymbol() const
{
	return *m_symbol;
}

void ImageSprite::SetSymbol(Symbol* symbol)
{
	Sprite::SetSymbol(&m_symbol, symbol);
}

void ImageSprite::BuildBoundingFromTexCoords(float* texCoords)
{
	if (!m_symbol) return;

	sm::vec2 sz = m_symbol->GetSize().Size();

	ee::BoundingBox* bb = BBFactory::CreateBB(e_obb);
 
 	sm::rect rect;
 	rect.xmin = sz.x * (texCoords[0] - 0.5f);
 	rect.xmax = sz.x * (texCoords[2] - 0.5f);
 	rect.ymin = sz.y * (texCoords[1] - 0.5f);
 	rect.ymax = sz.y * (texCoords[3] - 0.5f);
 
 	sm::vec2 center = rect.Center();
 	float hWidth = (rect.xmax - rect.xmin) * 0.5f * m_core->GetScale().x,
 		hHeight = (rect.ymax - rect.ymin) * 0.5f * m_core->GetScale().y;
 	rect.xmin = -hWidth;
 	rect.xmax = hWidth;
 	rect.ymin = -hHeight;
 	rect.ymax = hHeight;
 
 	sm::vec2 offset = Math2D::RotateVector(center, m_core->GetAngle());
 	rect.Translate(m_core->GetPosition() + offset);
 	bb->InitFromRect(rect);
 	bb->SetTransform(m_core->GetPosition(), GetOffset(), m_core->GetAngle());
	SetBounding(bb);
}

}