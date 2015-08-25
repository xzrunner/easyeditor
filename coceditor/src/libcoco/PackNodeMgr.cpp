#include "PackNodeMgr.h"

#include <easyscale9.h>

namespace libcoco
{

PackNodeMgr* PackNodeMgr::m_instance = NULL;

const IPackNode* PackNodeMgr::QueryScale9(const escale9::Sprite* scale9) const
{
	for (int i = 0, n = m_map_scale9.size(); i < n; ++i)
	{
		const escale9::Sprite* spr = m_map_scale9[i].first;
		if (&scale9->GetSymbol() == &spr->GetSymbol())
		{
			float w0, h0;
			scale9->GetSize(w0, h0);

			float w1, h1;
			spr->GetSize(w1, h1);

			if (w0 == w1 && h0 == h1) {
				return m_map_scale9[i].second;
			}
		}
	}
	return NULL;
}

void PackNodeMgr::InsertScale9(const escale9::Sprite* spr, const IPackNode* node)
{
	m_map_scale9.push_back(std::make_pair(spr, node));
}

const IPackNode* PackNodeMgr::QueryFont(const d2d::FontSprite* font) const
{
	for (int i = 0, n = m_map_font.size(); i < n; ++i) 
	{
		const d2d::FontSprite* spr = m_map_font[i].first;
		if (font->name == spr->name &&
			font->font == spr->font &&
			font->align_hori == spr->align_hori &&
			font->align_vert == spr->align_vert && 
			font->color == spr->color &&
			font->width == spr->width &&
			font->height == spr->height &&
			font->has_edge == spr->has_edge) {
			return m_map_font[i].second;
		}
	}
	return NULL;
}

void PackNodeMgr::InsertScale9(const d2d::FontSprite* font, const IPackNode* node)
{
	m_map_font.push_back(std::make_pair(font, node));
}

void PackNodeMgr::InsertImage(const d2d::ISprite* spr, const IPackNode* node)
{
	m_map_spr.push_back(std::make_pair(spr, node));
}

const IPackNode* PackNodeMgr::QuerySymbol(const d2d::ISymbol* symbol) const
{
	std::map<const d2d::ISymbol*, const IPackNode*>::const_iterator itr 
		= m_map_symbol.find(symbol);
	if (itr != m_map_symbol.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

void PackNodeMgr::InsertSymbol(const d2d::ISymbol* symbol, const IPackNode* node)
{
	m_map_symbol.insert(std::make_pair(symbol, node));
}

PackNodeMgr* PackNodeMgr::Instance()
{
	if (!m_instance) {
		m_instance = new PackNodeMgr();
	}
	return m_instance;
}

}