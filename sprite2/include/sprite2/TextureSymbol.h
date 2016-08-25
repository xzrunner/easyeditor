#ifndef _SPRITE2_TEXTURE_SYMBOL_H_
#define _SPRITE2_TEXTURE_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

#include <stdint.h>

namespace s2
{

class PolygonShape;

class TextureSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TextureSymbol();
	virtual ~TextureSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const std::vector<PolygonShape*>& GetPolygons() const { return m_polygons; }
	std::vector<PolygonShape*>& GetPolygons() { return m_polygons; }

protected:
	void Clear();

protected:
	std::vector<PolygonShape*> m_polygons;

}; // TextureSymbol

}

#endif // _SPRITE2_TEXTURE_SYMBOL_H_