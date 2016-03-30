#ifndef _EASYANIM_SYMBOL_H_
#define _EASYANIM_SYMBOL_H_

#include <ee/Symbol.h>

#include <vector>

namespace eanim
{

class LayersLoader;

class Symbol : public ee::Symbol
{
public:
	struct Frame
	{
//		int id;
		int index;
		std::vector<ee::Sprite*> sprites;
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
	virtual void Draw(const ee::RenderParams& trans, const ee::Sprite* spr = NULL, 
		const ee::Sprite* root = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	size_t getMaxFrameIndex() const;

	void setFrameIndex(int index) {
		m_index = index;
	}

	int getFPS() const { return m_fps; }
	void setFPS(int fps) { m_fps = fps; }

	void InitBounding();

	void LoadFromFile(const LayersLoader& loader);

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void clear();

public:
	std::vector<Layer*> m_layers;

private:
	ee::Rect m_rect;

	int m_fps;

	int m_index; // for draw certain frame

}; // Symbol

}

#endif // _EASYANIM_SYMBOL_H_