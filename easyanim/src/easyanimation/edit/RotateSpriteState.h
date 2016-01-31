#ifndef _EASYANIM_ROTATE_SPRITE_STATE_H_
#define _EASYANIM_ROTATE_SPRITE_STATE_H_



namespace eanim
{

class RotateSpriteState : public ee::RotateSpriteState
{
public:
	RotateSpriteState(ee::SpriteSelection* selection, const ee::Vector& first_pos);

protected:
	virtual void Rotate(const ee::Vector& dst);

}; // RotateSpriteState

}

#endif // _EASYANIM_ROTATE_SPRITE_STATE_H_