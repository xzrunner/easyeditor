#include "LoadFromJson.h"
#include "Symbol.h"

#include <ee/dev_config.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>
#include <ee/Sprite.h>

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
}

}