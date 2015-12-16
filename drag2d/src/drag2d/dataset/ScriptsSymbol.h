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
		virtual ScriptsSymbol* Clone() const { return NULL; }

		//
		// ISymbol interfaces
		//
		virtual void ReloadTexture() const {}
		virtual void Draw(const Matrix& mt, const ColorTrans& col = ColorTrans(), 
			const ISprite* spr = NULL, const ISprite* root = NULL) const {}
		virtual Rect GetSize(const ISprite* sprite = NULL) const;

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