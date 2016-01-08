#ifndef _LR_DYNAMIC_INFO_H_
#define _LR_DYNAMIC_INFO_H_

#include <string>
#include <map>

namespace lr
{

static const std::string DEFAULT_VAL = "_DEFAULT_";;
	
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

}; // DynamicInfo

}

#endif // _LR_DYNAMIC_INFO_H_