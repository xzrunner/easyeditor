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

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	eparticle3d::Symbol* sym = dynamic_cast<eparticle3d::Symbol*>(symbol);
	if (!sym) {
		return;
	}
	d2d::obj_assign<eparticle3d::Symbol>(m_symbol, sym);
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);

	const Json::Value& vdir = val["particle3d"]["dir"];
	float x = vdir["x"].asDouble(),
		y = vdir["y"].asDouble(),
		z = vdir["z"].asDouble(),
		w = vdir["w"].asDouble();
	m_dir = Quaternion(x, y, z, w);
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