#ifndef EPARTICLE_COCO3D_EDIT_OP_H
#define EPARTICLE_COCO3D_EDIT_OP_H

#include <drag2d.h>

namespace eparticle
{
namespace coco3d
{
	class StagePanel;

	class EditOP : public d2d::AbstractEditOP
	{
	public:
		EditOP(StagePanel* stage);

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseDrag(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseWheelRotation(int x, int y, int direction);

	private:
		StagePanel* m_stage;

	}; // EditOP
}
}

#endif // EPARTICLE_COCO3D_EDIT_OP_H