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
}

void UIList::StoreToFile(const char* filename) const
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	std::string dir = d2d::FilenameTools::getFileDir(filename);

	// items complex
	ecomplex::Symbol items_complex;
	m_items.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(items_complex.m_sprites), d2d::DT_ALL);
	std::sort(items_complex.m_sprites.begin(), items_complex.m_sprites.end(), ItemsCmp());
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
	items_sprite.name = "list";
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
	// not support hori & vert filling
	if (m_items.Size() != 2) {
		return false;
	}

	std::vector<d2d::ISprite*> ori_sprites;
	m_items.Traverse(d2d::FetchAllVisitor<d2d::ISprite>(ori_sprites), d2d::DT_ALL, true);

	assert(ori_sprites.size() == 2);
	if (&ori_sprites[0]->GetSymbol() != &ori_sprites[1]->GetSymbol()) {
		return false;
	}
	if (ori_sprites[0]->GetPosition().x != ori_sprites[1]->GetPosition().x &&
		ori_sprites[0]->GetPosition().y != ori_sprites[1]->GetPosition().y) {
		return false;
	}
	if (ori_sprites[0]->GetPosition().x == ori_sprites[1]->GetPosition().x &&
		ori_sprites[0]->GetPosition().y == ori_sprites[1]->GetPosition().y) {
		return false;
	}

	std::vector<d2d::ISprite*> new_sprites;
	if (ori_sprites[0]->GetPosition().x == ori_sprites[1]->GetPosition().x) {
		VertFilling(ori_sprites, new_sprites);
	} else if (ori_sprites[0]->GetPosition().y == ori_sprites[1]->GetPosition().y) {
		HoriFilling(ori_sprites, new_sprites);
	} else {
		return false;
	}

	for (int i = 0; i < new_sprites.size(); ++i) {
		m_items.Insert(new_sprites[i]);
	}

	return !new_sprites.empty();
}

void UIList::HoriFilling(const std::vector<d2d::ISprite*>& ori_sprites, 
						 std::vector<d2d::ISprite*>& new_sprites)
{
	m_horizontal = true;
	m_vertical = false;
	m_hori_space = fabs(ori_sprites[0]->GetPosition().x - ori_sprites[1]->GetPosition().x);
	m_vert_space = 1;

	float y = ori_sprites[0]->GetPosition().y;
	float x_min, x_max;
	if (ori_sprites[1]->GetPosition().x > ori_sprites[0]->GetPosition().x) {
		x_min = ori_sprites[0]->GetPosition().x;
		x_max = ori_sprites[1]->GetPosition().x;
	} else {
		x_min = ori_sprites[1]->GetPosition().x;
		x_max = ori_sprites[0]->GetPosition().x;
	}

	float dx = x_max - x_min;

	float _x = x_min - dx, _y = y;
	while (true) {
		d2d::ISprite* spr = ori_sprites[0]->Clone();
		spr->SetTransform(d2d::Vector(_x, _y), ori_sprites[0]->GetAngle());
		if (d2d::Math::isRectContainRect(m_clipbox, spr->GetRect())) {
			new_sprites.push_back(spr);
			_x -= dx;
		} else {
			delete spr;
			break;
		}
	}

	_x = x_max + dx;
	while (true) {
		d2d::ISprite* spr = ori_sprites[0]->Clone();
		spr->SetTransform(d2d::Vector(_x, _y), ori_sprites[0]->GetAngle());
		if (d2d::Math::isRectContainRect(m_clipbox, spr->GetRect())) {
			new_sprites.push_back(spr);
			_x += dx;
		} else {
			// add two more
			new_sprites.push_back(spr);
			spr = ori_sprites[0]->Clone();
			spr->SetTransform(d2d::Vector(_x + dx, _y), ori_sprites[0]->GetAngle());
			new_sprites.push_back(spr);
			break;
		}
	}

	m_hori_count = m_items.Size() + new_sprites.size();
	m_vert_count = 0;
}

void UIList::VertFilling(const std::vector<d2d::ISprite*>& ori_sprites, std::vector<d2d::ISprite*>& new_sprites)
{
	m_horizontal = false;
	m_vertical = true;
	m_hori_space = 1;
	m_vert_space = fabs(ori_sprites[0]->GetPosition().y - ori_sprites[1]->GetPosition().y);

	float x = ori_sprites[0]->GetPosition().x;
	float y_min, y_max;
	if (ori_sprites[1]->GetPosition().y > ori_sprites[0]->GetPosition().y) {
		y_min = ori_sprites[0]->GetPosition().y;
		y_max = ori_sprites[1]->GetPosition().y;
	} else {
		y_min = ori_sprites[1]->GetPosition().y;
		y_max = ori_sprites[0]->GetPosition().y;
	}

	float dy = y_max - y_min;

	float _x = x, _y = y_min - dy;
	while (true) {
		d2d::ISprite* spr = ori_sprites[0]->Clone();
		spr->SetTransform(d2d::Vector(_x, _y), ori_sprites[0]->GetAngle());
		if (d2d::Math::isRectContainRect(m_clipbox, spr->GetRect())) {
			new_sprites.push_back(spr);
			_y -= dy;
		} else {
			new_sprites.push_back(spr);
			spr = ori_sprites[0]->Clone();
			spr->SetTransform(d2d::Vector(_x, _y - dy), ori_sprites[0]->GetAngle());
			new_sprites.push_back(spr);
			break;
		}
	}

	_y = y_max + dy;
	while (true) {
		d2d::ISprite* spr = ori_sprites[0]->Clone();
		spr->SetTransform(d2d::Vector(_x, _y), ori_sprites[0]->GetAngle());
		if (d2d::Math::isRectContainRect(m_clipbox, spr->GetRect())) {
			new_sprites.push_back(spr);
			_y += dy;
		} else {
			delete spr;
			break;
		}
	}

	m_hori_count = 0;
	m_vert_count = m_items.Size() + new_sprites.size();
}

bool UIList::ItemsCmp::operator() (const d2d::ISprite* item0, const d2d::ISprite* item1) const
{
	if (item0->GetPosition().x != item1->GetPosition().x) {
		return item0->GetPosition().x < item1->GetPosition().x;
	} else {
		return item0->GetPosition().y >= item1->GetPosition().y;
	}
}

}
}