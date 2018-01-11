#pragma once

#include <map>
#include <string>

namespace esprpacker
{

class ParserLuaData
{
public:
	
	void LoadFromFile(const std::string& filepath);

private:
	std::map<int, std::string> m_spr_nodes;

}; // ParserLuaData

}