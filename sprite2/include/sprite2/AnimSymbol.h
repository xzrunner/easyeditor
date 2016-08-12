#ifndef _SPRITE2_ANIM_SYMBOL_H_
#define _SPRITE2_ANIM_SYMBOL_H_

#include "S2_Symbol.h"

#include <vector>

namespace s2
{

class AnimSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	class Frame
	{
	public:
		int index;
		std::vector<Sprite*> sprites;
		bool tween;

	public:
		Frame() : index(0), tween(false) {}

	}; // Frame

	class Layer
	{
	public:
		std::string name;
		std::vector<Frame*> frames;

	public:
		Frame* GetCurrFrame(int index) const;
		Frame* GetNextFrame(int index) const;

	}; // Layer

public:
	AnimSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	const std::vector<Layer*>& GetLayers() const { return m_layers; }

	/************************************************************************/
	/* api for dynamic change                                               */
	/************************************************************************/
	void AddLayer(Layer* layer);
	bool Clear();

protected:
	std::vector<Layer*> m_layers;

}; // AnimSymbol

}

#endif // _SPRITE2_ANIM_SYMBOL_H_