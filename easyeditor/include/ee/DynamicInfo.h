#ifndef _EASYEDITOR_DYNAMIC_INFO_H_
#define _EASYEDITOR_DYNAMIC_INFO_H_

#include <string>
#include <map>
#include <vector>

namespace ee
{

class DynamicInfo
{
public:
	std::string QueryValue(const std::string& key) const;
	std::string QueryTag(const std::string& key) const;

	void SetValue(const std::string& key, const std::string& val,
		bool is_default = false, const std::string& tag = "");

	void LoadFromString(const std::string& str, const std::string& tag = "");

	std::string ToString() const;

private:
	struct Item
	{
		std::string val;
		bool is_default;
		std::string tag;
	};

	std::map<std::string, Item> m_map_val;

	std::vector<std::string> m_others;

}; // DynamicInfo

}

#endif // _EASYEDITOR_DYNAMIC_INFO_H_