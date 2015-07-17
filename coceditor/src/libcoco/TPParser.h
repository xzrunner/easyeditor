#ifndef _LIBCOCO_TP_PARSER_H_
#define _LIBCOCO_TP_PARSER_H_

#include <drag2d.h>

#include "TPAdapter.h"
#include "TextureMgr.h"
#include "SymbolDependanceSorter.h"

namespace libcoco
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

		// 0 3
		// 1 2
		d2d::Vector scr[4];

		d2d::Vector offset;
	};

public:
	TPParser(const std::vector<const d2d::ISymbol*>& symbols,
		const TextureMgr& tex_mgr);

	void Parser();

	int GetTexSize() const { return m_tex_size; }

	const SymbolSet& GetSymbolSet() const {
		return m_symbol_sorter.GetSymbolSet();
	}

	TPParser::Picture* FindPicture(const d2d::ISymbol* symbol) const;

	void DebugInfo() const;

private:
	void ParserTexture(const TextureMgr::Entry* tex, int idx);

private:
	SymbolDependanceSorter m_symbol_sorter;

	const TextureMgr& m_tex_mgr;

	int m_tex_size;

	std::map<const d2d::ISymbol*, Picture*> m_map_symbol2picture;

}; // TPParser

}

#endif // _LIBCOCO_TP_PARSER_H_
