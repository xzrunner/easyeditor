#include "Sprite.h"
#include "Symbol.h"

#include <gum/SkeletonIO.h>
#include <s2loader/SymbolFile.h>

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

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::SkeletonSprite(sym)
	, ee::Sprite(sym)
{
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	std::string key = s2loader::SymbolFile::Instance()->Tag(s2::SYM_SKELETON).c_str();
	gum::SkeletonIO::Load(val[key], m_pose);
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	std::string key = s2loader::SymbolFile::Instance()->Tag(s2::SYM_SKELETON).c_str();
	gum::SkeletonIO::Store(val[key], m_pose);
}

}