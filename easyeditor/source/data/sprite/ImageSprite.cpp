#include "ImageSprite.h"
#include "SpriteFactory.h"
#include "FileHelper.h"
#include "FileType.h"
#include "Math2D.h"
#include "BBFactory.h"
#include "BoundingBox.h"

#include <sprite2/RenderShader.h>
#include <sprite2/ImageSprite.h>

namespace ee
{

ImageSprite::ImageSprite()
	: m_symbol(NULL)
{
	m_core = new s2::ImageSprite;
}

ImageSprite::ImageSprite(const ImageSprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_core = new s2::ImageSprite(*static_cast<s2::ImageSprite*>(sprite.m_core));

	m_symbol->Retain();
}

ImageSprite::ImageSprite(ImageSymbol* symbol)
	: m_symbol(symbol)
{
	m_core = new s2::ImageSprite;
	m_symbol->Retain();
	BuildBounding();
}

ImageSprite::~ImageSprite()
{
	delete m_core; m_core = NULL;
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

bool ImageSprite::Update(float dt)
{
	if (m_core->Update(dt)) return true;

	return GetShader().filter == s2::FM_HEAT_HAZE 
		|| GetShader().filter == s2::FM_SHOCK_WAVE 
		|| GetShader().filter == s2::FM_SWIRL;
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

 	int width = m_symbol->GetSize().Width(),
 		height = m_symbol->GetSize().Height();
 
 	delete m_bounding;
 	m_bounding = BBFactory::CreateBB(e_obb);
 
 	Rect rect;
 	rect.xmin = width * (texCoords[0] - 0.5f);
 	rect.xmax = width * (texCoords[2] - 0.5f);
 	rect.ymin = height * (texCoords[1] - 0.5f);
 	rect.ymax = height * (texCoords[3] - 0.5f);
 
 	sm::vec2 center(rect.CenterX(), rect.CenterY());
 	float hWidth = (rect.xmax - rect.xmin) * 0.5f * m_core->Scale().x,
 		hHeight = (rect.ymax - rect.ymin) * 0.5f * m_core->Scale().y;
 	rect.xmin = -hWidth;
 	rect.xmax = hWidth;
 	rect.ymin = -hHeight;
 	rect.ymax = hHeight;
 
 	sm::vec2 offset = Math2D::RotateVector(center, m_core->Angle());
 	rect.Translate(m_core->Position() + offset);
 	m_bounding->InitFromRect(rect);
 	m_bounding->SetTransform(m_core->Position(), m_offset, m_core->Angle());
}

}