#ifndef _EASYEDITOR_DUMMY_SYMBOL_H_
#define _EASYEDITOR_DUMMY_SYMBOL_H_

#include "Symbol.h"

namespace ee
{

class DummySymbol : public Symbol
{
public:
	DummySymbol(const std::string& filename) { m_filepath = filename; }
	DummySymbol(const std::string& filename, int width, int height)
		: m_size(sm::vec2(0, 0), static_cast<float>(width), static_cast<float>(height)) { m_filepath = filename; }

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interface
	//	
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const {}
	virtual sm::rect GetSize(const Sprite* sprite = NULL) const {
		return m_size;
	}

protected:
	virtual void LoadResources() {}

private:
	sm::rect m_size;

}; // DummySymbol

}

#endif // _EASYEDITOR_DUMMY_SYMBOL_H_