#ifndef _EASYANIM_TRANSLATE_SPRITE_STATE_H_
#define _EASYANIM_TRANSLATE_SPRITE_STATE_H_

#include <drag2d.h>

namespace eanim
{

class StagePanel;

class TranslateSpriteState : public d2d::TranslateSpriteState
{
public:
	TranslateSpriteState(StagePanel* stage, d2d::SpriteSelection* selection, 
		const d2d::Vector& first_pos);
	
protected:
	virtual void Translate(const d2d::Vector& offset);

private:
	StagePanel* m_stage;

}; // TranslateSpriteState 

}

#endif // _EASYANIM_TRANSLATE_SPRITE_STATE_H_