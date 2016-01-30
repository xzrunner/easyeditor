#ifndef _EASYICON_SYMBOL_H_
#define _EASYICON_SYMBOL_H_

#include <drag2d.h>

namespace eicon
{

class Icon;

class Symbol : public d2d::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	static d2d::Symbol* Create() { return new Symbol(); }

	void SetIcon(Icon* icon);
	const Icon* GetIcon() const { return m_icon; }
	Icon* GetIcon() { return m_icon; }

	void SetImage(d2d::Image* img);

protected:
	virtual void LoadResources();

private:
	Icon* m_icon;

}; // Symbol

}

#endif // _EASYICON_SYMBOL_H_