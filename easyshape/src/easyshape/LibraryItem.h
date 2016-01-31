#ifndef _ESHAPE_LIBRARY_ITEM_H_
#define _ESHAPE_LIBRARY_ITEM_H_



namespace eshape
{

class LibraryItem : public ee::ISymbol
{
public:
	LibraryItem();
	LibraryItem(const wxString& filename);
	virtual ~LibraryItem();

	//
	// ee::ICloneable interface
	//	
	virtual LibraryItem* clone() { return NULL; }

	//
	// UserDataImpl interface
	//	
	virtual void clearUserData(bool deletePtr);

	//
	// ee::ISymbol interface
	//
	virtual void reloadTexture() const;
	virtual void draw(const ee::Matrix& mt,
		const ee::Colorf& mul = ee::Colorf(1, 1, 1, 1), 
		const ee::Colorf& add = ee::Colorf(0, 0, 0, 0),
		const ee::ISprite* sprite = NULL) const;
	virtual ee::Rect getSize(const ee::ISprite* sprite = NULL) const;

protected:
	virtual void loadResources();

private:
	ee::ISymbol* m_symbol;

}; // LibraryItem

}

#endif // _ESHAPE_LIBRARY_ITEM_H_