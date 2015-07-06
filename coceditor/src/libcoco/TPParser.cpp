#include "TPParser.h"

namespace libcoco
{

TPParser::TPParser(const std::vector<const d2d::ISymbol*>& symbols,
				   const TextureMgr& tex_mgr)
	: m_tex_mgr(tex_mgr)
	, m_tex_size(0)
{
	m_symbol_sorter.prepare(symbols);	
}

void TPParser::Parser()
{
	int idx = 0;
	while (const TextureMgr::Entry* tex = m_tex_mgr.GetTexture(idx)) {
		ParserTexture(tex, idx);
		++idx;
	}
	m_tex_size = idx;
}

TPParser::Picture* TPParser::FindPicture(const d2d::ISymbol* symbol) const
{
	std::map<const d2d::ISymbol*, TPParser::Picture*>::const_iterator itr 
		= m_map_symbol2picture.find(symbol);
	if (itr == m_map_symbol2picture.end()) {
		return NULL;
	} else {
		return itr->second;
	}
}

void TPParser::ParserTexture(const TextureMgr::Entry* tex, int idx)
{
	const std::vector<const d2d::ISymbol*>& symbols = m_symbol_sorter.GetAllSymbols();

	const TPAdapter& adapter = tex->adapter;
	const std::vector<TPAdapter::Entry>& frames = adapter.GetFrames();
	bool flip_y = adapter.IsEasyDBData();
	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		const TPAdapter::Entry& entry = frames[i];

		// find symbol
		const d2d::ISymbol* symbol = NULL;
		for (size_t j = 0, m = symbols.size(); j < m; ++j)
		{
			const d2d::ISymbol* s = symbols[j];
			std::string filepath = s->GetFilepath();
			d2d::StringTools::ToLower(filepath);
			d2d::FilenameTools::formatSeparators(filepath);

			bool find = false;
			if (entry.filename.find("/") == std::string::npos) {
				if (filepath.find("/"+entry.filename) != std::string::npos) {
					find = true;
				}
			} else {
				if (filepath.find(entry.filename) != std::string::npos) {
					find = true;
				}
			}

			if (find) {
				symbol = s;
				break;
			}
		}
		if (!symbol) continue;

		Picture* picture = new Picture;

		picture->filename = entry.filename;
		picture->entry = &entry;
		picture->invscale = adapter.GetInvScale();
		picture->tex = idx;

		float left, right, up, down;
		if (entry.rotated)
		{
			left = entry.frame.x;
			right = entry.frame.x + entry.frame.h;
			if (flip_y) {
				up = adapter.GetHeight() - entry.frame.y;
				down = adapter.GetHeight() - (entry.frame.y + entry.frame.w);
			} else {
				up = entry.frame.y;
				down = entry.frame.y + entry.frame.w;
			}

			picture->scr[0].set(right, up);
			picture->scr[1].set(left, up);
			picture->scr[2].set(left, down);
			picture->scr[3].set(right, down);
		}
		else
		{
			left = entry.frame.x;
			right = entry.frame.x + entry.frame.w;
			if (flip_y) {
				up = adapter.GetHeight() - entry.frame.y;
				down = adapter.GetHeight() - (entry.frame.y + entry.frame.h);
			} else {
				up = entry.frame.y;
				down = entry.frame.y + entry.frame.h;
			}

			picture->scr[0].set(left, up);
			picture->scr[1].set(left, down);
			picture->scr[2].set(right, down);
			picture->scr[3].set(right, up);
		}

		picture->offset.x = (entry.sprite_source_size.x + entry.sprite_source_size.w * 0.5f) - entry.src_width * 0.5f;
		picture->offset.y = entry.src_height * 0.5f - (entry.sprite_source_size.y + entry.sprite_source_size.h * 0.5f);

		m_map_symbol2picture.insert(std::make_pair(symbol, picture));
	}
}

} // coceditor