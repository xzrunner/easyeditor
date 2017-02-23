#include "LoadFromJson.h"
#include "Symbol.h"

#include <ee/dev_config.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/StringHelper.h>
#include <ee/LogMgr.h>

#include <gum/ComplexSymLoader.h>

#include <assert.h>

namespace ecomplex
{

void LoadFromJson::Load(const std::string& _filepath, const Json::Value& value, 
						const std::string& dir, Symbol* complex)
{
	complex->name = value["name"].asString();
	complex->tag = value["tag"].asString();

	sm::rect scissor;
	scissor.xmin = static_cast<float>(value["xmin"].asInt());
	scissor.xmax = static_cast<float>(value["xmax"].asInt());
	scissor.ymin = static_cast<float>(value["ymin"].asInt());
	scissor.ymax = static_cast<float>(value["ymax"].asInt());
	complex->SetScissor(scissor);

	complex->m_use_render_cache = value["use_render_cache"].asBool();

	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		ee::Sprite* spr = NULL;
		try {
			spr = LoadSprite(value["sprite"][i], dir);
		} catch (ee::Exception& e) {
			ee::LogMgr::Instance()->AddException(e.What());
			continue;
		}
		if (spr) {
			complex->Add(spr);
			spr->RemoveReference();
		}
	}

	InitActions(complex, value);
}

void LoadFromJson::CreateActionsFromTag(Symbol* sym)
{
	std::map<std::string, std::vector<s2::Sprite*> > map_actions;

	const std::vector<s2::Sprite*>& children = sym->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i)
	{
 		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);
 		if (spr->GetTag().empty()) {
 			continue;			
 		}
 
 		std::vector<std::string> tags;
 		ee::StringHelper::Split(spr->GetTag(), ";", tags);
		for (int j = 0, m = tags.size(); j < m; ++j)
		{
 			const std::string& tag = tags[j];
 			if (tag.find("=") != std::string::npos) {
 				continue;
 			}
			if (map_actions.find(tag) == map_actions.end()) {
				map_actions.insert(std::make_pair(tag, std::vector<s2::Sprite*>()));
			}
		}
	}

	for (int i = 0, n = children.size(); i < n; ++i)
	{
		ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);

		bool inserted = false;
		if (!spr->GetTag().empty()) 
		{
			std::vector<std::string> tags;
			ee::StringHelper::Split(spr->GetTag(), ";", tags);
			for (int j = 0, m = tags.size(); j < m; ++j)
			{
				const std::string& tag = tags[j];
				if (tag.find("=") != std::string::npos) {
					continue;
				}

				std::map<std::string, std::vector<s2::Sprite*> >::iterator itr 
					= map_actions.find(tag);
				assert(itr != map_actions.end());
				itr->second.push_back(spr);
				inserted = true;
			}			
		}

		if (!inserted) {
			std::map<std::string, std::vector<s2::Sprite*> >::iterator itr 
				= map_actions.begin();
			for ( ; itr != map_actions.end(); ++itr) {
				itr->second.push_back(spr);
			}
		}
	}

	std::vector<s2::ComplexSymbol::Action> actions;
	actions.reserve(map_actions.size());
	std::map<std::string, std::vector<s2::Sprite*> >::iterator itr 
		= map_actions.begin();
	for ( ; itr != map_actions.end(); ++itr) 
	{
		s2::ComplexSymbol::Action action;
		action.name = itr->first;
		action.sprs = itr->second;
		actions.push_back(action);
	}
	sym->SetActions(actions);
}

ee::Sprite* LoadFromJson::LoadSprite(const Json::Value& val, const std::string& dir)
{
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(val, dir);
#ifdef OPEN_SCREEN_CACHE
	ee::SpatialPartition::Instance()->Insert(spr);
	ee::SpriteRenderer::InvalidRect(spr);
#endif // OPEN_SCREEN_CACHE
	return spr;
}

void LoadFromJson::InitActions(Symbol* sym, const Json::Value& val)
{
// 	if (val.isMember("action")) {
// 		gum::ComplexSymLoader::LoadJsonAction(val, sym);	
// 	} else {
		CreateActionsFromTag(sym);
//	}
}

}