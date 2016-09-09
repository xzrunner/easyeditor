#include "LoadFromJson.h"
#include "Symbol.h"

#include <ee/dev_config.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>
#include <ee/StringHelper.h>

#include <glue/ComplexSymLoader.h>

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

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spriteValue);
		ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!sym) {
			std::string filepath = spriteValue["filepath"].asString();
			throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s, [src]:%s", dir.c_str(), filepath.c_str(), _filepath.c_str());
		}
		ee::SymbolSearcher::SetSymbolFilepaths(dir, sym, spriteValue);

		//		sym->refresh();
		ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
		spr->Load(spriteValue, dir);

		sym->RemoveReference();

		static_cast<Symbol*>(complex)->Add(spr);
		spr->RemoveReference();
#ifdef OPEN_SCREEN_CACHE
		ee::SpatialPartition::Instance()->Insert(spr);
		ee::SpriteRenderer::InvalidRect(spr);
#endif // OPEN_SCREEN_CACHE

		spriteValue = value["sprite"][i++];
	}	

	InitActions(complex, value);
}

void LoadFromJson::InitActions(Symbol* sym, const Json::Value& val)
{
	if (val.isMember("action")) {
		glue::ComplexSymLoader::LoadJsonAction(val, sym);	
	} else {
		CreateActionsFromTag(sym);
	}
}

void LoadFromJson::CreateActionsFromTag(Symbol* sym)
{
	std::vector<s2::ComplexSymbol::Action> actions;

	const std::vector<s2::Sprite*>& children = sym->GetChildren();
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

			bool find = false;
			for (int k = 0; k < actions.size(); ++k) {
				if (actions[k].name == tag) {
					find = true;
					actions[k].sprs.push_back(spr);
				}
			}

			if (!find) {
				s2::ComplexSymbol::Action a;
				a.name = tag;
				a.sprs.push_back(spr);
				actions.push_back(a);
			}
		}
	}

	sym->SetActions(actions);
}

}