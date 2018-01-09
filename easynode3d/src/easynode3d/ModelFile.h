#pragma once

#include <cu/cu_macro.h>

#include <string>
#include <map>

namespace enode3d
{

class ModelFile
{
public:
	int Type(const std::string& filepath) const;

	std::string Tag(int type) const;

private:
	void Regist(int type, const std::string& tag);

protected:
	std::map<int, std::string> m_type2tag;
	std::map<std::string, int> m_tag2type;

	CU_SINGLETON_DECLARATION(ModelFile);

}; // ModelFile

}