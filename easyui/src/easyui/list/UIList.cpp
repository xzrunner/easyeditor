#include "UIList.h"
#include "widget_id.h"

#include <ee/FileHelper.h>
#include <ee/Visitor.h>
#include <ee/StringHelper.h>

#include <easyui.h>
#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>
#include <sprite2/BoundingBox.h>

#include <algorithm>

namespace eui
{
namespace list
{

UIList::UIList()
	: m_reverse_order(false)
	, m_column_order(false)
{
	m_clipbox.xmin = m_clipbox.ymin = -200;
	m_clipbox.xmax = m_clipbox.ymax =  200;

	m_vertical = m_horizontal = false;
	m_hori_count = m_vert_count = 0;
	m_hori_space = m_vert_space = 0;

	m_item_spr = NULL;
}

bool UIList::InsertSprite(ee::Sprite* spr, int idx)
{
	if (!spr) {
		return false;
	}
	assert(m_items.size() == m_hori_count * m_vert_count);
	if (m_item_spr && spr->GetSymbol() != m_item_spr->GetSymbol()) {
		return false;
	}
	if (m_hori_count > 1 && m_vert_count > 1) {
		return false;
	}

	if (m_hori_count ==  0 && m_vert_count == 0) {
		spr->AddReference();
		m_item_spr = spr;
		ee::Sprite* cp = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)m_item_spr)->Clone());
		m_items.push_back(cp);
		m_hori_count = m_vert_count = 1;
		return true;
	}

	sm::vec2 base_pos = m_items[0]->GetPosition();
	sm::vec2 new_pos = spr->GetPosition();
	if (m_vert_count == 1 && m_hori_count == 1) {
		if (base_pos == new_pos) {
			return false;
		}
		float dx = fabs(base_pos.x - new_pos.x);
		float dy = fabs(base_pos.y - new_pos.y);
		if (dx > dy) {
			m_hori_space = dx;
		} else {
			m_vert_space = dy;
		}
	} else if (m_hori_count == 1) {
		if (base_pos.x == new_pos.x) {
			return false;
		} else {
			m_hori_space = fabs(base_pos.x - new_pos.x);
		}
	} else if (m_vert_count == 1) {
		assert(m_hori_count != 1);
		if (base_pos.y == new_pos.y) {
			return false;
		} else {
			m_vert_space = fabs(base_pos.y - new_pos.y);
		}
	}

	Filling();

	return true;
}

bool UIList::ClearAllSprite()
{
	bool ret = !m_items.empty();

	if (m_item_spr) {
		m_item_spr->RemoveReference();
		m_item_spr = NULL;
	}

	for_each(m_items.begin(), m_items.end(), cu::RemoveRefFonctor<ee::Sprite>());
	m_items.clear();

	return ret;
}

void UIList::TraverseSprites(ee::Visitor<ee::Sprite>& visitor) const
{
	if (m_reverse_order) {
		for (int i = m_items.size() - 1; i >= 0; --i) {
			bool has_next;
			visitor.Visit(m_items[i], has_next);
			if (!has_next) break;
		}
	} else {
		for (int i = 0, n = m_items.size(); i < n; ++i) {
			bool has_next;
			visitor.Visit(m_items[i], has_next);
			if (!has_next) break;
		}
	}
}

void UIList::StoreToFile(const char* filename) const
{
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));

	std::string dir = ee::FileHelper::GetFileDir(filename);

	// items complex
	ecomplex::Symbol items_complex;
	if (m_column_order) {
		assert(m_items.size() == m_hori_count * m_vert_count);
		for (int i = 0, n = m_items.size(); i < n; ++i) {
			ee::Sprite* spr = m_items[i];
			int row = i / m_hori_count,
				col = i % m_hori_count;
			int idx = col * m_vert_count + row;
			spr->SetName("item" + ee::StringHelper::ToString(idx + 1));
			items_complex.Add(spr);
		}
	} else if (m_reverse_order) {
		for (int i = m_items.size() - 1; i >= 0; --i) {
			ee::Sprite* spr = m_items[i];
			spr->SetName("item" + ee::StringHelper::ToString(i + 1));
			items_complex.Add(spr);
		}
	} else {
		for (int i = 0, n = m_items.size(); i < n; ++i) {
			ee::Sprite* spr = m_items[i];
			spr->SetName("item" + ee::StringHelper::ToString(i + 1));
			items_complex.Add(spr);
		}
	}
	std::string items_path = name + "_items_complex[gen].json";
	items_complex.SetFilepath(items_path);
	ecomplex::FileStorer::Store(items_path.c_str(), &items_complex);

	// wrapper complex
	ecomplex::Sprite items_sprite(&items_complex);
	items_sprite.SetName("anchor");
	ecomplex::Symbol wrapper_complex;
	wrapper_complex.m_clipbox = m_clipbox;
	wrapper_complex.Add(&items_sprite);
	items_sprite.AddReference();
	std::string top_path = name + "_wrapper_complex[gen].json";
	wrapper_complex.SetFilepath(top_path);
	ecomplex::FileStorer::Store(top_path.c_str(), &wrapper_complex);

	// ui
	std::string ui_path = filename;
	Json::Value value;
	value["items filepath"] = ee::FileHelper::GetRelativePath(dir, items_path);
	value["wrapper filepath"] = ee::FileHelper::GetRelativePath(dir, top_path);
	value["type"] = get_widget_desc(ID_LIST);
	value["horizontal"] = m_horizontal;
	value["vertical"] = m_vertical;
	value["reverse_order"] = m_reverse_order;
	value["column_order"] = m_column_order;
	sm::vec2 cb_sz = m_clipbox.Size();
	value["clipbox"]["w"] = cb_sz.x;
	value["clipbox"]["h"] = cb_sz.y;
	value["clipbox"]["x"] = m_clipbox.xmin;
	value["clipbox"]["y"] = m_clipbox.ymax;
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
	if (!is_widget(type, ID_LIST)) {
		return;
	}

	const Json::Value& cb_val = value["clipbox"];
	m_clipbox.xmin = cb_val["x"].asDouble();
	m_clipbox.ymax = cb_val["y"].asDouble();
	m_clipbox.xmax = m_clipbox.xmin + cb_val["w"].asDouble();
	m_clipbox.ymin = m_clipbox.ymax - cb_val["h"].asDouble();

	m_horizontal = value["horizontal"].asBool();
	m_vertical = value["vertical"].asBool();

	if (!value["reverse_order"].isNull()) {
		m_reverse_order = value["reverse_order"].asBool();
	} else {
		m_reverse_order = false;
	}
	if (!value["column_order"].isNull()) {
		m_column_order = value["column_order"].asBool();
	} else {
		m_column_order = false;
	}

	m_hori_count = value["hori count"].asInt();
	m_vert_count = value["vert count"].asInt();

	m_hori_space = value["hori space"].asDouble();
	m_vert_space = value["vert space"].asDouble();

	std::string items_filepath = value["items filepath"].asString();
	items_filepath = ee::FileHelper::GetAbsolutePathFromFile(filename, items_filepath);
	ecomplex::Symbol items_complex;
	items_complex.LoadFromFile(items_filepath);
	const std::vector<s2::Sprite*>& children = items_complex.GetChildren();
	if (!m_reverse_order) {
		for (int i = 0, n = children.size(); i < n; ++i) {
			ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);
			spr->AddReference();
			m_items.push_back(spr);
		}
		if (!children.empty()) {
			m_item_spr = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)children[0])->Clone());
		}
	} else {
		for (int i = 0, n = children.size(); i < n; ++i) {
			ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);
			spr->AddReference();
			m_items.insert(m_items.begin(), spr);
		}
		if (!children.empty()) {
			m_item_spr = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)children.back())->Clone());
		}
	}
}

bool UIList::ReFilling()
{
	if (!m_item_spr) {
		return false;
	}

	for_each(m_items.begin(), m_items.end(), cu::RemoveRefFonctor<ee::Sprite>());
	m_items.clear();

	ee::Sprite* cp = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)m_item_spr)->Clone());
	m_items.push_back(cp);

	m_hori_count = m_vert_count = 1;

	Filling();

	return true;
}

bool UIList::Arrange(const ee::Sprite* spr)
{
	if (m_items.empty()) {
		return false;
	}

	int idx = -1;
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		if (m_items[i] == spr) {
			idx = i;
			break;
		}
	}
	assert(idx != -1);
	int row = idx / m_hori_count,
		col = idx % m_hori_count;
	if (idx == 0) {
		m_item_spr->SetPosition(m_items[0]->GetPosition());
	}
	if (row > 0) {
		float space = fabs(spr->GetPosition().y - m_item_spr->GetPosition().y) / row;
		if (space > 0) {
			m_vert_space = space;
		}
	}
	if (col > 0) {
		float space = fabs(spr->GetPosition().x - m_item_spr->GetPosition().x) / col;
		if (space > 0) {
			m_hori_space = space;
		}
	}
	return Arrange(m_hori_space, m_vert_space);
}

void UIList::EnableHori(bool enable)
{
	m_horizontal = enable;
}

void UIList::EnableVert(bool enable)
{
	m_vertical = enable;
}

bool UIList::Filling()
{
	if (!m_item_spr) {
		return false;
	}
	if (m_hori_count == 0 && m_vert_count == 0 ||
		m_hori_count > 1 && m_vert_count > 1) {
		return false;
	}

	bool ret = false;
	assert((m_hori_count == 1 && m_vert_count == 1) ||
		((m_hori_count == 1 || m_vert_count == 1) &&
		(m_hori_count > 1 || m_vert_count > 1)));

	for_each(m_items.begin(), m_items.end(), cu::RemoveRefFonctor<ee::Sprite>());
	m_items.clear();

	sm::vec2 base = m_item_spr->GetPosition();
	sm::vec2 item_sz = m_item_spr->GetBounding()->GetSize().Size();
	float hw = item_sz.x * 0.5f;
	float hh = item_sz.y * 0.5f;

	sm::rect region = m_clipbox;
	if (m_horizontal) {
		region.xmax += m_hori_space * 2;
	}
	if (m_vertical) {
		region.ymin -= m_vert_space * 2;
	}

	sm::vec2 pos = base;
	m_hori_count = m_vert_count = 0;
	while (true) {
		bool new_line = false;
		if (pos.y - hh < region.ymin) {
			break;
		}
		int count = 0;
		while (true) {
			if (pos.x + hw > region.xmax) {
				pos.x = base.x;
				pos.y -= m_vert_space;
				break;
			} else {
				new_line = true;
				ee::Sprite* spr = dynamic_cast<ee::Sprite*>(((cu::Cloneable*)m_item_spr)->Clone());
				spr->SetPosition(pos);
				m_items.push_back(spr);
				ret = true;
				pos.x += m_hori_space;
				++count;
			}
			if (m_hori_space == 0) {
				break;
			}
		}
		if (new_line) {
			new_line = false;
			++m_vert_count;
		}
		m_hori_count = count;
		if (m_hori_space == 0) {
			pos.y -= m_vert_space;
		}
		if (m_vert_space == 0) {
			break;
		}
	}

	return ret;
}

bool UIList::Arrange(float hori_space, float vert_space)
{
	assert(m_hori_count >= 1 && m_vert_count >= 1);
	assert(m_vert_count * m_hori_count == m_items.size());

	m_hori_space = hori_space;
	m_vert_space = vert_space;
	sm::vec2 pos = m_items[0]->GetPosition();
	float x_base = pos.x;
	int count = 0;
	for (int i = 0, n = m_items.size(); i < n; ++i) {
		ee::Sprite* spr = m_items[i];
		spr->SetPosition(pos);
		pos.x += m_hori_space;

		++count;
		if (count >= m_hori_count) {
			pos.x = x_base;
			pos.y -= m_vert_space;
			count = 0;
		}
	}

	return true;
}

}
}