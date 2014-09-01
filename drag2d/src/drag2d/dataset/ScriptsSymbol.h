#ifndef D2D_SCRIPTS_SYMBOL_H
#define D2D_SCRIPTS_SYMBOL_H

#include "ISymbol.h"

namespace d2d
{
	class ScriptsSymbol : public ISymbol
	{
	public:
		ScriptsSymbol() {}
		virtual ~ScriptsSymbol() {}

		//
		// ICloneable interface
		//
		virtual ScriptsSymbol* clone() const { return NULL; }

		//
		// ISymbol interfaces
		//
		virtual void reloadTexture() const {}
		virtual void draw(const Screen& scr,
			const Matrix& mt,
			const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const ISprite* sprite = NULL) const {}
		virtual Rect getSize(const ISprite* sprite = NULL) const { return Rect(0, 0); }

		//
		// ListItem interface
		//
		virtual void refresh() {}

		const std::string getContent() const { return m_data; }

	protected:
		virtual void loadResources();

	private:
		std::string m_data;

	}; // ScriptsSymbol
}

#endif // D2D_SCRIPTS_SYMBOL_H