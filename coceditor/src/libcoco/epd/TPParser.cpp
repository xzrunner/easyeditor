#include "TPParser.h"

namespace libcoco
{
namespace epd
{

TPParser::TPParser(const std::vector<const ee::Symbol*>& symbols,
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

TPParser::Picture* TPParser::FindPicture(const ee::Symbol* symbol) const
{
	std::map<const ee::Symbol*, TPParser::Picture*>::const_iterator itr 
		= m_map_symbol2picture.find(symbol);
	if (itr == m_map_symbol2picture.end()) {
		return NULL;
	} else {
		return itr->second;
	}
}

void TPParser::DebugInfo() const
{
	std::ofstream fout("d:/zz_debug.txt");

	std::map<const ee::Symbol*, Picture*>::const_iterator itr 
		= m_map_symbol2picture.begin();
	for ( ; itr != m_map_symbol2picture.end(); ++itr) {
		fout << itr->first->GetFilepath() << "\n";
		fout << itr->first << "\n\n";
	}

	fout.close();
}

void TPParser::ParserTexture(const TextureMgr::Entry* tex, int idx)
{
	const TPAdapter& adapter = tex->adapter;
	const std::vector<TPAdapter::Entry>& frames = adapter.GetFrames();
	bool flip_y = adapter.IsEasyDBData();
	for (size_t i = 0, n = frames.size(); i < n; ++i)
	{
		const TPAdapter::Entry& entry = frames[i];

		// find symbol
		const ee::Symbol* symbol = m_symbol_sorter.GetSymbolSet().Query(entry.filename);
		if (!symbol) {
			continue;
		}

		Picture* picture = new Picture;

		picture->filename = entry.filename;
		picture->entry = &entry;
		picture->invscale = adapter.GetInvScale();
		picture->tex = idx;

		picture->rotated = entry.rotated;

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

			picture->scr[0].Set(right, up);
			picture->scr[1].Set(left, up);
			picture->scr[2].Set(left, down);
			picture->scr[3].Set(right, down);
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

			picture->scr[0].Set(left, up);
			picture->scr[1].Set(left, down);
			picture->scr[2].Set(right, down);
			picture->scr[3].Set(right, up);
		}

		picture->offset.x = (entry.sprite_source_size.x + entry.sprite_source_size.w * 0.5f) - entry.src_width * 0.5f;
		picture->offset.y = entry.src_height * 0.5f - (entry.sprite_source_size.y + entry.sprite_source_size.h * 0.5f);

		m_map_symbol2picture.insert(std::make_pair(symbol, picture));
	}
}

}
}
