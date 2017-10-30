#ifndef _EASYEDITOR_SPRITE_POOL_H_
#define _EASYEDITOR_SPRITE_POOL_H_

#include <cu/cu_macro.h>

#include <vector>
#include <map>

namespace ee
{

class Sprite;
class Symbol;

class SpritePool
{
public:
	void Insert(const Sprite& spr);
	void Remove(const Sprite& spr);

	void UpdateBoundings(const Symbol& sym);

private:
	using SpriteList = std::vector<const Sprite*>;
	std::map<const Symbol*, SpriteList> m_map_symbol2sprites;

	CU_SINGLETON_DECLARATION(SpritePool);

}; // SpritePool

}

#endif // _EASYEDITOR_SPRITE_POOL_H_