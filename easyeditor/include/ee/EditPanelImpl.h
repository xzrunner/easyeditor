#ifndef _EASYEDITOR_EDIT_PANEL_IMPL_H_
#define _EASYEDITOR_EDIT_PANEL_IMPL_H_

#include "KeysState.h"
#include "Observer.h"
#include "HistoryList.h"

#include <SM_Vector.h>
#include <cu/CU_RefCountObj.h>

#include <wx/wx.h>

namespace ee
{

class EditPanel;
class AtomicOP;
class StageCanvas;
class EditOP;

class EditPanelImpl : public Observer, public cu::RefCountObj
{
public:
	EditPanelImpl(wxTopLevelWindow* frame, EditPanel* stage);
	~EditPanelImpl();

	void SetEditPanelNull();
	
	void SetActive(bool active) { m_active = active; }
	bool Update();

	sm::vec2 TransPosScrToProj(int x, int y) const;
	sm::vec2 TransPosProjToScr(const sm::vec2& proj) const;

	void DrawEditOP() const;

	const std::shared_ptr<ee::EditOP>& GetEditOP() const { return m_edit_op; }
	std::shared_ptr<ee::EditOP>& GetEditOP() { return m_edit_op; }
	void SetEditOP(const std::shared_ptr<EditOP>& op);

	const std::shared_ptr<StageCanvas>& GetCanvas() const { return m_canvas; }
	std::shared_ptr<StageCanvas>& GetCanvas() { return m_canvas; }
	void SetCanvas(const std::shared_ptr<StageCanvas>& canvas) { m_canvas = canvas; }

	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnMouseWheelRotation(int x, int y, int direction);

	void Undo();
	void Redo();
	void RedoTop();

	void SaveOpRecordList(const std::string& filepath, const std::vector<SprPtr>& sprs);
	void LoadOpRecordList(const std::string& filepath, const std::vector<SprPtr>& sprs);

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
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	void Clear();

	void InitSubjects();

	void AddOpRecord(const std::shared_ptr<AtomicOP>& op);

	void SetWndDirty(bool dirty);

	void UpdateWndState(HistoryList::Type type);

protected:
	std::shared_ptr<EditOP> m_edit_op = nullptr;

	std::shared_ptr<StageCanvas> m_canvas = nullptr;

	wxTopLevelWindow* m_frame;

private:
	EditPanel* m_stage;

	HistoryList m_history_list;

	KeysState m_keys_state;

	bool m_active;

}; // EditPanelImpl

}

#endif // _EASYEDITOR_EDIT_PANEL_IMPL_H_