#include "DynamicInfo.h"
#include "Exception.h"
#include "StringHelper.h"

#include <vector>

namespace ee
{

std::string DynamicInfo::QueryValue(const std::string& key) const
{
	std::map<std::string, Item>::const_iterator itr 
		= m_map_val.find(key);
	if (itr == m_map_val.end()) {
		return "";
	} else {
		return itr->second.val;
	}
}

std::string DynamicInfo::QueryTag(const std::string& key) const
{
	std::map<std::string, Item>::const_iterator itr 
		= m_map_val.find(key);
	if (itr == m_map_val.end()) {
		return "";
	} else {
		return itr->second.tag;
	}
}

void DynamicInfo::SetValue(const std::string& key, const std::string& val, 
						   bool is_default, const std::string& tag)
{
	std::map<std::string, Item>::iterator itr 
		= m_map_val.find(key);
	if (itr == m_map_val.end()) {
		Item item;
		item.val = val;
		item.is_default = is_default;
		item.tag = tag;
		m_map_val.insert(std::make_pair(key, item));
	} else {
		itr->second.is_default = is_default;
		if (!is_default) {
			itr->second.val = val;
			itr->second.tag = tag;
		}
	}
}

void DynamicInfo::LoadFromString(const std::string& str, const std::string& tag)
{
	std::vector<std::string> pairs;
	ee::StringHelper::Split(str, ";", pairs);

	for (int i = 0, n = pairs.size(); i < n; ++i) 
	{
		const std::string& pair = pairs[i];
		int pos = pair.find('=');
		if (pos == std::string::npos) {
			m_others.push_back(pair);
			continue;
		}

		std::string key = pair.substr(0, pos),
			val = pair.substr(pos + 1);
		if (key.empty() || val.empty()) {
			continue;
		}

		SetValue(key, val, false, tag);
	}
}

std::string DynamicInfo::ToString() const
{
	std::string str;

	for (int i = 0, n = m_others.size(); i < n; ++i) {
		str += m_others[i] + ";";
	}

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