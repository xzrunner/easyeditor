#ifndef _SPRITE2_STATIC_QUAD_ICON_H_
#define _SPRITE2_STATIC_QUAD_ICON_H_

#include "Icon.h"

namespace s2
{

class StaticQuadIcon : public VIRTUAL_INHERITANCE Icon
{
public:
	StaticQuadIcon();
	StaticQuadIcon(const StaticQuadIcon& icon);
	StaticQuadIcon& operator = (const StaticQuadIcon& icon);

	virtual void Draw(const RenderParams& params, float process) const;

	virtual sm::rect GetRegion(float process) const;

	const sm::vec2* GetScreen() const { return m_screen; }
	void SetScreen(const sm::vec2* screen);

protected:
	virtual void GetQuad(float process, sm::vec2 quad[4]) const;

	virtual void Update();

protected:
	sm::vec2 m_src[4], m_screen[4];

}; // StaticQuadIcon

}

#endif // _SPRITE2_STATIC_QUAD_ICON_H_