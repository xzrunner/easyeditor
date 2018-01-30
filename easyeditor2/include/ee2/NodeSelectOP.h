#pragma once

#include <ee0/NodeSelectOP.h>

namespace ee2
{

class StagePage;

class NodeSelectOP : public ee0::NodeSelectOP
{
public:
	NodeSelectOP(StagePage& stage);

	virtual bool OnDraw() const override;

private:
	virtual n0::SceneNodePtr SelectByPos(int screen_x, int screen_y) const override;

}; // NodeSelectOP

}