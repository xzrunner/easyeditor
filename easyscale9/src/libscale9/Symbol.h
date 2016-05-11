#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <ee/Symbol.h>

#include "Scale9Data.h"

namespace escale9
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& symbol);

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return new Symbol(*this); }

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	void ResizeScale9(float width, float height);
	void ComposeFromSprites(ee::Sprite* sprites[3][3], float width, float height);

	const Scale9Data& GetScale9Data() const { return m_data; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Scale9Data m_data;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_