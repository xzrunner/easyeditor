#pragma once

#include <ee/Sprite.h>

#include <sprite2/ModelSprite.h>

namespace ee3
{

class NodeSprite : public s2::ModelSprite, public ee::Sprite
{
public:
	NodeSprite(const NodeSprite& spr);
	NodeSprite& operator = (const NodeSprite& spr);
	NodeSprite(const s2::SymPtr& sym, uint32_t id = -1);

	/**
	*  @interface
	*    ee::Sprite
	*/
	virtual void Load(const Json::Value& val, const std::string& dir = "") override;
	virtual void Store(Json::Value& val, const std::string& dir = "") const override;

	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage) override;

	static ee::SprPtr Create(const std::shared_ptr<ee::Symbol>& sym);

	S2_SPR_CLONE_FUNC(NodeSprite)

}; // NodeSprite

}