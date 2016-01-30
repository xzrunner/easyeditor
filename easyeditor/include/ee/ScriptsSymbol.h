#ifndef D2D_SCRIPTS_SYMBOL_H
#define D2D_SCRIPTS_SYMBOL_H

#include "Symbol.h"

namespace d2d
{
	class ScriptsSymbol : public Symbol
	{
	public:
		ScriptsSymbol() {}
		virtual ~ScriptsSymbol() {}

		//
		// Cloneable interface
		//
		virtual ScriptsSymbol* Clone() const { return NULL; }

		//
		// Symbol interfaces
		//
		virtual void ReloadTexture() const {}
		virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
			const Sprite* spr = NULL, const Sprite* root = NULL) const {}
		virtual Rect GetSize(const Sprite* sprite = NULL) const;

		//
		// ListItem interface
		//
		virtual void refresh() {}

		const std::string getContent() const { return m_data; }

	protected:
		virtual void LoadResources();

	private:
		std::string m_data;

	}; // ScriptsSymbol
}

#endif // D2D_SCRIPTS_SYMBOL_H