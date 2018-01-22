#pragma once

#include <data/SceneNode.h>

#include <ee/EditOP.h>

#include <SM_Vector.h>

namespace eone
{

class StagePanel;

class NodeSelectOP : public ee::EditOP
{
public:	
	NodeSelectOP(StagePanel& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	SceneNodePtr SelectByPos(const sm::vec2& pos) const;

protected:
	StagePanel& m_stage;

}; // NodeSelectOP

}
