#pragma once

#include "ee2/CamControlOP.h"

#include <ee0/NodeSelectOP.h>

#include <memory>

namespace ee0 { class EditOpState; }

namespace ee2
{

class StagePage;
class CamControlOP;

class NodeSelectOP : public ee0::NodeSelectOP
{
public:
	NodeSelectOP(StagePage& stage);
	virtual ~NodeSelectOP() = default;

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	virtual n0::SceneNodePtr QueryByPos(int screen_x, int screen_y) const override;
	virtual void QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1,
		bool contain, std::vector<n0::SceneNodePtr>& result) const;

private:
	std::unique_ptr<ee0::EditOpState> m_draw_state = nullptr;

}; // NodeSelectOP

}