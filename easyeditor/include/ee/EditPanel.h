#ifndef _EASYEDITOR_EDIT_PANEL_H_
#define _EASYEDITOR_EDIT_PANEL_H_

#include "Vector.h"

#include <wx/wx.h>

#include <vector>

namespace ee
{

class Sprite;
class AtomicOP;
class EditOP;
class StageCanvas;
class Camera;
class KeysState;
class EditPanelImpl;

class EditPanel : public wxPanel
{
public:
	EditPanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~EditPanel();

	virtual bool Update(int version) { return false; }

 	virtual void OnSizeDebug(wxSizeEvent& event) {}
 	virtual void OnMouseHook(wxMouseEvent& event) {}
 	virtual void OnKeyHook(int key_code) {}

	EditPanelImpl* GetStageImpl() { return m_impl; }

	Vector TransPosScrToProj(int x, int y) const;
	Vector TransPosProjToScr(const Vector& proj) const;

	void DrawEditOP() const;

	const EditOP* GetEditOP() const;
	EditOP* GetEditOP();
	void SetEditOP(EditOP* editOP);

	const StageCanvas* GetCanvas() const;
	StageCanvas* GetCanvas();
	void SetCanvas(StageCanvas* canvas);

	Camera* GetCamera() const;

	// In Stage, class StagePanel can't get focus, only its class StageCanvas has the focus, so 
	// these two func should be called by StageCanvas.
	// While in SymbolEdit, class SymbolEditPanel can get focus.
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);

	void SaveOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites);
	void LoadOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites);

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