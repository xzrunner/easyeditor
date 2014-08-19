#ifndef _EASYANIM_ROTATE_SPRITE_STATE_H_
#define _EASYANIM_ROTATE_SPRITE_STATE_H_

#include <drag2d.h>

namespace eanim
{

class StagePanel;

class RotateSpriteState : public d2d::RotateSpriteState
{
public:
	RotateSpriteState(StagePanel* stage, d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos);

protected:
	virtual void Rotate(const d2d::Vector& dst);

private:
	StagePanel* m_stage;

}; // RotateSpriteState

}

#endif // _EASYANIM_ROTATE_SPRITE_STATE_H_