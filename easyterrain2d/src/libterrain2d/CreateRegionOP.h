#ifndef _EASYTERRAIN2D_CREATE_REGION_OP_H_
#define _EASYTERRAIN2D_CREATE_REGION_OP_H_

#include <easyshape.h>

namespace eterrain2d
{

class StagePanel;

class CreateRegionOP : public libshape::DrawPolygonOP
{
public:
	CreateRegionOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl);

	virtual bool OnDraw() const;

private:
	StagePanel* m_stage;

}; // CreateRegionOP

}

#endif // _EASYTERRAIN2D_CREATE_REGION_OP_H_