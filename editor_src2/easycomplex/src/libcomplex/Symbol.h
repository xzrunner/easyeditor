#ifndef COMPLEX_SYMBOL_H
#define COMPLEX_SYMBOL_H

#include <drag2d.h>

namespace ecomplex
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const { return NULL; }

	//
	// ISerializable interface
	//
	virtual void loadFromTextFile(std::ifstream& fin);
	virtual void storeToTextFile(std::ofstream& fout) const;

	//
	// ISymbol interfaces
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	//
	// ListItem interface
	//
	virtual void refresh();

	bool isOneLayer() const;

protected:
	virtual void loadResources();

private:
	void clear();

	void initBounding();

	void refreshThumbnail();

private:
	static const float SCALE;

public:
	struct Group
	{
		std::string name;
		std::vector<d2d::ISprite*> members;
	};

	// todo: 
public:
	std::vector<d2d::ISprite*> m_sprites;

	std::vector<Group> m_groups;

	d2d::Rect m_rect;

	d2d::Rect m_clipbox;

}; // Symbol

}

#endif // COMPLEX_SYMBOL_H