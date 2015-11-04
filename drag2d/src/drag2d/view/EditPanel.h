#ifndef _DRAG2D_EDIT_PANEL_H_
#define _DRAG2D_EDIT_PANEL_H_

#include "common/Vector.h"

#include <wx/wx.h>
#include <vector>

namespace d2d
{

class ISprite;
class AbstractAtomicOP;
class AbstractEditOP;
class IStageCanvas;
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

	const AbstractEditOP* GetEditOP() const;
	AbstractEditOP* GetEditOP();
	void SetEditOP(AbstractEditOP* editOP);

	const IStageCanvas* GetCanvas() const;
	IStageCanvas* GetCanvas();
	void SetCanvas(IStageCanvas* canvas);

	Camera* GetCamera() const;

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

#endif // _DRAG2D_EDIT_PANEL_H_