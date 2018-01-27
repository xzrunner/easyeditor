#include "ee3/NodeSprite.h"
#include "ee3/NodeSymbol.h"
//#include "PropertySetting2.h"

namespace ee3
{

NodeSprite::NodeSprite(const NodeSprite& spr)
	: s2::Sprite(spr)
	, s2::ModelSprite(spr)
	, ee::Sprite(spr)
{
}

NodeSprite& NodeSprite::operator = (const NodeSprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::ModelSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

NodeSprite::NodeSprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::ModelSprite(sym)
	, ee::Sprite(sym)
{
}

void NodeSprite::Load(const Json::Value& val, const std::string& dir)
{

}

void NodeSprite::Store(Json::Value& val, const std::string& dir) const
{

}

ee::PropertySetting* NodeSprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
//	return new PropertySetting2(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
	return nullptr;
}

ee::SprPtr NodeSprite::Create(const std::shared_ptr<ee::Symbol>& sym)
{
	return std::make_shared<NodeSprite>(sym);
}

}