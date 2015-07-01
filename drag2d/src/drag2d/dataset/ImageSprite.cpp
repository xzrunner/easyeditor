#include "ImageSprite.h"
#include "SpriteFactory.h"

#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

namespace d2d
{

ImageSprite::ImageSprite()
	: m_symbol(NULL)
{
}

ImageSprite::ImageSprite(const ImageSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();
}

ImageSprite::ImageSprite(ImageSymbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	buildBounding();
}

ImageSprite::~ImageSprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

ImageSprite* ImageSprite::clone() const
{
	ImageSprite* sprite = new ImageSprite(*this);
	SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const ImageSymbol& ImageSprite::getSymbol() const
{
	return *m_symbol;
}

void ImageSprite::setSymbol(ISymbol* symbol)
{
	ImageSymbol* image = dynamic_cast<ImageSymbol*>(symbol);
	if (m_symbol != symbol && image)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = image;
	}
}

void ImageSprite::buildBoundingFromTexCoords(float* texCoords)
{
	if (!m_symbol) return;

 	const int width = m_symbol->getSize().xLength(),
 		height = m_symbol->getSize().yLength();
 
 	delete m_bounding;
 	m_bounding = BVFactory::createBV(e_obb);
 
 	Rect rect;
 	rect.xMin = width * (texCoords[0] - 0.5f);
 	rect.xMax = width * (texCoords[2] - 0.5f);
 	rect.yMin = height * (texCoords[1] - 0.5f);
 	rect.yMax = height * (texCoords[3] - 0.5f);
 
 	Vector center(rect.xCenter(), rect.yCenter());
 	float hWidth = (rect.xMax - rect.xMin) * 0.5f * m_scale.x,
 		hHeight = (rect.yMax - rect.yMin) * 0.5f * m_scale.y;
 	rect.xMin = -hWidth;
 	rect.xMax = hWidth;
 	rect.yMin = -hHeight;
 	rect.yMax = hHeight;
 
 	Vector offset = Math::rotateVector(center, m_angle);
 	rect.translate(m_pos + offset);
 	m_bounding->initFromRect(rect);
 	m_bounding->SetTransform(m_pos, m_offset, m_angle);
}

} // d2d