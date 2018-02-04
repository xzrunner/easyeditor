#pragma once

#include "ee0/EditOP.h"

#include <SM_Vector.h>
#include <node0/typedef.h>

#include <vector>

namespace ee0
{

class WxStagePage;

class NodeSelectOP : public EditOP
{
public:	
	NodeSelectOP(WxStagePage& stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;

private:
	virtual n0::SceneNodePtr QueryByPos(int screen_x, int screen_y) const = 0;
	virtual void QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1,
		bool contain, std::vector<n0::SceneNodePtr>& result) const = 0;

protected:
	WxStagePage& m_stage;

private:
	sm::ivec2 m_last_pos;

}; // NodeSelectOP

}
