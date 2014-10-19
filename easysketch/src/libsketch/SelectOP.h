#ifndef _LIBSKETCH_SELECT_OP_H_
#define _LIBSKETCH_SELECT_OP_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class StagePanel;

class SelectOP : public d2d::AbstractEditOP
{
public:	
	SelectOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);

	const d2d::SpriteSelection& GetSelection() const {
		return *m_selection;
	}

private:
	d2d::ISprite* SelectByPos(const ivec2& pos) const;

protected:
	StagePanel* m_stage;

private:
	d2d::SpriteSelection* m_selection;

}; // SelectOP

}

#endif // _LIBSKETCH_SELECT_OP_H_