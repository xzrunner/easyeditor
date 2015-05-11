#include "CharacterAllDirections.h"

namespace lr
{

void CharacterAllDirections::BuildSymbolDirections(const CharacterFileName& name)
{
	std::string key = name.ConnectFieldExcept(CharacterFileName::FT_DIRECTION);
	std::map<std::string, Directions>::iterator itr = m_map2symbols.find(key);
	if (itr != m_map2symbols.end()) {
		return;
	}

	Directions dirs;
	for (int i = 0; i < DIR_COUNT; ++i) 
	{
		std::string val;
		val.push_back(char('1' + i));
		std::string filepath = name.GetFilepathSwitchField(CharacterFileName::FT_DIRECTION, val);
		assert(d2d::FilenameTools::isExist(filepath));
		dirs.symbols[i] = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	}
	m_map2symbols.insert(std::make_pair(key, dirs));
}

d2d::ISymbol* CharacterAllDirections::GetSymbolByDir(const std::string& filepath, int dir_pos) const
{
	assert(dir_pos >= 1 && dir_pos <= 8);
	CharacterFileName name(filepath);
	std::string key = name.ConnectFieldExcept(CharacterFileName::FT_DIRECTION);
	std::map<std::string, Directions>::const_iterator itr = m_map2symbols.find(key);
	if (itr == m_map2symbols.end()) {
		return NULL;
	} else {
		int idx;
		if (dir_pos >= 1 && dir_pos <= 5) {
			idx = dir_pos - 1;
		} else {
			idx = 9 - dir_pos;
		}
		return itr->second.symbols[idx];
	}
}

}