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
#include <ee/SymbolSearcher.h>

#include <s2loader/ComplexSymLoader.h>

#include <assert.h>

namespace ecomplex
{

void LoadFromJson::Load(const std::string& _filepath, const Json::Value& value, 
						const std::string& dir, Symbol& complex)
{
	complex.name = value["name"].asString();
	complex.tag = value["tag"].asString();

	sm::rect scissor;
	scissor.xmin = static_cast<float>(value["xmin"].asInt());
	scissor.xmax = static_cast<float>(value["xmax"].asInt());
	scissor.ymin = static_cast<float>(value["ymin"].asInt());
	scissor.ymax = static_cast<float>(value["ymax"].asInt());
	complex.SetScissor(scissor);

	complex.m_use_render_cache = value["use_render_cache"].asBool();

	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		ee::SprPtr spr = nullptr;
		try {
			spr = LoadSprite(value["sprite"][i], dir);
		} catch (ee::Exception& e) {
			ee::LogMgr::Instance()->AddException(e.What());
			continue;
		}
		if (spr) {
			complex.Add(spr);
		}
	}

	InitActions(complex, value);
}

void LoadFromJson::LoadChildren(const Json::Value& value, const std::string& dir,
								std::vector<std::string>& children)
{
	children.reserve(value["sprite"].size());
	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, value["sprite"][i]);
		children.push_back(filepath);
	}
}

void LoadFromJson::CreateActionsFromTag(Symbol& sym)
{
	std::map<std::string, CU_VEC<s2::SprPtr>> map_actions;

	auto& children = sym.GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i)
	{
 		auto spr = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
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
				map_actions.insert(std::make_pair(tag, CU_VEC<s2::SprPtr>()));
			}
		}
	}

	for (int i = 0, n = children.size(); i < n; ++i)
	{
		auto spr = std::dynamic_pointer_cast<ee::Sprite>(children[i]);

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

				auto itr = map_actions.find(tag);
				assert(itr != map_actions.end());
				itr->second.push_back(spr);
				inserted = true;
			}			
		}

		if (!inserted) {
			auto itr = map_actions.begin();
			for ( ; itr != map_actions.end(); ++itr) {
				itr->second.push_back(spr);
			}
		}
	}

	CU_VEC<s2::ComplexSymbol::Action> actions;
	actions.reserve(map_actions.size());
	auto itr = map_actions.begin();
	for ( ; itr != map_actions.end(); ++itr) 
	{
		s2::ComplexSymbol::Action action;
		action.name = itr->first.c_str();
		action.sprs = itr->second;
		actions.push_back(action);
	}
	sym.SetActions(actions);
}

ee::SprPtr LoadFromJson::LoadSprite(const Json::Value& val, const std::string& dir)
{
	auto spr = ee::SpriteFactory::Instance()->Create(val, dir);
#ifdef OPEN_SCREEN_CACHE
	ee::SpatialPartition::Instance()->Insert(spr);
	ee::SpriteRenderer::InvalidRect(spr);
#endif // OPEN_SCREEN_CACHE
	return spr;
}

void LoadFromJson::InitActions(Symbol& sym, const Json::Value& val)
{
// 	if (val.isMember("action")) {
// 		s2loader::ComplexSymLoader::LoadJsonAction(val, sym);	
// 	} else {
		CreateActionsFromTag(sym);
//	}
}

}