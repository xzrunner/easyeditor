#pragma once

#include <ee/EditOP.h>

#include <memory>

namespace enode3d
{

class StageCanvas;
class EditOpState;

class CamControlOP : public ee::EditOP
{
public:
	CamControlOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	StageCanvas* m_canvas;

	std::unique_ptr<EditOpState> m_op_state = nullptr;

}; // CamControlOP

}