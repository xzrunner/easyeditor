#include "Layer.h"

namespace lr
{

Layer::Layer()
	: m_editable(true)
	, m_visible(true)
{	
}

Layer::~Layer()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

void Layer::TraverseSprite(d2d::IVisitor& visitor, bool order/* = true*/) const
{
	d2d::VectorContainer::Traverse(m_sprites, visitor, order);
}

void Layer::TraverseSprite(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	d2d::VectorContainer::Traverse(m_sprites, visitor, type, order);
}

bool Layer::RemoveSprite(Object* obj)
{
	return d2d::VectorContainer::Remove(m_sprites, static_cast<d2d::ISprite*>(obj));
}

void Layer::InsertSprite(Object* obj)
{
	d2d::VectorContainer::Insert(m_sprites, static_cast<d2d::ISprite*>(obj));
}

void Layer::ClearSprite()
{
	d2d::VectorContainer::Clear(m_sprites);
}

bool Layer::ResetOrderSprite(const Object* obj, bool up)
{
	return d2d::VectorContainer::ResetOrder(m_sprites, static_cast<const d2d::ISprite*>(obj), up);
}

void Layer::TraverseShape(d2d::IVisitor& visitor, bool order) const
{
	d2d::VectorContainer::Traverse(m_shapes, visitor, order);
}

bool Layer::RemoveShape(Object* obj)
{
	return d2d::VectorContainer::Remove(m_shapes, static_cast<d2d::IShape*>(obj));
}

void Layer::InsertShape(Object* obj)
{
	d2d::VectorContainer::Insert(m_shapes, static_cast<d2d::IShape*>(obj));
}

void Layer::ClearShape()
{
	d2d::VectorContainer::Clear(m_shapes);
}

void Layer::LoadFromFile(const Json::Value& val, const std::string& dir)
{
	m_name = val["name"].asString();
	m_editable = val["editable"].asBool();
	m_visible = val["visible"].asBool();

	int i = 0;
	Json::Value spr_val = val["sprite"][i++];
	while (!spr_val.isNull()) {
		wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::ISymbol* symbol = NULL;
		try {
			wxString shape_tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
			wxString shape_filepath = d2d::FilenameTools::getFilenameAddTag(filepath, shape_tag, "json");
			if (d2d::FilenameTools::isExist(shape_filepath)) {
				symbol = d2d::SymbolMgr::Instance()->fetchSymbol(shape_filepath);
			} else {
				symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			}

			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->load(spr_val);

			sprite->Retain();
			m_sprites.push_back(sprite);

			sprite->Release();
			symbol->Release();
		} catch (d2d::Exception& e) {
			std::cout << "Symbol::loadResources error! File:" << filepath << std::endl;
			std::cout << e.what();
		}

		spr_val = val["sprite"][i++];
	}
}

void Layer::StoreToFile(Json::Value& val, const std::string& dir) const
{
	val["name"] = m_name;
	val["visible"] = m_visible;
	val["editable"] = m_editable;

	for (int i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		d2d::ISprite* spr = m_sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FilenameTools::getRelativePath(dir,
			spr->getSymbol().getFilepath()).ToStdString();
		spr->store(spr_val);

		val["sprite"][i] = spr_val;
	}
}

}