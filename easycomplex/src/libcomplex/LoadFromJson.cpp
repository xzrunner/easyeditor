#include <common/dev_config.h>

#include "LoadFromJson.h"
#include "Symbol.h"

namespace ecomplex
{

void LoadFromJson::Load(const Json::Value& value, const std::string& dir, Symbol* complex)
{
	complex->name = value["name"].asString();
	complex->tag = value["tag"].asString();

	complex->m_clipbox.xMin = value["xmin"].asInt();
	complex->m_clipbox.xMax = value["xmax"].asInt();
	complex->m_clipbox.yMin = value["ymin"].asInt();
	complex->m_clipbox.yMax = value["ymax"].asInt();

	complex->m_use_render_cache = value["use_render_cache"].asBool();

	int i = 0;
	Json::Value spriteValue = value["sprite"][i++];
	while (!spriteValue.isNull()) {
		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spriteValue);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!symbol) {
			std::string filepath = spriteValue["filepath"].asString();
			throw d2d::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", dir.c_str(), filepath.c_str());
		}
		d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spriteValue);

		//		symbol->refresh();
		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Load(spriteValue);

		symbol->Release();

		static_cast<Symbol*>(complex)->m_sprites.push_back(sprite);
#ifdef OPEN_SCREEN_CACHE
		d2d::SpatialIndex::Instance()->Insert(sprite);
		d2d::SpriteRenderer::Instance()->InvalidRect(sprite);
#endif // OPEN_SCREEN_CACHE

		spriteValue = value["sprite"][i++];
	}	
}

}