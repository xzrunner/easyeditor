#ifndef _EASYANIM_SYMBOL_H_
#define _EASYANIM_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/AnimSymbol.h>

#include <vector>

namespace eanim
{

class LayersLoader;
class Sprite;

class Symbol : public ee::Symbol, public s2::AnimSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const { return s2::AnimSymbol::GetBounding(spr); }

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor);

	size_t getMaxFrameIndex() const;

	void setFrameIndex(int index) {
		m_index = index;
	}

	int getFPS() const { return m_fps; }
	void setFPS(int fps) { m_fps = fps; }

	void LoadFromFile(const LayersLoader& loader);

	int GetCurrFrame() const;

	void SetLoop(bool loop) { m_loop = loop; }
	void ResetTime() { m_init_time = 0; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	int m_fps;

	int m_index; // for draw certain frame

	mutable long m_init_time;

	bool m_loop;

}; // Symbol

}

#endif // _EASYANIM_SYMBOL_H_