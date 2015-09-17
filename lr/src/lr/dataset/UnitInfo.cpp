#include "UnitInfo.h"

#include <drag2d.h>

namespace lr
{

UnitInfo::UnitInfo(const std::string& str)
{
	std::vector<std::string> pairs;
	d2d::StringTools::Split(str, ";", pairs);

	for (int i = 0, n = pairs.size(); i < n; ++i) {
		const std::string& pair = pairs[i];
		int pos = pair.find('=');
		if (pos == std::string::npos) {
			continue;
		}

		std::string key = pair.substr(0, pos),
			val = pair.substr(pos + 1);
		if (key.empty() || val.empty()) {
			continue;
		}

		SetValue(key, val);
	}
}

std::string UnitInfo::QueryValue(const std::string& key) const
{
	std::map<std::string, Item>::const_iterator itr 
		= m_map_val.find(key);
	if (itr == m_map_val.end()) {
		return "";
	} else {
		return itr->second.val;
	}
}

void UnitInfo::SetValue(const std::string& key, const std::string& val,
						bool is_default)
{
	std::map<std::string, Item>::iterator itr 
		= m_map_val.find(key);
	if (itr == m_map_val.end()) {
		Item item;
		item.val = val;
		item.is_default = is_default;
		m_map_val.insert(std::make_pair(key, item));
	} else {
		itr->second.is_default = is_default;
		if (!is_default) {
			itr->second.val = val;
		}
	}
}

std::string UnitInfo::ToString() const
{
	std::string str;

	std::map<std::string, Item>::const_iterator itr
		= m_map_val.begin();
	for ( ; itr != m_map_val.end(); ++itr) {
		if (itr->second.is_default) {
			continue;
		}
		str += itr->first + "=" + itr->second.val + ";";
	}

	return str;
}

}