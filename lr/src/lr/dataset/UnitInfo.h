#ifndef _LR_UNIT_INFO_H_
#define _LR_UNIT_INFO_H_

#include <string>
#include <map>

namespace lr
{

static const std::string DEFAULT_VAL = "_DEFAULT_";;
	
class UnitInfo
{
public:
	UnitInfo(const std::string& str);

	std::string QueryValue(const std::string& key) const;

	void SetValue(const std::string& key, const std::string& val,
		bool is_default = false);

	std::string ToString() const;

private:
	struct Item
	{
		std::string val;
		bool is_default;
	};

	std::map<std::string, Item> m_map_val;

}; // UnitInfo

}

#endif // _LR_UNIT_INFO_H_