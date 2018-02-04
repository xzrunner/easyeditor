#pragma once

#include "ee0/KeysState.h"

#include <memory>

class wxMouseEvent;
class wxKeyEvent;
class wxSizeEvent;

namespace ee0
{

class EditOP;
class WxStageCanvas;

class EditPanelImpl
{
public:
	EditPanelImpl();

	const std::shared_ptr<EditOP>& GetEditOP() const { return m_edit_op; }
	std::shared_ptr<EditOP>& GetEditOP() { return m_edit_op; }
	void SetEditOP(const std::shared_ptr<EditOP>& op) { m_edit_op = op; }

	const std::shared_ptr<WxStageCanvas>& GetCanvas() const { return m_canvas; }
	std::shared_ptr<WxStageCanvas>& GetCanvas() { return m_canvas; }
	void SetCanvas(const std::shared_ptr<WxStageCanvas>& canvas) { m_canvas = canvas; }

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);

	bool GetKeyState(int key) const { return m_keys_state.GetKeyState(key); }

	void OnSize(wxSizeEvent& event);

private:
	std::shared_ptr<EditOP> m_edit_op = nullptr;

	std::shared_ptr<WxStageCanvas> m_canvas = nullptr;

	KeysState m_keys_state;

}; // EditPanelImpl

}