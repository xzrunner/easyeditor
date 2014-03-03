
#ifndef ESHAPE_LIBRARY_ITEM_H
#define ESHAPE_LIBRARY_ITEM_H

#include <drag2d.h>

namespace eshape
{
	class LibraryItem : public d2d::ISymbol, public d2d::UserDataImpl
	{
	public:
		LibraryItem();
		LibraryItem(const wxString& filename);
		virtual ~LibraryItem();

		//
		// d2d::ICloneable interface
		//	
		virtual LibraryItem* clone() { return NULL; }

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr);

		//
		// d2d::ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const d2d::ISprite* sprite = NULL) const;
		virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	protected:
		virtual void loadResources();

	private:
		d2d::ISymbol* m_symbol;

	}; // LibraryItem
}

#endif // ESHAPE_LIBRARY_ITEM_H