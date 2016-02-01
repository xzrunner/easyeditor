#ifndef _EASYTERRAIN2D_CREATE_REGION_OP_H_
#define _EASYTERRAIN2D_CREATE_REGION_OP_H_

#include <easyshape.h>

namespace eterrain2d
{

class StagePanel;

class CreateRegionOP : public eshape::DrawPolygonOP
{
public:
	CreateRegionOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiShapesImpl* shapes_impl);

	virtual bool OnDraw() const;

private:
	StagePanel* m_stage;

	ee::MultiShapesImpl* m_shapes_impl;

}; // CreateRegionOP

}

#endif // _EASYTERRAIN2D_CREATE_REGION_OP_H_