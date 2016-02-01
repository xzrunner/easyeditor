#include "Layer.h"
#include "Symbol.h"
#include "SymbolSearcher.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"
#include "StringHelper.h"
#include "FileHelper.h"
#include "Exception.h"
#include "FetchAllVisitor.h"

namespace ee
{

Layer::Layer()
	: visible(true)
	, editable(true)
{
	static int count = 0;
	name = std::string("layer") + StringHelper::ToString(count++);
}

Layer::~Layer()
{
	Clear();
}

void Layer::TraverseSprite(Visitor& visitor, DataTraverseType type, bool order) const
{
	if (type == DT_EDITABLE && editable ||
		type == DT_VISIBLE && visible ||
		type == DT_ALL || type == DT_SELECTABLE)
	{
		m_sprites.Traverse(visitor, type, order);
	}
}

bool Layer::Insert(Sprite* sprite)
{
	if (m_sprites.IsExist(sprite)) {
		return false;
	} else {
		return m_sprites.Insert(sprite);
	}
}

bool Layer::Remove(Sprite* sprite)
{
	return m_sprites.Remove(sprite);
}

void Layer::TraverseShape(Visitor& visitor, bool order) const
{
	m_shapes.Traverse(visitor, order);
}

bool Layer::Insert(Shape* shape)
{
	if (m_shapes.IsExist(shape)) {
		return false;
	} else {
		return m_shapes.Insert(shape);
	}
}

bool Layer::Remove(Shape* shape)
{
	return m_shapes.Remove(shape);
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
		Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		if (!symbol) {
			std::string filepath = spr_val["filepath"].asString();
			throw Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", dir.c_str(), filepath.c_str());
		}
		SymbolSearcher::SetSymbolFilepaths(dir, symbol, spr_val);

		Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);
		m_sprites.Insert(sprite);

		symbol->Release();

		spr_val = val["sprite"][i++];
	}

// 	i = 0;
// 	Json::Value shape_val = val["shape"][i++];
// 	while (!shape_val.isNull()) {
// 		Shape* shape = eshape::ShapeFactory::CreateShapeFromFile(shape_val, dir);
// 		m_shapes.Insert(shape);
// 		shape->Release();
// 
// 		shape_val = val["shape"][i++];
// 	}
}

void Layer::StoreToFile(Json::Value& val, const std::string& dir) const
{
	val["name"] = name;
	val["visible"] = visible;
	val["editable"] = editable;

	std::vector<Sprite*> sprites;
	m_sprites.Traverse(FetchAllVisitor<Sprite>(sprites), true);
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Sprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = FileHelper::GetRelativePath(dir,
			spr->GetSymbol().GetFilepath());
		spr->Store(spr_val);

		val["sprite"][i] = spr_val;
	}

	std::vector<Shape*> shapes;
	m_shapes.Traverse(FetchAllVisitor<Shape>(shapes), true);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		Shape* shape = shapes[i];
		Json::Value shape_val;
		shape->StoreToFile(shape_val, dir);
		val["shape"][i] = shape_val;
	}
}

void Layer::Clear()
{
	m_sprites.Clear();
	m_shapes.Clear();
}

}