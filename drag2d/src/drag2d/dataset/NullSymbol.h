#pragma once

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
		virtual void Draw(const Matrix& mt,
			const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const Colorf& r_trans = Colorf(1, 0, 0, 0),
			const Colorf& g_trans = Colorf(0, 1, 0, 0),
			const Colorf& b_trans = Colorf(0, 0, 1, 0),
			const ISprite* sprite = NULL) const {}
		virtual Rect GetSize(const ISprite* sprite = NULL) const {
			return m_size;
		}

	protected:
		virtual void LoadResources() {}

	private:
		Rect m_size;

	}; // NullSymbol
}