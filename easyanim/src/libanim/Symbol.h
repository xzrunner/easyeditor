#ifndef _EASYANIM_SYMBOL_H_
#define _EASYANIM_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/AnimSymbol.h>

#include <vector>

namespace eanim
{

class LayersLoader;

class Symbol : public ee::Symbol
{
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
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;

	size_t getMaxFrameIndex() const;

	void setFrameIndex(int index) {
		m_index = index;
	}

	int getFPS() const { return m_fps; }
	void setFPS(int fps) { m_fps = fps; }

	void InitBounding();

	void LoadFromFile(const LayersLoader& loader);

	const std::vector<s2::AnimSymbol::Layer*>& GetLayers() const { return m_core->GetLayers(); }
	void AddLayer(s2::AnimSymbol::Layer* layer) { m_core->AddLayer(layer); }

	int GetCurrFrame() const;

	void SetLoop(bool loop) { m_loop = loop; }
	void ResetTime() { m_init_time = 0; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void Clear();

private:
	s2::AnimSymbol* m_core;

	sm::rect m_rect;

	int m_fps;

	int m_index; // for draw certain frame

	mutable long m_init_time;

	bool m_loop;

}; // Symbol

}

#endif // _EASYANIM_SYMBOL_H_