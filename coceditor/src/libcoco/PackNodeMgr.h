#ifndef _LIBCOCO_PACK_NODE_MGR_H_
#define _LIBCOCO_PACK_NODE_MGR_H_

#include "IPackNode.h"

namespace escale9 { class Sprite; }

#include <drag2d.h>

namespace libcoco
{

class PackNodeMgr
{
public:
	// picture
	const IPackNode* QueryScale9(const escale9::Sprite* scale9) const;
	void InsertScale9(const escale9::Sprite* spr, const IPackNode* node);

	// label
	const IPackNode* QueryFont(const d2d::FontSprite* font) const;
	void InsertScale9(const d2d::FontSprite* font, const IPackNode* node);		

	// common
	void InsertImage(const d2d::ISprite* spr, const IPackNode* node);

	const IPackNode* QuerySymbol(const d2d::ISymbol* symbol) const;
	void InsertSymbol(const d2d::ISymbol* symbol, const IPackNode* node);

	static PackNodeMgr* Instance();

private:
	PackNodeMgr() {}

private:
	std::vector<std::pair<const escale9::Sprite*, const IPackNode*> > m_map_scale9;

	std::vector<std::pair<const d2d::FontSprite*, const IPackNode*> > m_map_font;

	std::vector<std::pair<const d2d::ISprite*, const IPackNode*> > m_map_spr;

	std::map<const d2d::ISymbol*, const IPackNode*> m_map_symbol;

private:
	static PackNodeMgr* m_instance;

}; // PackNodeMgr

}

#endif // _LIBCOCO_PACK_NODE_MGR_H_