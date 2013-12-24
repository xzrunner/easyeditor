
#ifndef EPARTICLE_SELECT_BODY_OP_H
#define EPARTICLE_SELECT_BODY_OP_H

#include <drag2d.h>

namespace eparticle
{
	class StagePanel;

	class EditOP : public d2d::ZoomViewOP
	{
	public:
		EditOP(StagePanel* stage);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);

	private:
		StagePanel* m_stage;

	}; // SelectBodyOP
}

#endif // EPARTICLE_SELECT_BODY_OP_H
