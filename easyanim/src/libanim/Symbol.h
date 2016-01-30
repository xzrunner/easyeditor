#ifndef _LIBANIMATION_SYMBOL_H_
#define _LIBANIMATION_SYMBOL_H_

#include <drag2d.h>

namespace libanim
{

class LayersLoader;

class Symbol : public d2d::Symbol
{
public:
	struct Frame
	{
//		int id;
		int index;
		std::vector<d2d::Sprite*> sprites;
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
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	size_t getMaxFrameIndex() const;

	void setFrameIndex(int index) {
		m_index = index;
	}

	int getFPS() const { return m_fps; }
	void setFPS(int fps) { m_fps = fps; }

	void InitBounding();

	void LoadFromFile(const LayersLoader& loader);

	static d2d::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void clear();

public:
	std::vector<Layer*> m_layers;

private:
	d2d::Rect m_rect;

	int m_fps;

	int m_index; // for draw certain frame

}; // Symbol

}

#endif // _LIBANIMATION_SYMBOL_H_