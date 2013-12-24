#ifndef EPARTICLE_COCO_EDIT_OP_H
#define EPARTICLE_COCO_EDIT_OP_H

#include <drag2d.h>

namespace eparticle
{
namespace coco
{
	class StagePanel;

	class EditOP : public d2d::ZoomViewOP
	{
	public:
		EditOP(StagePanel* stage);

		virtual bool onMouseLeftDown(int x, int y);

	private:
		StagePanel* m_stage;

	}; // EditOP
}
}

#endif // EPARTICLE_COCO_EDIT_OP_H