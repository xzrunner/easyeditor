#pragma once

#include "NodeSelectOP.h"
#include "EditOpState.h"

#include <memory>

namespace enode3d
{

class StageCanvas;

class NodeArrangeOP : public NodeSelectOP
{
public:
	NodeArrangeOP(StagePanel& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	std::shared_ptr<StageCanvas> m_canvas = nullptr;

	std::unique_ptr<EditOpState> m_op_state = nullptr;

}; // NodeArrangeOP

}
