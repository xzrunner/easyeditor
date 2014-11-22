#include "Sprite.h"
#include "FileIO.h"
#include "Shape.h"

namespace emesh
{

Sprite::Sprite()
	: m_symbol(NULL)
{
//	m_speed.set(0, -0.01f);
}

Sprite::Sprite(const Sprite& s)
	: d2d::ISprite(s)
	, m_speed(s.m_speed)
{
	m_symbol = s.m_symbol->clone();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
//	m_speed.set(0, -0.01f);

	m_symbol->Retain();
	buildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::clone() const
{
	return new Sprite(*this);
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* mesh = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != mesh)
	{
		if (m_symbol) {
			m_symbol->Release();
		}
		symbol->Retain();

		m_symbol = mesh;
		buildBounding();
	}
}

void Sprite::loadBodyFromFile()
{
}

void Sprite::load(const Json::Value& val)
{
	ISprite::load(val);
	m_speed.x = val["speed"]["x"].asDouble();
	m_speed.y = val["speed"]["y"].asDouble();

	// ¼æÈÝÀÏÊý¾Ý
	if (!val["left nodes"].isNull()) {
		m_symbol->getShape()->Load(val);
	}
}

void Sprite::store(Json::Value& val) const
{
	ISprite::store(val);
	val["speed"]["x"] = m_speed.x;
	val["speed"]["y"] = m_speed.y;

	m_symbol->getShape()->Store(val);
}

void Sprite::buildBounding()
{
	if (!m_bounding) 
		m_bounding = d2d::BVFactory::createBV(d2d::e_obb);
	const d2d::ISymbol& symbol = getSymbol();

	d2d::Rect rect = m_symbol->getSize();
	if (m_offset.x == 0 && m_offset.y == 0)
		m_offset.set(rect.xCenter(), rect.yCenter());
	rect.scale(m_scale.x, m_scale.y);
	rect.shear(m_shear.x, m_shear.y);
	m_bounding->initFromRect(rect);
	m_bounding->setTransform(m_pos, m_offset, m_angle);
}

void Sprite::SetTween(Sprite* begin, Sprite* end, float process)
{
	getShape()->SetTween(begin->getShape(), end->getShape(), process);
}

}