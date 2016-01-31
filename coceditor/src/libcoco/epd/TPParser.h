#ifndef _LIBCOCO_EPD_TP_PARSER_H_
#define _LIBCOCO_EPD_TP_PARSER_H_



#include "TPAdapter.h"
#include "TextureMgr.h"
#include "../SymbolDependanceSorter.h"

namespace libcoco
{
namespace epd
{

class TPParser
{
public:
	struct Picture
	{
		std::string filename;

		const TPAdapter::Entry* entry;

		float invscale;

		int tex;

		bool rotated;

		// 0 3
		// 1 2
		ee::Vector scr[4];

		ee::Vector offset;
	};

public:
	TPParser(const std::vector<const ee::Symbol*>& symbols,
		const TextureMgr& tex_mgr);

	void Parser();

	int GetTexSize() const { return m_tex_size; }

	const SymbolSet& GetSymbolSet() const {
		return m_symbol_sorter.GetSymbolSet();
	}

	TPParser::Picture* FindPicture(const ee::Symbol* symbol) const;

	void DebugInfo() const;

private:
	void ParserTexture(const TextureMgr::Entry* tex, int idx);

private:
	SymbolDependanceSorter m_symbol_sorter;

	const TextureMgr& m_tex_mgr;

	int m_tex_size;

	std::map<const ee::Symbol*, Picture*> m_map_symbol2picture;

}; // TPParser

}
}

#endif // _LIBCOCO_EPD_TP_PARSER_H_
