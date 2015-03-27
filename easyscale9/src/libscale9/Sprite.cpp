#include "Sprite.h"

namespace escale9
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_width = m_height = 0;

	memset(m_sprites, 0, sizeof(void*) * 9);
}

Sprite::Sprite(const Sprite& sprite)
	: d2d::ISprite(sprite)
	, m_symbol(sprite.m_symbol)
	, m_width(sprite.m_width)
	, m_height(sprite.m_height)
{
	m_symbol->Retain();

	LoadFromSymbol(sprite.m_symbol);
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	m_width = m_symbol->getSize().xLength();
	m_height = m_symbol->getSize().yLength();

	LoadFromSymbol(symbol);

	buildBounding();
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			m_sprites[i][j]->Release();
		}
	}
}

Sprite* Sprite::clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::getSymbol() const
{
	return *m_symbol;
}

void Sprite::setSymbol(d2d::ISymbol* symbol)
{
	Symbol* scale9 = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && scale9)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = scale9;
	}
}

void Sprite::load(const Json::Value& val)
{
	ISprite::load(val);

	if (val["scale9"].isNull()) {
		return;
	}

	float w = val["scale9"]["width"].asDouble();
	float h = val["scale9"]["height"].asDouble();
	SetSize(w, h);
}

void Sprite::store(Json::Value& val) const
{
	ISprite::store(val);

	val["scale9"]["width"] = m_width;
	val["scale9"]["height"] = m_height;
}

void Sprite::GetSize(float& w, float& h) const
{
	w = m_width;
	h = m_height;
}

void Sprite::SetSize(float w, float h)
{
	if (m_width == w && m_height == h) {
		return;
	}

	m_width = w;
	m_height = h;

	ResizeScale9(m_symbol->m_type, m_sprites, m_width, m_height);

	buildBounding();
}

void Sprite::Draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans) const
{
	Symbol::DrawScale9(m_symbol->m_type, m_sprites, mt, mul, add, r_trans, g_trans, b_trans);
}

void Sprite::LoadFromSymbol(Symbol* symbol)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			d2d::ISprite* src = symbol->m_sprites[i][j];
			if (src) {
				m_sprites[i][j] = src->clone();
			} else {
				m_sprites[i][j] = NULL;
			}
		}
	}

	ResizeScale9(m_symbol->m_type, m_sprites, m_width, m_height);
}

void Sprite::ResizeScale9(Symbol::Type type, d2d::ISprite* const sprites[3][3],
						  float width, float height)
{
	if (type == Symbol::e_9Grid) 
	{
		const float w0 = sprites[0][0]->getSymbol().getSize().xLength(),
			w2 = sprites[0][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2;
		const float h0 = sprites[0][0]->getSymbol().getSize().yLength(),
			h2 = sprites[0][2]->getSymbol().getSize().yLength(),
			h1 = height - h0 - h2;

		ResizeSprite(sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		ResizeSprite(sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		ResizeSprite(sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);		
	}
	else if (type == Symbol::e_9GridHollow)
	{
		const float w0 = sprites[0][0]->getSymbol().getSize().xLength(),
			w2 = sprites[0][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2;
		const float h0 = sprites[0][0]->getSymbol().getSize().yLength(),
			h2 = sprites[0][2]->getSymbol().getSize().yLength(),
			h1 = height - h0 - h2;

		ResizeSprite(sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		ResizeSprite(sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		ResizeSprite(sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (type == Symbol::e_6GridUpper)
	{
		const float w0 = sprites[2][0]->getSymbol().getSize().xLength(),
			w2 = sprites[2][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2;
		const float h2 = sprites[2][0]->getSymbol().getSize().yLength(),
			h1 = height - h2;

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (type == Symbol::e_3GridHor)
	{
		const float w0 = sprites[1][0]->getSymbol().getSize().xLength(),
			w2 = sprites[1][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2; 

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, height);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, height);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, height);
	}
	else if (type == Symbol::e_3GridVer)
	{
		const float h0 = sprites[0][1]->getSymbol().getSize().yLength(),
			h2 = sprites[2][1]->getSymbol().getSize().yLength(),
			h1 = height - h0 - h2;

		ResizeSprite(sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), width, h0);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), width, h1);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), width, h2);
	}	
}

void Sprite::ResizeSprite(d2d::ISprite* sprite, const d2d::Vector& center, 
						  float width, float height)
{
	const d2d::ImageSymbol& symbol = dynamic_cast<const d2d::ImageSymbol&>(sprite->getSymbol());
	int w = symbol.getImage()->originWidth(),
		h = symbol.getImage()->originHeight();
	assert(w != 0 && h != 0);

	sprite->setTransform(center, sprite->getAngle());
	const float times = sprite->getAngle() / d2d::PI;
	if (times - (int)(times + 0.01f) < 0.3f)
		sprite->setScale(width / w, height / h);
	else
		sprite->setScale(height / w, width / h);

	sprite->translate(d2d::Math::rotateVector(sprite->getOffset(), sprite->getAngle()) - sprite->getOffset());
}

}