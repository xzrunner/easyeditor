#pragma once

#include <ee/EditOP.h>

#include <memory>

namespace ee0
{

class EditOP : public ee::EditOP
{
public:
	EditOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnKeyUp(int keyCode) override;
	virtual bool OnChar(int keyCode) override;

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

	virtual bool OnPopMenuSelected(int type) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	void SetPrevEditOP(const std::shared_ptr<EditOP>& op) { m_prev_op = op; }

private:
	std::shared_ptr<EditOP> m_prev_op = nullptr;

}; // EditOP

}