#pragma once

#include <ee/EditOP.h>

#include <node3/INode.h>

namespace enode3d
{

class StagePanel;

class SprSelectOP : public ee::EditOP
{
public:	
	SprSelectOP(StagePanel& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	n3::NodePtr SelectByPos(const sm::vec2& pos) const;

protected:
	StagePanel& m_stage;

}; // SprSelectOP

}
