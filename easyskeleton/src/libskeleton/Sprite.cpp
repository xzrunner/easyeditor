#include "Sprite.h"
#include "Symbol.h"

#include <gum/SkeletonIO.h>
#include <gum/SymbolFile.h>

#include <sprite2/SymType.h>

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

	std::string key = gum::SymbolFile::Instance()->Tag(s2::SYM_SKELETON);
	gum::SkeletonIO::Load(val[key], m_pose);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	std::string key = gum::SymbolFile::Instance()->Tag(s2::SYM_SKELETON);
	gum::SkeletonIO::Store(val[key], m_pose);
}

}