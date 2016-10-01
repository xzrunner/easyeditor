#include "Sprite.h"
#include "Symbol.h"

#include <gum/SkeletonIO.h>

namespace libskeleton
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::SkeletonSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::SkeletonSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(Symbol* sym)
	: s2::Sprite(sym)
	, s2::SkeletonSprite(sym)
	, ee::Sprite(sym)
{
}

ee::Sprite* Sprite::Create(ee::Symbol* sym) 
{
	return new Sprite(static_cast<Symbol*>(sym));
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);
	gum::SkeletonIO::Load(val["skeleton"], m_pose);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);
	gum::SkeletonIO::Store(val["skeleton"], m_pose);
}

}