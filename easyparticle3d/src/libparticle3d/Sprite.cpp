#include "Sprite.h"
#include "ParticleSystem.h"

namespace eparticle3d
{

Sprite::Sprite()
	: m_symbol(NULL)
{
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();
	BuildBounding();	
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

bool Sprite::Update(int version) 
{
	m_symbol->GetPS()->SetDirection(m_dir);
	return true; 
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	Symbol* particle3d = dynamic_cast<Symbol*>(symbol);
	if (m_symbol != symbol && particle3d)
	{
		m_symbol->Release();
		symbol->Retain();

		m_symbol = particle3d;
	}
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);

//	m_dir.CreateFromVectors()
}

void Sprite::Store(Json::Value& val) const
{
	ISprite::Store(val);

	vec4 dir = m_dir.ToVector();
	Json::Value vdir;
	vdir["x"] = dir.x;
	vdir["y"] = dir.y;
	vdir["z"] = dir.z;
	vdir["w"] = dir.w;
	val["particle3d"]["dir"] = vdir;
}

}