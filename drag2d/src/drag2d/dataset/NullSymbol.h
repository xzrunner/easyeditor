#ifndef _DRAG2D_NULL_SYMBOL_H_
#define _DRAG2D_NULL_SYMBOL_H_

#include "ISymbol.h"

namespace d2d
{
class NullSymbol : public ISymbol
{
public:
	NullSymbol(const std::string& filename) { m_filepath = filename; }
	NullSymbol(const std::string& filename, int width, int height)
		: m_size(width, height) { m_filepath = filename; }

	//
	// IObject interface
	//	
	virtual ISymbol* Clone() const { return NULL; }

	//
	// ISymbol interface
	//	
	virtual void ReloadTexture() const {}
	virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
		const ISprite* spr = NULL, const ISprite* root = NULL) const {}
	virtual Rect GetSize(const ISprite* sprite = NULL) const {
		return m_size;
	}

protected:
	virtual void LoadResources() {}

private:
	Rect m_size;

}; // NullSymbol
}

#endif // _DRAG2D_NULL_SYMBOL_H_