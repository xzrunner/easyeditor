#ifndef _EASYMESH_EDIT_UV_OP_H_
#define _EASYMESH_EDIT_UV_OP_H_

#include <ee/ZoomViewOP.h>

namespace emesh
{

class StagePanel;

class EditUVOP : public ee::ZoomViewOP
{
public:
	EditUVOP(StagePanel* stage);

	virtual bool OnDraw() const;

private:
	StagePanel* m_stage;

}; // EditUVOP

}

#endif // _EASYMESH_EDIT_UV_OP_H_