#include "ImageSprite.h"
#include "IBody.h"
#include "BodyFactory.h"
#include "SpriteFactory.h"

#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

#include <Box2D/Box2D.h>

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
}

ImageSprite::ImageSprite(ImageSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

ImageSprite* ImageSprite::clone()
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
	if (image) m_symbol = image;
}

void ImageSprite::loadBodyFromFile()
{
	wxString path = FilenameTools::getFilePathExceptExtension(m_symbol->getFilepath());
	wxString polygonPath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_polyline) + ".txt";
	wxString circlePath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_circle) + ".txt";
	wxString shapePath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_shape) + ".json";
	if (FilenameTools::isExist(polygonPath))
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(polygonPath, m_xScale);
	}
	else if (FilenameTools::isExist(circlePath))
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(circlePath, m_xScale);
	}
	else if (FilenameTools::isExist(shapePath))
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(shapePath, m_xScale);
	}
	m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
}

void ImageSprite::buildBounding(float* texCoords)
{
	if (!m_symbol) return;

	const int width = m_symbol->getWidth(),
		height = m_symbol->getHeight();

	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	Rect rect;
	rect.xMin = width * (texCoords[0] - 0.5f);
	rect.xMax = width * (texCoords[2] - 0.5f);
	rect.yMin = height * (texCoords[1] - 0.5f);
	rect.yMax = height * (texCoords[3] - 0.5f);

	Vector center(rect.xCenter(), rect.yCenter());
	float hWidth = (rect.xMax - rect.xMin) * 0.5f * m_xScale,
		hHeight = (rect.yMax - rect.yMin) * 0.5f * m_yScale;
	rect.xMin = -hWidth;
	rect.xMax = hWidth;
	rect.yMin = -hHeight;
	rect.yMax = hHeight;

	Vector offset = Math::rotateVector(center, m_angle);
	rect.translate(m_pos + offset);
	m_bounding->initFromRect(rect);
	m_bounding->rotate(m_angle);
}

void ImageSprite::buildBounding()
{
	if (!m_symbol) return;

	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	//////////////////////////////////////////////////////////////////////////

// 	const float hWidth = m_symbol->getWidth() * 0.5f * m_xScale,
// 		hHeight = m_symbol->getHeight() * 0.5f * m_yScale;
// 
// 	Rect rect;
// 	rect.xMin = -hWidth;
// 	rect.xMax = hWidth;
// 	rect.yMin = -hHeight;
// 	rect.yMax = hHeight;
// 	rect.translate(m_pos);
// 	m_bounding->initFromRect(rect);
// 	m_bounding->rotate(m_angle);

	//////////////////////////////////////////////////////////////////////////

	Rect rect = m_symbol->getImage()->getRegion();
	rect.scale(m_xScale, m_yScale);
	rect.translate(m_pos);
	m_bounding->initFromRect(rect);
	m_bounding->rotate(m_angle);
}

} // d2d