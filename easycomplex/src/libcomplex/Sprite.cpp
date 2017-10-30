#include "Sprite.h"
#include "Symbol.h"
#include "SpritePropertySetting.h"
#include "LoadFromJson.h"

#include <ee/SpriteFactory.h>
#include <ee/FileHelper.h>
#include <ee/SymbolType.h>
#include <ee/Exception.h>
#include <ee/LogMgr.h>
#include <ee/SymbolPath.h>

#include <sprite2/RenderParams.h>

namespace ecomplex
{

Sprite::Sprite(const Sprite& spr)
	: s2::Sprite(spr)
	, s2::ComplexSprite(spr)
	, ee::Sprite(spr)
{
}

Sprite& Sprite::operator = (const Sprite& spr)
{
	s2::Sprite::operator = (spr);
	s2::ComplexSprite::operator = (spr);
	ee::Sprite::operator = (spr);
	return *this;
}

Sprite::Sprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, s2::ComplexSprite(sym)
	, ee::Sprite(sym)
{
}

void Sprite::Load(const Json::Value& val, const std::string& dir)
{
	ee::Sprite::Load(val);

	if (val["complex"].isNull()) {
		return;
	}	

	const Json::Value& comp_val = val["complex"];
	
	m_action = comp_val["action"].asInt();

	if (val.isMember(ee::SYM_GROUP_TAG))
	{
		std::string filepath = val["filepath"].asString();
		std::string group_dir = ee::FileHelper::GetFileDir(filepath);
		std::string _dir = ee::FileHelper::GetAbsolutePath(dir, group_dir);

		auto sym = std::dynamic_pointer_cast<s2::ComplexSymbol>(m_sym);
		std::dynamic_pointer_cast<ee::Symbol>(m_sym)->SetFilepath(ee::SYM_GROUP_TAG);
		for (int i = 0, n = val[ee::SYM_GROUP_TAG].size(); i < n; ++i) 
		{
			ee::SprPtr spr = nullptr;
			try {
				spr = ee::SpriteFactory::Instance()->Create(val[ee::SYM_GROUP_TAG][i], _dir);
			} catch (ee::Exception& e) {
				ee::LogMgr::Instance()->AddException(e.What());
				continue;
			}
			if (spr) {
				sym->Add(spr);
			}
		}
		LoadFromJson::CreateActionsFromTag(*std::dynamic_pointer_cast<Symbol>(sym));
	}
}

void Sprite::Store(Json::Value& val, const std::string& dir) const
{
	ee::Sprite::Store(val);

	Json::Value comp_val;

	comp_val["action"] = m_action;

	val["complex"] = comp_val;

	auto ee_sym = std::dynamic_pointer_cast<ee::Symbol>(m_sym);
	if (ee_sym->GetFilepath() == ee::SYM_GROUP_TAG)
	{
		auto sym = std::dynamic_pointer_cast<s2::ComplexSymbol>(m_sym);
		assert(sym);
		auto& children = sym->GetAllChildren();
		int count = 0;
		for (int i = 0, n = children.size(); i < n; ++i) 
		{
			Json::Value cval;
			auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
			val[ee::SYM_GROUP_TAG][i]["filepath"] = 
				ee::SymbolPath::GetRelativePath(*std::dynamic_pointer_cast<ee::Symbol>(child->GetSymbol()), dir);
			child->Store(val[ee::SYM_GROUP_TAG][i], dir);
		}
	}
}

ee::PropertySetting* Sprite::CreatePropertySetting(ee::EditPanelImpl* stage)
{
	return new SpritePropertySetting(stage, std::dynamic_pointer_cast<Sprite>(shared_from_this()));
}

void Sprite::SetAction(int idx)
{
	m_action = idx;
	UpdateBounding();
}

ee::SprPtr Sprite::Create(const std::shared_ptr<ee::Symbol>& sym) 
{
	return std::make_shared<Sprite>(sym);
}

}