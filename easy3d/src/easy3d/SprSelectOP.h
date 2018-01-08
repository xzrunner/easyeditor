#pragma once

#include <ee/EditOP.h>
#include <ee/Sprite.h>

namespace ee { class SpriteSelection; }

namespace e3d
{

class StagePanel;

class SprSelectOP : public ee::EditOP
{
public:	
	SprSelectOP(StagePanel& stage);

	virtual bool OnMouseLeftDown(int x, int y) override;

	virtual bool OnDraw() const override;

	const ee::SpriteSelection& GetSelection() const {
		return *m_selection;
	}
	ee::SpriteSelection& GetSelection() {
		return *m_selection;
	}

private:
	ee::SprPtr SelectByPos(const sm::vec2& pos) const;

protected:
	StagePanel& m_stage;

private:
	ee::SpriteSelection* m_selection;

}; // SprSelectOP

}
