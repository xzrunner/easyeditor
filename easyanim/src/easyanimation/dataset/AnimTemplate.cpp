#include "AnimTemplate.h"

#include "view/ToolbarPanel.h"

namespace eanim
{

AnimTemplate::AnimTemplate() 
	: m_choice(0)
{
}

void AnimTemplate::Clear() 
{
	m_items.clear();
	m_choice = 0;
}

bool AnimTemplate::Empty() const 
{ 
	return m_items.empty(); 
}

const std::string& AnimTemplate::Dir() const 
{
	return m_items[m_choice].path; 
}

void AnimTemplate::Add(const std::string& path, const std::string& name)
{
	m_items.push_back(Item(path, name));
}

void AnimTemplate::StoreToFile(Json::Value& value) const
{
	if (Empty()) {
		return;
	}

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		Json::Value item_val;
		item_val["path"] = m_items[i].path;
		item_val["name"] = m_items[i].name;
		 value["template"][i] = item_val;
	}
}

void AnimTemplate::LoadFromFile(const Json::Value& value,
								ToolbarPanel* toolbar)
{
	Clear();

	std::vector<std::string> anims;

	int i = 0;
	Json::Value item_val = value[i++];
	while (!item_val.isNull()) {
		std::string path = item_val["path"].asString(),
			name = item_val["name"].asString();
		Add(path, name);
		anims.push_back(name);
		item_val = value[i++];
	}

	toolbar->AddAnimChoice(anims);
}

}