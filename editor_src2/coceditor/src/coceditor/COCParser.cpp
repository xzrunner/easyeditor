#include "COCParser.h"

#include "Context.h"

#include <queue>

namespace coceditor
{

void COCParser::parser()
{
	prepareSymbols();
	prepareOutList();
	prepareMapSymbolPic();
}

void COCParser::prepareSymbols()
{
	Context* context = Context::Instance();
	if (context->symbols.empty())
		return;

	for (size_t i = 0, n = context->symbols.size(); i < n; ++i)
		m_setSymbols.insert(context->symbols[i]);
	
	std::queue<d2d::ISprite*> buffer;
	for (size_t i = 0, n = context->symbols.size(); i < n; ++i)
	{
		d2d::ISymbol* symbol = context->symbols[i];
		if (d2d::ComplexSymbol* complex = dynamic_cast<d2d::ComplexSymbol*>(symbol))
		{
			for (size_t j = 0, n = complex->m_sprites.size(); j < n; ++j)
				buffer.push(complex->m_sprites[j]);
		}
		else if (d2d::AnimSymbol* anim = dynamic_cast<d2d::AnimSymbol*>(symbol))
		{
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				d2d::AnimSymbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					d2d::AnimSymbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						buffer.push(frame->sprites[k]);
				}
			}
		}
	}
	
	while (!buffer.empty())
	{
		d2d::ISprite* sprite = buffer.front(); buffer.pop();
		if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(sprite))
		{
			m_setSymbols.insert(&sprite->getSymbol());
		}
		else if (const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(sprite))
		{
			m_setSymbols.insert(&sprite->getSymbol());
		}
		else if (const d2d::ComplexSprite* complex = dynamic_cast<const d2d::ComplexSprite*>(sprite))
		{
			const d2d::ComplexSymbol& symbol = complex->getSymbol();
			if (m_setSymbols.find(&symbol) == m_setSymbols.end())
			{
				m_setSymbols.insert(&symbol);
				for (size_t i = 0, n = symbol.m_sprites.size(); i < n; ++i)
				{
					d2d::ISprite* child = symbol.m_sprites[i];
					buffer.push(child);
				}
			}
		}
		else if (const d2d::AnimSprite* anim = dynamic_cast<const d2d::AnimSprite*>(sprite))
		{
			const d2d::AnimSymbol& symbol = anim->getSymbol();
			if (m_setSymbols.find(&symbol) == m_setSymbols.end())
			{
				m_setSymbols.insert(&symbol);
				for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
				{
					d2d::AnimSymbol::Layer* layer = symbol.m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
					{
						d2d::AnimSymbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
							buffer.push(frame->sprites[k]);
					}
				}
			}
		}
		else if (const d2d::Patch9Sprite* anim = dynamic_cast<const d2d::Patch9Sprite*>(sprite))
		{
			const d2d::Patch9Symbol& symbol = anim->getSymbol();
			if (m_setSymbols.find(&symbol) == m_setSymbols.end())
			{
				m_setSymbols.insert(&symbol);
				switch (symbol.type())
				{
				case d2d::Patch9Symbol::e_9Grid:
					for (size_t i = 0; i < 3; ++i)
						for (size_t j = 0; j < 3; ++j)
							buffer.push(symbol.m_sprites[i][j]);
					break;
				case d2d::Patch9Symbol::e_9GridHollow:
					for (size_t i = 0; i < 3; ++i) {
						for (size_t j = 0; j < 3; ++j) {
							if (i == 1 && j == 1) continue;
							buffer.push(symbol.m_sprites[i][j]);
						}
					}
					break;
				case d2d::Patch9Symbol::e_3GridHor:
					for (size_t i = 0; i < 3; ++i)
						buffer.push(symbol.m_sprites[1][i]);
					break;
				case d2d::Patch9Symbol::e_3GridVer:
					for (size_t i = 0; i < 3; ++i)
						buffer.push(symbol.m_sprites[i][1]);
					break;
				case d2d::Patch9Symbol::e_6GridUpper:
					for (size_t i = 1; i < 3; ++i)
						for (size_t j = 0; j < 3; ++j)
							buffer.push(symbol.m_sprites[i][j]);
					break;
				}
			}
		}
	}
}

void COCParser::prepareOutList()
{
	while (!m_setSymbols.empty())
	{
		std::set<const d2d::ISymbol*>::iterator itr = m_setSymbols.begin();
		for ( ; itr != m_setSymbols.end(); ++itr)
		{
			d2d::ISymbol* symbol = const_cast<d2d::ISymbol*>(*itr);
			if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
			{
				std::string path = symbol->getFilepath();
				m_outList.push_back(image);
				m_setSymbols.erase(itr);
				break;
			}
			else if (d2d::FontBlankSymbol* font = dynamic_cast<d2d::FontBlankSymbol*>(symbol))
			{
				m_outList.push_back(font);
				m_setSymbols.erase(itr);
				break;
			}
			else if (d2d::ComplexSymbol* complex = dynamic_cast<d2d::ComplexSymbol*>(symbol))
			{
				bool prepared = true;
				for (size_t i = 0, n = complex->m_sprites.size(); i < n && prepared; ++i)
					if (!isSymbolPrepared(complex->m_sprites[i]))
						prepared = false;
				if (prepared)
				{
					m_outList.push_back(complex);
					m_setSymbols.erase(itr);
					break;
				}
			}
			else if (d2d::AnimSymbol* anim = dynamic_cast<d2d::AnimSymbol*>(symbol))
			{
				bool prepared = true;
				for (size_t i = 0, n = anim->m_layers.size(); i < n && prepared; ++i)
				{
					d2d::AnimSymbol::Layer* layer = anim->m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m && prepared; ++j)
					{
						d2d::AnimSymbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l && prepared; ++k)
							if (!isSymbolPrepared(frame->sprites[k]))
								prepared = false;
					}
				}
				if (prepared)
				{
					m_outList.push_back(anim);
					m_setSymbols.erase(itr);
					break;
				}
			}
			else if (d2d::Patch9Symbol* patch9 = dynamic_cast<d2d::Patch9Symbol*>(symbol))
			{
				bool prepared = true;
				switch (patch9->type())
				{
				case d2d::Patch9Symbol::e_9Grid:
					for (size_t i = 0; i < 3 && prepared; ++i)
						for (size_t j = 0; j < 3 && prepared; ++j)
							if (!isSymbolPrepared(patch9->m_sprites[i][j]))
								prepared = false;
					break;
				case d2d::Patch9Symbol::e_9GridHollow:
					for (size_t i = 0; i < 3 && prepared; ++i) {
						for (size_t j = 0; j < 3 && prepared; ++j) {
							if (i == 1 && j == 1) continue;
							if (!isSymbolPrepared(patch9->m_sprites[i][j]))
								prepared = false;
						}
					}
					break;
				case d2d::Patch9Symbol::e_3GridHor:
					for (size_t i = 0; i < 3 && prepared; ++i)
						if (!isSymbolPrepared(patch9->m_sprites[1][i]))
							prepared = false;
					break;
				case d2d::Patch9Symbol::e_3GridVer:
					for (size_t i = 0; i < 3 && prepared; ++i)
						if (!isSymbolPrepared(patch9->m_sprites[i][1]))
							prepared = false;
					break;
				case d2d::Patch9Symbol::e_6GridUpper:
					for (size_t i = 1; i < 3 && prepared; ++i)
						for (size_t j = 0; j < 3 && prepared; ++j)
							if (!isSymbolPrepared(patch9->m_sprites[i][j]))
								prepared = false;
					break;
				}
				if (prepared)
				{
					m_outList.push_back(patch9);
					m_setSymbols.erase(itr);
					break;
				}
			}
		}
	}
}

void COCParser::prepareMapSymbolPic()
{
	const TexturesMgr& texMgr = Context::Instance()->texMgr;
	for (size_t i = 0; i < TexturesMgr::NUM; ++i)
	{
		if (!texMgr.textures[i])
			continue;
		const d2d::TexturePackerFileAdapter& adapter = texMgr.textures[i]->adapter;
		for (size_t j = 0, m = adapter.frames.size(); j < m; ++j)
		{
			const d2d::TexturePackerFileAdapter::Entry& entry = adapter.frames[j];

			// find symbol
			const d2d::ISymbol* symbol = NULL;
			for (size_t k = 0, l = m_outList.size(); k < l; ++k)
			{
				const d2d::ISymbol* s = m_outList[k];
				std::string filepath = s->getFilepath();
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

bool COCParser::isSymbolPrepared(const d2d::ISprite* sprite) const
{
	for (size_t i = 0, n = m_outList.size(); i < n; ++i)
		if (&sprite->getSymbol() == m_outList[i])
			return true;
	return false;
}
} // coceditor