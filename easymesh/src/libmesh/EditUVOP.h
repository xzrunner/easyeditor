#ifndef _EASYMESH_EDIT_UV_OP_H_
#define _EASYMESH_EDIT_UV_OP_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;

class EditUVOP : public d2d::ZoomViewOP
{
public:
	EditUVOP(StagePanel* stage);

	virtual bool onDraw(const d2d::Screen& scr) const;

private:
	StagePanel* m_stage;

}; // EditUVOP

}

#endif // _EASYMESH_EDIT_UV_OP_H_