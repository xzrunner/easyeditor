#ifndef _EASYEDITOR_EDIT_PANEL_H_
#define _EASYEDITOR_EDIT_PANEL_H_

#include "Sprite.h"

#include <SM_Vector.h>

#include <wx/wx.h>

#include <vector>

namespace ee
{

class AtomicOP;
class StageCanvas;
class Camera;
class KeysState;
class EditPanelImpl;
class EditOP;

class EditPanel : public wxPanel
{
public:
	EditPanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~EditPanel();

	virtual bool UpdateStage() { return false; }

 	virtual void OnSizeDebug(wxSizeEvent& event) {}
 	virtual void OnMouseHook(wxMouseEvent& event) {}
 	virtual void OnKeyHook(int key_code) {}

	EditPanelImpl* GetStageImpl() { return m_impl; }

	sm::vec2 TransPosScrToProj(int x, int y) const;
	sm::vec2 TransPosProjToScr(const sm::vec2& proj) const;

	void DrawEditOP() const;

	const std::shared_ptr<ee::EditOP>& GetEditOP() const;
	std::shared_ptr<ee::EditOP>& GetEditOP();
	void SetEditOP(const std::shared_ptr<EditOP>& op);

	const std::shared_ptr<StageCanvas>& GetCanvas() const;
	std::shared_ptr<StageCanvas>& GetCanvas();
	void SetCanvas(const std::shared_ptr<StageCanvas>& canvas);

	// In Stage, class StagePanel can't get focus, only its class StageCanvas has the focus, so 
	// these two func should be called by StageCanvas.
	// While in SymbolEdit, class SymbolEditPanel can get focus.
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);

	void SaveOpRecordList(const std::string& filepath, const std::vector<SprPtr>& sprs);
	void LoadOpRecordList(const std::string& filepath, const std::vector<SprPtr>& sprs);

	void OnSave();
	bool IsEditDirty() const;

	void OnRightPopupMenu(wxCommandEvent& event);

	bool GetKeyState(int key) const;
	const KeysState& GetKeyState() const;
	KeysState& GetKeyState();

	void RefreshFrame();

protected:
	void OnSize(wxSizeEvent& event);

private:
	EditPanelImpl* m_impl;

	DECLARE_EVENT_TABLE()

}; // EditPanel

}

#endif // _EASYEDITOR_EDIT_PANEL_H_