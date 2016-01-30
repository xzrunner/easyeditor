#include "ImageSprite.h"
#include "SpriteFactory.h"
#include "FileHelper.h"
#include "FileType.h"
#include "Math2D.h"
#include "BBFactory.h"
#include "BoundingBox.h"

namespace ee
{

ImageSprite::ImageSprite()
	: m_symbol(NULL)
{
}

ImageSprite::ImageSprite(const ImageSprite& sprite)
	: Sprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();
}

ImageSprite::ImageSprite(ImageSymbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	BuildBounding();
}

ImageSprite::~ImageSprite()
{
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

 	const int width = m_symbol->GetSize().xLength(),
 		height = m_symbol->GetSize().yLength();
 
 	delete m_bounding;
 	m_bounding = BBFactory::CreateBB(e_obb);
 
 	Rect rect;
 	rect.xmin = width * (texCoords[0] - 0.5f);
 	rect.xmax = width * (texCoords[2] - 0.5f);
 	rect.ymin = height * (texCoords[1] - 0.5f);
 	rect.ymax = height * (texCoords[3] - 0.5f);
 
 	Vector center(rect.CenterX(), rect.CenterY());
 	float hWidth = (rect.xmax - rect.xmin) * 0.5f * m_scale.x,
 		hHeight = (rect.ymax - rect.ymin) * 0.5f * m_scale.y;
 	rect.xmin = -hWidth;
 	rect.xmax = hWidth;
 	rect.ymin = -hHeight;
 	rect.ymax = hHeight;
 
 	Vector offset = Math2D::RotateVector(center, m_angle);
 	rect.translate(m_pos + offset);
 	m_bounding->InitFromRect(rect);
 	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

}