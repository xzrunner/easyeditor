#pragma once

#include <wx/wx.h>

#include "interfaces.h"
#include "HistoryList.h"
#include "common/Vector.h"

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

		virtual void clear();

		Vector transPosScreenToProject(int x, int y) const;
		Vector transPosProjectToScreen(const Vector& proj) const;

		void drawEditTemp() const;

		AbstractEditOP* getEditOP() const { return m_editOP; }
		void setEditOP(AbstractEditOP* editOP);

		IStageCanvas* getCanvas() const { return m_canvas; }
		void SetCanvas(IStageCanvas* canvas) { m_canvas = canvas; }

		Camera* getCamera() const { return m_camera; }

		// In Stage, class StagePanel can't get focus, only its class IStageCanvas has the focus, so 
		// these two func should be called by IStageCanvas.
		// While in SymbolEdit, class SymbolEditPanel can get focus.
		void onMouse(wxMouseEvent& event);
		virtual void OnKeyDown(wxKeyEvent& event);
		virtual void OnKeyUp(wxKeyEvent& event);
		void OnMouseWheelRotation(int x, int y, int direction);

		void resetCanvas();
		void ResetViewport();

		void undo();
		void redo();
		void addHistoryOP(AbstractAtomicOP* op);

		void saveHistoryList(const std::string& filepath, const std::vector<ISprite*>& sprites);
		void loadHistoryList(const std::string& filepath, const std::vector<ISprite*>& sprites);

		void onSave();
		bool isDirty() const;

		void setTitleStatus(bool dirty);

		void OnRightPopupMenu(wxCommandEvent& event);

	protected:
		void onSize(wxSizeEvent& event);

		virtual void OnSizeDebug(wxSizeEvent& event) {}

		virtual void OnMouseHook(wxMouseEvent& event) {}
		virtual void OnKeyHook(int key_code) {}

	protected:
		AbstractEditOP* m_editOP;

		IStageCanvas* m_canvas;
		Camera* m_camera;

		HistoryList m_historyList;

		wxTopLevelWindow* m_frame;

		DECLARE_EVENT_TABLE()

	}; // EditPanel
}

