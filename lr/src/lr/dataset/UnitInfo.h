#ifndef _LR_UNIT_INFO_H_
#define _LR_UNIT_INFO_H_

#include <string>
#include <map>

namespace lr
{
	
class UnitInfo
{
public:
	UnitInfo(const std::string& str);

	std::string QueryValue(const std::string& key) const;

	void SetValue(const std::string& key, const std::string& val);

	std::string ToString() const;

public:
	std::map<std::string, std::string> map_val;

}; // UnitInfo

}

#endif // _LR_UNIT_INFO_H_