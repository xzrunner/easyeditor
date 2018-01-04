#ifndef _EASYCOMPLEX3D_SELECT_SPRITE_OP_H_
#define _EASYCOMPLEX3D_SELECT_SPRITE_OP_H_

#include <ee/EditOP.h>
#include <ee/Sprite.h>

namespace ee { class SpriteSelection; }

namespace ecomplex3d
{

class StagePanel;

class SelectSpriteOP : public ee::EditOP
{
public:	
	SelectSpriteOP(StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;

	virtual bool OnDraw() const override;

	const ee::SpriteSelection& GetSelection() const {
		return *m_selection;
	}
	ee::SpriteSelection& GetSelection() {
		return *m_selection;
	}

private:
	ee::SprPtr SelectByPos(const sm::ivec2& pos) const;

protected:
	StagePanel* m_stage;

private:
	ee::SpriteSelection* m_selection;

}; // SelectSpriteOP

}

#endif // _EASYCOMPLEX3D_SELECT_SPRITE_OP_H_
