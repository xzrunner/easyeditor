#ifndef _EASYSCALE_COMPOSE_IMPL_H_
#define _EASYSCALE_COMPOSE_IMPL_H_

#include <drag2d.h>

namespace escale9
{

class StagePanel;

class ComposeImpl : public d2d::ArrangeSpriteImpl
{
public:
	ComposeImpl(StagePanel* stage, d2d::PropertySettingPanel* property);

	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseDrag(int x, int y) { return; }

private:
	StagePanel* m_stage;

}; // ComposeImpl

}

#endif // _EASYSCALE_COMPOSE_IMPL_H_