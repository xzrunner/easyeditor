#ifndef _ANIMATION_SYMBOL_H_
#define _ANIMATION_SYMBOL_H_

#include <drag2d.h>

namespace anim
{

class Symbol : public d2d::ISymbol
{
public:
	struct Frame
	{
//		int id;
		int index;
		std::vector<d2d::ISprite*> sprites;
		bool bClassicTween;
	};

	struct Layer
	{
		std::string name;
		std::vector<Frame*> frames;

		Frame* getCurrFrame(int index) const;
		Frame* getNextFrame(int index) const;
	};

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

	size_t getMaxFrameIndex() const;

protected:
	virtual void loadResources();

private:
	void clear();

	void initBounding();

	void refreshThumbnail();

public:
	std::vector<Layer*> m_layers;

	d2d::Rect m_rect;

	int m_fps;

}; // Symbol

}

#endif // _ANIMATION_SYMBOL_H_