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
		virtual ISymbol* clone() const { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin) {}
		virtual void storeToTextFile(std::ofstream& fout) const {}

		//
		// ISymbol interface
		//	
		virtual void reloadTexture() const {}
		virtual void draw(const Colorf& mul = Colorf(1, 1, 1, 1), 
			const Colorf& add = Colorf(0, 0, 0, 0),
			const ISprite* sprite = NULL) const {}
		virtual Rect getSize(const ISprite* sprite = NULL) const {
			return m_size;
		}

	protected:
		virtual void loadResources() {}

	private:
		Rect m_size;

	}; // NullSymbol
}