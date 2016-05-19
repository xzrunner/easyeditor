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

void LoadFromJson::Load(const Json::Value& value, const std::string& dir, Symbol* complex)
{
	complex->name = value["name"].asString();
	complex->tag = value["tag"].asString();

	complex->m_clipbox.xmin = static_cast<float>(value["xmin"].asInt());
	complex->m_clipbox.xmax = static_cast<float>(value["xmax"].asInt());
	complex->m_clipbox.ymin = static_cast<float>(value["ymin"].asInt());
	complex->m_clipbox.ymax = static_cast<float>(value["ymax"].asInt());

	complex->m_use_render_cache = value["use_render_cache"].asBool();

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spriteValue);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!symbol) {
			std::string filepath = spriteValue["filepath"].asString();
			throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", dir.c_str(), filepath.c_str());
		}
		ee::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spriteValue);

		//		symbol->refresh();
		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spriteValue, dir);

		symbol->Release();

		static_cast<Symbol*>(complex)->Add(sprite);
#ifdef OPEN_SCREEN_CACHE
		ee::SpatialPartition::Instance()->Insert(sprite);
		ee::SpriteRenderer::InvalidRect(sprite);
#endif // OPEN_SCREEN_CACHE

		spriteValue = value["sprite"][i++];
	}	
}

}