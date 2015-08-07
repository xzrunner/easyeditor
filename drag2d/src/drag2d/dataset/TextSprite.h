#ifndef _DRAG2D_TEXT_SPRITE_H_
#define _DRAG2D_TEXT_SPRITE_H_

#include "ISprite.h"

#include "FontSymbol.h"

#include "common/Color.h"

namespace d2d
{

class TextSprite : public ISprite
{
public:
	TextSprite();
	TextSprite(const TextSprite& sprite);
	TextSprite(FontSymbol* symbol);
	virtual ~TextSprite();

	//
	// IObject interface
	//
	virtual TextSprite* Clone() const;

	//
	// ISprite interface
	//
	virtual bool Update(int version) { return false; }
	virtual const FontSymbol& GetSymbol() const;
	virtual void SetSymbol(ISymbol* symbol);

	virtual IPropertySetting* CreatePropertySetting(EditPanelImpl* stage);

	virtual void SetTransform(const Vector& position, float angle);

	void setText(const std::string& text);
	const std::string& getText() const {
		return m_text;
	}

	void setSize(int size);
	int getSize() const { return m_size; }

	void setColor(const d2d::Colori& color) { m_color = color; }
	const d2d::Colori& getColor() const { return m_color; }

protected:
	FontSymbol* m_symbol;

private:
	std::string m_text;

	int m_size;

	d2d::Colori m_color;

}; // TextSprite

}

#endif // _DRAG2D_TEXT_SPRITE_H_