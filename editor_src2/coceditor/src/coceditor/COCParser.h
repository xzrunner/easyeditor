#pragma once

#include <drag2d.h>
#include <easycoco.h>

namespace coceditor
{
	class COCParser
	{
	public:
		
		void parser();

	private:
		void prepareMapSymbolPic();

	public:
		struct Picture
		{
			std::string filename;

			const d2d::TexturePackerFileAdapter::Entry* entry;

			int tex;

			// 0 3
			// 1 2
			d2d::Vector scr[4];

			d2d::Vector offset;
		};

	private:
		std::set<const d2d::ISymbol*> m_setSymbols;

	public:
//		std::vector<const d2d::ISymbol*> m_outList;

		libcoco::SymbolsPrepare m_symbolPrepare;

		std::map<const d2d::ISymbol*, Picture*> m_mapSymbolPicture;

	}; // COCParser
}

