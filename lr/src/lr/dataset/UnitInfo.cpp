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
	std::map<std::string, std::string>::const_iterator itr 
		= map_val.find(key);
	if (itr == map_val.end()) {
		return "";
	} else {
		return itr->second;
	}
}

void UnitInfo::SetValue(const std::string& key, const std::string& val)
{
	std::map<std::string, std::string>::iterator itr 
		= map_val.find(key);
	if (itr == map_val.end()) {
		map_val.insert(std::make_pair(key, val));
	} else {
		itr->second = val;
	}
}

std::string UnitInfo::ToString() const
{
	std::string str;

	std::map<std::string, std::string>::const_iterator itr
		= map_val.begin();
	for ( ; itr != map_val.end(); ++itr) {
		str += itr->first + "=" + itr->second + ";";
	}

	return str;
}

}