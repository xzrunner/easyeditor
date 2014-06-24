#include "COCParser.h"

#include "Context.h"

namespace coceditor
{

void COCParser::parser()
{
	Context* context = Context::Instance();
	m_symbolPrepare.prepare(context->symbols);

	prepareMapSymbolPic();
}

void COCParser::prepareMapSymbolPic()
{
	const std::vector<const d2d::ISymbol*>& sorted = m_symbolPrepare.getResult();

	const TexturesMgr& texMgr = Context::Instance()->texMgr;
	for (size_t i = 0; i < TexturesMgr::MAX_NUM; ++i)
	{
		if (!texMgr.textures[i])
			continue;
		const TexPackerAdapter& adapter = texMgr.textures[i]->adapter;
		for (size_t j = 0, m = adapter.frames.size(); j < m; ++j)
		{
			const TexPackerAdapter::Entry& entry = adapter.frames[j];

			// find symbol
			const d2d::ISymbol* symbol = NULL;
			for (size_t k = 0, l = sorted.size(); k < l; ++k)
			{
				const d2d::ISymbol* s = sorted[k];
				std::string filepath = s->getFilepath();
				StringTools::toLower(filepath);
				d2d::FilenameTools::formatSeparators(filepath);

				bool find = false;
				if (entry.filename.find("/") == std::string::npos) {
					if (filepath.find("/"+entry.filename) != std::string::npos)
						find = true;
				} else {
					if (filepath.find(entry.filename) != std::string::npos)
						find = true;
				}

				if (find)
				{
					symbol = s;
					break;
				}
			}
			if (!symbol) continue;

			Picture* picture = new Picture;

			picture->filename = entry.filename;
			picture->entry = &entry;
			picture->invscale = adapter.invscale;
			picture->tex = i;
			
			if (entry.rotated)
			{
				const float left = entry.frame.x,
					right = entry.frame.x + entry.frame.h,
					up = entry.frame.y,
					down = entry.frame.y + entry.frame.w;

 				picture->scr[0].set(right, up);
 				picture->scr[1].set(left, up);
 				picture->scr[2].set(left, down);
 				picture->scr[3].set(right, down);
			}
			else
			{
				const float left = entry.frame.x,
					right = entry.frame.x + entry.frame.w,
					up = entry.frame.y,
					down = entry.frame.y + entry.frame.h;

 				picture->scr[0].set(left, up);
 				picture->scr[1].set(left, down);
 				picture->scr[2].set(right, down);
 				picture->scr[3].set(right, up);
			}

			picture->offset.x = (entry.spriteSourceSize.x + entry.spriteSourceSize.w * 0.5f) - entry.srcWidth * 0.5f;
			picture->offset.y = entry.srcHeight * 0.5f - (entry.spriteSourceSize.y + entry.spriteSourceSize.h * 0.5f);

			m_mapSymbolPicture.insert(std::make_pair(symbol, picture));
		}
	}
}

} // coceditor