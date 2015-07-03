#ifndef _EASYANIM_ANIM_TEMPLATE_H_
#define _EASYANIM_ANIM_TEMPLATE_H_

#include <string>
#include <json/json.h>

namespace eanim
{

class ToolbarPanel;

class AnimTemplate
{
public:
	AnimTemplate();

	void Clear();

	bool Empty() const;

	const std::string& Dir() const;

	void SetChoice(int choice) { m_choice = choice; }

	void Add(const std::string& path, const std::string& name);

	void PreparePaths(const std::string& filepath);
	void StoreToFile(Json::Value& value) const;
	void LoadFromFile(const Json::Value& value, ToolbarPanel* toolbar);

	void SetTemplateDir(const std::string& dir) { m_temp_dir = dir; }

	bool ContainPath(const std::string& path) const;

private:
	struct Item
	{
		Item(const std::string& path, const std::string& name)
			: path(path), name(name) {}

		std::string path;
		std::string name;
	};

private:
	std::vector<Item> m_items;
	int m_choice;

	std::string m_temp_dir;

}; // AnimTemplate

}

#endif // _EASYANIM_ANIM_TEMPLATE_H_