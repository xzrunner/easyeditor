#ifndef _EASYSHADOW_SYMBOL_H_
#define _EASYSHADOW_SYMBOL_H_

#include <ee/Symbol.h>

namespace eshadow
{

class Shadow;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const ee::Matrix& mt, const ee::ColorTrans& color = ee::ColorTrans(), 
		const ee::Sprite* spr = NULL, const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	Shadow* GetShadow() { return m_shadow; }
	const Shadow* GetShadow() const { return m_shadow; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Shadow* m_shadow;

}; // Symbol

}

#endif // _EASYSHADOW_SYMBOL_H_