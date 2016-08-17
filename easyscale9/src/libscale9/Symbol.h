#ifndef _EASYSCALE9_SYMBOL_H_
#define _EASYSCALE9_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Scale9Symbol.h>

#include "Scale9Data.h"

namespace escale9
{

class Symbol : public ee::Symbol, public s2::Scale9Symbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

	void ResizeScale9(float width, float height);
	void ComposeFromSprites(ee::Sprite* sprs[3][3], float width, float height);

	const Scale9Data& GetScale9Data() const { return m_data; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Scale9Data m_data;

}; // Symbol

}

#endif // _EASYSCALE9_SYMBOL_H_