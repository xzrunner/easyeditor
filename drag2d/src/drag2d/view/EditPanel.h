#ifndef _DRAG2D_EDIT_PANEL_H_
#define _DRAG2D_EDIT_PANEL_H_

#include "HistoryList.h"
#include "KeysState.h"

#include "common/Vector.h"

#include <wx/wx.h>

namespace d2d
{

class AbstractAtomicOP;
class AbstractEditOP;
class IStageCanvas;
class Camera;

class EditPanel : public wxPanel
{
public:
	EditPanel(wxWindow* parent, wxTopLevelWindow* frame);
	virtual ~EditPanel();

	//
	// wxWindow
	//
	virtual void Clear();
	virtual bool Update(int version) { return false; }

	Vector TransPosScrToProj(int x, int y) const;
	Vector TransPosProjToScr(const Vector& proj) const;

	void DrawEditOP() const;

	const AbstractEditOP* GetEditOP() const { return m_edit_op; }
	AbstractEditOP* GetEditOP() { return m_edit_op; }
	void SetEditOP(AbstractEditOP* editOP);

	const IStageCanvas* GetCanvas() const { return m_canvas; }
	IStageCanvas* GetCanvas() { return m_canvas; }
	void SetCanvas(IStageCanvas* canvas) { m_canvas = canvas; }

	Camera* GetCamera() const { return m_camera; }

	// In Stage, class StagePanel can't get focus, only its class IStageCanvas has the focus, so 
	// these two func should be called by IStageCanvas.
	// While in SymbolEdit, class SymbolEditPanel can get focus.
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);

	void ResetCanvas();
	void ResetViewport();

	void Undo();
	void Redo();
	void AddOpRecord(AbstractAtomicOP* op);

	void SaveOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites);
	void LoadOpRecordList(const std::string& filepath, const std::vector<ISprite*>& sprites);

	void OnSave();
	bool IsEditDirty() const;

	void SetTitleStatus(bool dirty);

	void OnRightPopupMenu(wxCommandEvent& event);

	void SetCanvasDirty();

	bool GetKeyState(int key) const;
	const KeysState& GetKeyState() const { return m_keys_state; }

protected:
	void OnSize(wxSizeEvent& event);

	virtual void OnSizeDebug(wxSizeEvent& event) {}

	virtual void OnMouseHook(wxMouseEvent& event) {}
	virtual void OnKeyHook(int key_code) {}

protected:
	AbstractEditOP* m_edit_op;

	IStageCanvas* m_canvas;
	Camera* m_camera;

	wxTopLevelWindow* m_frame;

private:
	HistoryList m_history_list;

	KeysState m_keys_state;

	DECLARE_EVENT_TABLE()

}; // EditPanel

}

#endif // _DRAG2D_EDIT_PANEL_H_