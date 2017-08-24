#ifndef _EASYAUDIO_SYMBOL_H_
#define _EASYAUDIO_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/AudioSymbol.h>

namespace eaudio
{

class Sprite;

class Symbol : public ee::Symbol, public s2::AudioSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual s2::RenderReturn Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;
	virtual bool LoadResources();

private:
	void DrawBackground(const Sprite* spr, const S2_MAT& mt) const;

	bool LoadResourcesStatic();
	bool LoadResourcesStream();

}; // Symbol

}

#endif // _EASYAUDIO_SYMBOL_H_