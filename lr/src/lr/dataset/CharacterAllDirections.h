#ifndef _LR_CHARACTER_ALL_DIRECTIONS_H_
#define _LR_CHARACTER_ALL_DIRECTIONS_H_

#include "CharacterFileName.h"

#include <map>

namespace ee { class Symbol; }

namespace lr
{

class CharacterAllDirections
{
public:

	void BuildSymbolDirections(const CharacterFileName& name);

	ee::Symbol* GetSymbolByDir(const std::string& filepath, int dir_pos) const;

private:
	static const int DIR_COUNT = 5;

	struct Directions
	{
		ee::Symbol* symbols[DIR_COUNT];
	}; // Directions

private:
	std::map<std::string, Directions> m_map2symbols;

}; // CharacterAllDirections

}

#endif // _LR_CHARACTER_ALL_DIRECTIONS_H_