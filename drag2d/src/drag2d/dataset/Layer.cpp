#include "Layer.h"
#include "ISymbol.h"
#include "SymbolSearcher.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/StringTools.h"
#include "common/visitors.h"
#include "common/FileNameTools.h"
#include "common/Exception.h"

namespace d2d
{

Layer::Layer()
	: visible(true)
	, editable(true)
{
	static int count = 0;
	name = std::string("layer") + StringTools::ToString(count++);
}

Layer::~Layer()
{
	Clear();
}

void Layer::TraverseSprite(IVisitor& visitor, DataTraverseType type, bool order) const
{
	if (type == DT_EDITABLE && editable ||
		type == DT_VISIBLE && visible ||
		type == DT_ALL || type == DT_SELECTABLE)
	{
		m_sprites.Traverse(visitor, type, order);
	}
}

bool Layer::Insert(ISprite* sprite)
{
	if (m_sprites.IsExist(sprite)) {
		return false;
	} else {
// 		sprite->visiable = m_visible;
// 		sprite->editable = m_editable;
		m_sprites.Insert(sprite);
		return true;
	}
}

bool Layer::Remove(ISprite* sprite)
{
	return m_sprites.Remove(sprite);
}

void Layer::LoadFromFile(const Json::Value& val, const std::string& dir)
{
	name = val["name"].asString();
	visible = val["visible"].asBool();
	editable = val["editable"].asBool();

	int i = 0;
	Json::Value spr_val = val["sprite"][i++];
	while (!spr_val.isNull()) {
		std::string filepath = SymbolSearcher::GetSymbolPath(dir, spr_val);
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!symbol) {
			std::string filepath = spr_val["filepath"].asString();
			throw Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", dir.c_str(), filepath.c_str());
		}
		SymbolSearcher::SetSymbolFilepaths(dir, symbol, spr_val);

		//		symbol->refresh();
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprite->Load(spr_val);
		m_sprites.Insert(sprite);

		symbol->Release();

		spr_val = val["sprite"][i++];
	}	
}

void Layer::StoreToFile(Json::Value& val, const std::string& dir) const
{
	val["name"] = name;
	val["visible"] = visible;
	val["editable"] = editable;

	std::vector<ISprite*> sprites;
	m_sprites.Traverse(FetchAllVisitor<ISprite>(sprites), true);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = FilenameTools::getRelativePath(dir,
			spr->GetSymbol().GetFilepath()).ToStdString();
		spr->Store(spr_val);

		val["sprite"][i] = spr_val;
	}
}

void Layer::Clear()
{
	m_sprites.Clear();
	m_shapes.Clear();
}

} // d2d