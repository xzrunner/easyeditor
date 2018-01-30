#pragma once

#include "node0/SceneNode.h"

#include <ee/EditOP.h>

#include <SM_Vector.h>

namespace ee0
{

class StagePage;

class NodeSelectOP : public ee::EditOP
{
public:	
	NodeSelectOP(StagePage& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;

private:
	virtual n0::SceneNodePtr SelectByPos(int screen_x, int screen_y) const = 0;

protected:
	StagePage& m_stage;

}; // NodeSelectOP

}
