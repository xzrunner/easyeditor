#ifndef _DRAG2D_EDIT_PANEL_IMPL_H_
#define _DRAG2D_EDIT_PANEL_IMPL_H_

#include "HistoryList.h"
#include "KeysState.h"

#include "common/Object.h"
#include "common/Vector.h"

#include <wx/wx.h>

namespace d2d
{

class EditPanel;
class AbstractAtomicOP;
class AbstractEditOP;
class IStageCanvas;
class Camera;

class EditPanelImpl : public Object
{
public:
	EditPanelImpl(wxTopLevelWindow* frame, EditPanel* stage);
	~EditPanelImpl();

	void SetEditPanelNull();
	
	void Clear();

	Vector TransPosScrToProj(int x, int y) const;
	Vector TransPosProjToScr(const Vector& proj) const;

	void DrawEditOP() const;

	const AbstractEditOP* GetEditOP() const { return m_edit_op; }
	AbstractEditOP* GetEditOP() { return m_edit_op; }
	void SetEditOP(AbstractEditOP* editOP);

	const IStageCanvas* GetCanvas() const { return m_canvas; }
	IStageCanvas* GetCanvas() { return m_canvas; }
	void SetCanvas(IStageCanvas* canvas);

	Camera* GetCamera() const { return m_camera; }

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
	KeysState& GetKeyState() { return m_keys_state; }

	void RefreshFrame();

	void OnSize(wxSizeEvent& event);

	void SetCursor(wxCursor cursor);
	void SetFocus();

	void PopupMenu(wxMenu* menu, int x, int y);

protected:
	AbstractEditOP* m_edit_op;

	IStageCanvas* m_canvas;
	Camera* m_camera;

	wxTopLevelWindow* m_frame;

private:
	EditPanel* m_stage;

	HistoryList m_history_list;

	KeysState m_keys_state;

}; // EditPanelImpl

}

#endif // _DRAG2D_EDIT_PANEL_IMPL_H_