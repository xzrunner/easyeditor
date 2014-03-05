#include "SymbolsPrepare.h"

#include <queue>

#include <easycomplex.h>
#include <easyanim.h>

namespace libcoco
{

void SymbolsPrepare::prepare(const std::vector<const d2d::ISymbol*>& symbols)
{
	fetch(symbols);
	sort();
}

void SymbolsPrepare::prepare(const std::vector<const d2d::ISprite*>& sprites)
{
	std::vector<const d2d::ISymbol*> symbols;
	symbols.reserve(sprites.size());
	for (int i = 0, n = sprites.size(); i < n; ++i)
		symbols.push_back(&sprites[i]->getSymbol());
	prepare(symbols);
}

void SymbolsPrepare::fetch(const std::vector<const d2d::ISymbol*>& symbols)
{
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
		m_unique.insert(symbols[i]);

	std::queue<d2d::ISprite*> buffer;
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		const d2d::ISymbol* symbol = symbols[i];
		if (const complex::Symbol* complex = dynamic_cast<const complex::Symbol*>(symbol))
		{
			for (size_t j = 0, n = complex->m_sprites.size(); j < n; ++j)
				buffer.push(complex->m_sprites[j]);
		}
		else if (const anim::Symbol* anim = dynamic_cast<const anim::Symbol*>(symbol))
		{
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				anim::Symbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					anim::Symbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						buffer.push(frame->sprites[k]);
				}
			}
		}
		else if (const d2d::Scale9Symbol* patch = dynamic_cast<const d2d::Scale9Symbol*>(symbol))
		{
			switch (patch->type())
			{
			case d2d::Scale9Symbol::e_9Grid:
				for (size_t i = 0; i < 3; ++i)
					for (size_t j = 0; j < 3; ++j)
						buffer.push(patch->m_sprites[i][j]);
				break;
			case d2d::Scale9Symbol::e_9GridHollow:
				for (size_t i = 0; i < 3; ++i) {
					for (size_t j = 0; j < 3; ++j) {
						if (i == 1 && j == 1) continue;
						buffer.push(patch->m_sprites[i][j]);
					}
				}
				break;
			case d2d::Scale9Symbol::e_3GridHor:
				for (size_t i = 0; i < 3; ++i)
					buffer.push(patch->m_sprites[1][i]);
				break;
			case d2d::Scale9Symbol::e_3GridVer:
				for (size_t i = 0; i < 3; ++i)
					buffer.push(patch->m_sprites[i][1]);
				break;
			case d2d::Scale9Symbol::e_6GridUpper:
				for (size_t i = 1; i < 3; ++i)
					for (size_t j = 0; j < 3; ++j)
						buffer.push(patch->m_sprites[i][j]);
				break;
			}
		}
	}

	while (!buffer.empty())
	{
		d2d::ISprite* sprite = buffer.front(); buffer.pop();
		if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(sprite))
		{
			m_unique.insert(&sprite->getSymbol());
		}
		else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(sprite))
		{
			m_unique.insert(&sprite->getSymbol());
		}
		else if (const complex::Sprite* complex = dynamic_cast<const complex::Sprite*>(sprite))
		{
			const complex::Symbol& symbol = complex->getSymbol();
			if (m_unique.find(&symbol) == m_unique.end())
			{
				m_unique.insert(&symbol);
				for (size_t i = 0, n = symbol.m_sprites.size(); i < n; ++i)
				{
					d2d::ISprite* child = symbol.m_sprites[i];
					buffer.push(child);
				}
			}
		}
		else if (const anim::Sprite* anim = dynamic_cast<const anim::Sprite*>(sprite))
		{
			const anim::Symbol& symbol = anim->getSymbol();
			if (m_unique.find(&symbol) == m_unique.end())
			{
				m_unique.insert(&symbol);
				for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
				{
					anim::Symbol::Layer* layer = symbol.m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
					{
						anim::Symbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
							buffer.push(frame->sprites[k]);
					}
				}
			}
		}
		else if (const d2d::Scale9Sprite* anim = dynamic_cast<const d2d::Scale9Sprite*>(sprite))
		{
			const d2d::Scale9Symbol& symbol = anim->getSymbol();
			if (m_unique.find(&symbol) == m_unique.end())
			{
				m_unique.insert(&symbol);
				switch (symbol.type())
				{
				case d2d::Scale9Symbol::e_9Grid:
					for (size_t i = 0; i < 3; ++i)
						for (size_t j = 0; j < 3; ++j)
							buffer.push(symbol.m_sprites[i][j]);
					break;
				case d2d::Scale9Symbol::e_9GridHollow:
					for (size_t i = 0; i < 3; ++i) {
						for (size_t j = 0; j < 3; ++j) {
							if (i == 1 && j == 1) continue;
							buffer.push(symbol.m_sprites[i][j]);
						}
					}
					break;
				case d2d::Scale9Symbol::e_3GridHor:
					for (size_t i = 0; i < 3; ++i)
						buffer.push(symbol.m_sprites[1][i]);
					break;
				case d2d::Scale9Symbol::e_3GridVer:
					for (size_t i = 0; i < 3; ++i)
						buffer.push(symbol.m_sprites[i][1]);
					break;
				case d2d::Scale9Symbol::e_6GridUpper:
					for (size_t i = 1; i < 3; ++i)
						for (size_t j = 0; j < 3; ++j)
							buffer.push(symbol.m_sprites[i][j]);
					break;
				}
			}
		}
	}
}

void SymbolsPrepare::sort()
{
	while (!m_unique.empty())
	{
		std::set<const d2d::ISymbol*>::iterator itr = m_unique.begin();
		for ( ; itr != m_unique.end(); ++itr)
		{
			d2d::ISymbol* symbol = const_cast<d2d::ISymbol*>(*itr);
			if (d2d::ImageSymbol* image = dynamic_cast<d2d::ImageSymbol*>(symbol))
			{
				std::string path = symbol->getFilepath();
				m_result.push_back(image);
				m_unique.erase(itr);
				break;
			}
			else if (d2d::FontBlankSymbol* font = dynamic_cast<d2d::FontBlankSymbol*>(symbol))
			{
				m_result.push_back(font);
				m_unique.erase(itr);
				break;
			}
			else if (complex::Symbol* complex = dynamic_cast<complex::Symbol*>(symbol))
			{
				bool prepared = true;
				for (size_t i = 0, n = complex->m_sprites.size(); i < n && prepared; ++i)
					if (!isSymbolPrepared(complex->m_sprites[i]))
						prepared = false;
				if (prepared)
				{
					m_result.push_back(complex);
					m_unique.erase(itr);
					break;
				}
			}
			else if (anim::Symbol* anim = dynamic_cast<anim::Symbol*>(symbol))
			{
				bool prepared = true;
				for (size_t i = 0, n = anim->m_layers.size(); i < n && prepared; ++i)
				{
					anim::Symbol::Layer* layer = anim->m_layers[i];
					for (size_t j = 0, m = layer->frames.size(); j < m && prepared; ++j)
					{
						anim::Symbol::Frame* frame = layer->frames[j];
						for (size_t k = 0, l = frame->sprites.size(); k < l && prepared; ++k)
							if (!isSymbolPrepared(frame->sprites[k]))
								prepared = false;
					}
				}
				if (prepared)
				{
					m_result.push_back(anim);
					m_unique.erase(itr);
					break;
				}
			}
			else if (d2d::Scale9Symbol* patch9 = dynamic_cast<d2d::Scale9Symbol*>(symbol))
			{
				bool prepared = true;
				switch (patch9->type())
				{
				case d2d::Scale9Symbol::e_9Grid:
					for (size_t i = 0; i < 3 && prepared; ++i)
						for (size_t j = 0; j < 3 && prepared; ++j)
							if (!isSymbolPrepared(patch9->m_sprites[i][j]))
								prepared = false;
					break;
				case d2d::Scale9Symbol::e_9GridHollow:
					for (size_t i = 0; i < 3 && prepared; ++i) {
						for (size_t j = 0; j < 3 && prepared; ++j) {
							if (i == 1 && j == 1) continue;
							if (!isSymbolPrepared(patch9->m_sprites[i][j]))
								prepared = false;
						}
					}
					break;
				case d2d::Scale9Symbol::e_3GridHor:
					for (size_t i = 0; i < 3 && prepared; ++i)
						if (!isSymbolPrepared(patch9->m_sprites[1][i]))
							prepared = false;
					break;
				case d2d::Scale9Symbol::e_3GridVer:
					for (size_t i = 0; i < 3 && prepared; ++i)
						if (!isSymbolPrepared(patch9->m_sprites[i][1]))
							prepared = false;
					break;
				case d2d::Scale9Symbol::e_6GridUpper:
					for (size_t i = 1; i < 3 && prepared; ++i)
						for (size_t j = 0; j < 3 && prepared; ++j)
							if (!isSymbolPrepared(patch9->m_sprites[i][j]))
								prepared = false;
					break;
				}
				if (prepared)
				{
					m_result.push_back(patch9);
					m_unique.erase(itr);
					break;
				}
			}
		}
	}
}

bool SymbolsPrepare::isSymbolPrepared(const d2d::ISprite* sprite) const
{
	for (size_t i = 0, n = m_result.size(); i < n; ++i)
		if (&sprite->getSymbol() == m_result[i])
			return true;
	return false;
}

}