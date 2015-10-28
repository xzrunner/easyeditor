#include "UIList.h"

#include <easycomplex.h>

namespace eui
{
namespace list
{

UIList::UIList()
{
	m_clipbox.xMin = m_clipbox.yMin = -200;
	m_clipbox.xMax = m_clipbox.yMax =  200;

	m_vertical = m_horizontal = false;
	m_hori_count = m_vert_count = 0;
	m_hori_space = m_vert_space = 0;

	m_base_spr = m_hori_spr = m_vert_spr = NULL;
}

void UIList::ClearExceptBase()
{
	d2d::ISprite* spr = m_base_spr;

	ClearAllSprite();

	if (!spr) {
		return;
	}

	spr->Retain();
	InsertSprite(spr);
	spr->Release();
}

bool UIList::ReorderSprite(d2d::ISprite* sprite, bool up)
{
	return m_items.ResetOrder(sprite, up);
}

bool UIList::InsertSprite(d2d::ISprite* sprite, int idx)
{
	if (m_base_spr && m_hori_spr && m_vert_spr) {
		return false;
	}

	if (!m_base_spr) {
		m_base_spr = sprite;
		sprite->Retain();
		m_items.Insert(sprite);
		return false;
	}

	if (&m_base_spr->GetSymbol() != &sprite->GetSymbol()) {
		return false;
	}

	d2d::Vector base_pos = m_base_spr->GetPosition();
	d2d::Vector new_pos = sprite->GetPosition();
	if (!m_hori_spr && !m_vert_spr) {
		if (base_pos == new_pos) {
			return false;
		}
		float dx = fabs(base_pos.x - new_pos.x);
		float dy = fabs(base_pos.y - new_pos.y);
		if (dx > dy) {
			m_hori_spr = sprite;
			m_hori_spr->SetTransform(d2d::Vector(new_pos.x, base_pos.y), sprite->GetAngle());
		} else {
			m_vert_spr = sprite;
			m_vert_spr->SetTransform(d2d::Vector(base_pos.x, new_pos.y), sprite->GetAngle());
		}
		sprite->Retain();
		m_items.Insert(sprite);
	} else if (!m_hori_spr) {
		if (base_pos.y == new_pos.y) {
			return false;
		} 
		m_hori_spr = sprite;
		sprite->Retain();
		m_items.Insert(sprite);
		m_hori_spr->SetTransform(d2d::Vector(new_pos.x, base_pos.y), sprite->GetAngle());
	} else {
		assert(!m_vert_spr);
		if (base_pos.x == new_pos.x) {
			return false;
		} 
		m_vert_spr = sprite;
		sprite->Retain();
		m_items.Insert(sprite);
		m_vert_spr->SetTransform(d2d::Vector(base_pos.x, new_pos.y), sprite->GetAngle());
	}

	Filling();

	return true;
}

bool UIList::RemoveSprite(d2d::ISprite* sprite)
{
	return m_items.Remove(sprite);
}

bool UIList::ClearAllSprite()
{
	if (m_base_spr) {
		m_base_spr->Release();
		m_base_spr = NULL;
	}
	if (m_hori_spr) {
		m_hori_spr->Release();
		m_hori_spr = NULL;
	}
	if (m_vert_spr) {
		m_vert_spr->Release();
		m_vert_spr = NULL;
	}

	bool ret = m_items.Clear();
	return ret;
}

void UIList::TraverseSprites(d2d::IVisitor& visitor) const
{
	m_items.Traverse(visitor, d2d::DT_ALL, true);
}

void UIList::StoreToFile(const char* filename) const
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	// items complex
	ecomplex::Symbol items_complex;
	m_items.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(items_complex.m_sprites), d2d::DT_ALL);
//	std::sort(items_complex.m_sprites.begin(), items_complex.m_sprites.end(), ItemsCmp());
	for (int i = 0, n = items_complex.m_sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = items_complex.m_sprites[i];
		spr->name = "item" + d2d::StringTools::ToString(i+1);
		spr->Retain();
	}
	std::string items_path = name + "_items_complex.json";
	items_complex.SetFilepath(items_path);
	ecomplex::FileStorer::Store(items_path.c_str(), &items_complex);

	// wrapper complex
	ecomplex::Sprite items_sprite(&items_complex);
	items_sprite.name = "anchor";
	ecomplex::Symbol wrapper_complex;
	wrapper_complex.m_clipbox = m_clipbox;
	wrapper_complex.m_sprites.push_back(&items_sprite);
	items_sprite.Retain();
	std::string top_path = name + "_wrapper_complex.json";
	wrapper_complex.SetFilepath(top_path);
	ecomplex::FileStorer::Store(top_path.c_str(), &wrapper_complex);

	// ui
	std::string ui_path = filename;
	Json::Value value;
	value["items filepath"] = d2d::FilenameTools::getRelativePath(dir, items_path).ToStdString();
	value["wrapper filepath"] = d2d::FilenameTools::getRelativePath(dir, top_path).ToStdString();
	value["type"] = "list";
	value["horizontal"] = m_horizontal;
	value["vertical"] = m_vertical;
	value["clipbox"]["w"] = m_clipbox.xLength();
	value["clipbox"]["h"] = m_clipbox.yLength();
	value["clipbox"]["x"] = m_clipbox.xMin;
	value["clipbox"]["y"] = m_clipbox.yMax;
	value["hori space"] = m_hori_space;
	value["vert space"] = m_vert_space;
	value["hori count"] = m_hori_count;
	value["vert count"] = m_vert_count;
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(ui_path.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void UIList::LoadFromFile(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string type = value["type"].asString();
	if (type != "list") {
		return;
	}

	const Json::Value& cb_val = value["clipbox"];
	m_clipbox.xMin = cb_val["x"].asDouble();
	m_clipbox.yMax = cb_val["y"].asDouble();
	m_clipbox.xMax = m_clipbox.xMin + cb_val["w"].asDouble();
	m_clipbox.yMin = m_clipbox.yMax - cb_val["h"].asDouble();

	m_horizontal = value["horizontal"].asBool();
	m_vertical = value["vertical"].asBool();

	m_hori_count = value["hori count"].asInt();
	m_vert_count = value["vert count"].asInt();

	m_hori_space = value["hori space"].asDouble();
	m_vert_space = value["vert space"].asDouble();

	std::string items_filepath = value["items filepath"].asString();
	items_filepath = d2d::FilenameTools::getAbsolutePathFromFile(filename, items_filepath);
	ecomplex::Symbol items_complex;
	items_complex.LoadFromFile(items_filepath);
	for (int i = 0, n = items_complex.m_sprites.size(); i < n; ++i) {
		d2d::ISprite* spr = items_complex.m_sprites[i];
		spr->Retain();
		m_items.Insert(spr);
	}
}

bool UIList::Filling()
{
	bool ret = false;

	m_items.Clear();

	if (!m_base_spr) {
		return false;
	}	
	if (!m_hori_spr && !m_vert_spr) {
		return false;
	}

	d2d::Vector base = m_base_spr->GetPosition();

	float dx = 0, dy = 0;
	if (m_hori_spr) {
		dx = fabs(base.x - m_hori_spr->GetPosition().x);
	} 
	if (m_vert_spr) {
		dy = fabs(base.y - m_vert_spr->GetPosition().y);
	}

	m_hori_space = dx;
	m_vert_space = dy;

	d2d::Rect item_r = m_base_spr->GetRect();
	float hw = item_r.xLength() * 0.5f;
	float hh = item_r.yLength() * 0.5f;

	d2d::Rect region = m_clipbox;
	region.xMax += dx * 2;
	region.yMin -= dy * 2;

	d2d::Vector pos = base;

	m_hori_count = 0;
	m_vert_count = 1;

	m_base_spr->Retain();
	m_items.Insert(m_base_spr);
	while (true) {
		if (pos.y - hh < region.yMin) {
			break;
		} else if (dx == 0) {
			pos.y -= dy;
			++m_vert_count;
		}
		int count = 0;
		while (true) {
			if (pos.x + hw > region.xMax) {
				pos.x = base.x;
				pos.y -= dy;
				++m_vert_count;
				break;
			} else {
				d2d::ISprite* spr = m_base_spr->Clone();
				spr->SetTransform(pos, spr->GetAngle());
				m_items.Insert(spr);
				ret = true;
				pos.x += dx;
				++count;
			}
			if (dx == 0) {
				break;
			}
		}
		m_hori_count = count;
		if (dy == 0) {
			--m_vert_count;
			break;
		}
	}

	return ret;
}

void UIList::ChangeHori()
{
	m_horizontal = !m_horizontal;
}

void UIList::ChangeVert()
{
	m_vertical = !m_vertical;
}

}
}