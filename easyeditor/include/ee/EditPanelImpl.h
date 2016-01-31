#ifndef _EASYEDITOR_EDIT_PANEL_IMPL_H_
#define _EASYEDITOR_EDIT_PANEL_IMPL_H_

#include "KeysState.h"
#include "Object.h"
#include "Vector.h"
#include "Observer.h"
#include "HistoryList.h"

#include <wx/wx.h>

namespace ee
{

class EditPanel;
class AtomicOP;
class EditOP;
class StageCanvas;
class Camera;
class Sprite;

class EditPanelImpl : public Object, public Observer
{
public:
	EditPanelImpl(wxTopLevelWindow* frame, EditPanel* stage);
	~EditPanelImpl();

	void SetEditPanelNull();
	
	bool Update(int version);

	Vector TransPosScrToProj(int x, int y) const;
	Vector TransPosProjToScr(const Vector& proj) const;

	void DrawEditOP() const;

	const EditOP* GetEditOP() const { return m_edit_op; }
	EditOP* GetEditOP() { return m_edit_op; }
	void SetEditOP(EditOP* editOP);

	const StageCanvas* GetCanvas() const { return m_canvas; }
	StageCanvas* GetCanvas() { return m_canvas; }
	void SetCanvas(StageCanvas* canvas);

	Camera* GetCamera() const { return m_camera; }

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);

	void Undo();
	void Redo();

	void SaveOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites);
	void LoadOpRecordList(const std::string& filepath, const std::vector<Sprite*>& sprites);

	void OnSave();
	bool IsEditDirty() const;

	void OnRightPopupMenu(wxCommandEvent& event);

	bool GetKeyState(int key) const;
	const KeysState& GetKeyState() const { return m_keys_state; }
	KeysState& GetKeyState() { return m_keys_state; }

	void RefreshFrame();

	void OnSize(wxSizeEvent& event);

	void SetCursor(wxCursor cursor);
	void SetFocus();

	void PopupMenu(wxMenu* menu, int x, int y);

	void SetDropTarget(wxDropTarget* target);

	EditPanel* GetEditPanel() { return m_stage; }

protected:
	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void Clear();

	void InitSubjects();

	void AddOpRecord(AtomicOP* op);

	void SetWndDirty(bool dirty);

protected:
	EditOP* m_edit_op;

	StageCanvas* m_canvas;
	Camera* m_camera;

	wxTopLevelWindow* m_frame;

private:
	EditPanel* m_stage;

	HistoryList m_history_list;

	KeysState m_keys_state;

}; // EditPanelImpl

}

#endif // _EASYEDITOR_EDIT_PANEL_IMPL_H_