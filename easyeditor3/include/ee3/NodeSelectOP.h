#pragma once

#include <ee/EditOP.h>

#include <node0/SceneNode.h>

#include <SM_Vector.h>

namespace ee3
{

class StagePage;

class NodeSelectOP : public ee::EditOP
{
public:	
	NodeSelectOP(StagePage& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	n0::SceneNodePtr SelectByPos(const sm::vec2& pos) const;

protected:
	StagePage& m_stage;

}; // NodeSelectOP

}
