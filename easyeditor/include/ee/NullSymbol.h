#ifndef _EASYEDITOR_NULL_SYMBOL_H_
#define _EASYEDITOR_NULL_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class NullSymbol : public Symbol
{
public:
	NullSymbol(const std::string& filename) { m_filepath = filename; }
	NullSymbol(const std::string& filename, int width, int height)
		: m_size(static_cast<float>(width), static_cast<float>(height)) { m_filepath = filename; }

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interface
	//	
	virtual void Draw(const s2::RenderParams& trans, const s2::Sprite* spr = NULL) const {}
	virtual void ReloadTexture() const {}
	virtual Rect GetSize(const Sprite* sprite = NULL) const {
		return m_size;
	}

protected:
	virtual void LoadResources() {}

private:
	Rect m_size;

}; // NullSymbol

}

#endif // _EASYEDITOR_NULL_SYMBOL_H_