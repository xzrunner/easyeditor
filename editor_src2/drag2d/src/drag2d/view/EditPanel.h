#pragma once

#include <wx/wx.h>

#include "interfaces.h"
#include "HistoryList.h"
#include "common/Vector.h"

namespace d2d
{
	class AbstractAtomicOP;
	class AbstractEditOP;
	class GLCanvas;
	class Camera;

	class EditPanel : public wxPanel, public ICameraObserver
	{
	public:
		EditPanel(wxWindow* parent, wxTopLevelWindow* frame);
		virtual ~EditPanel();

 		//
 		// ICameraObserver interface
 		//
 		virtual void onCameraChanged();

		virtual void clear();

		Vector transPosScreenToProject(int x, int y) const;
//		Vector transPosProjectToScreen(const Vector& proj) const;

		void drawEditTemp() const;

		AbstractEditOP* getEditOP() const { return m_editOP; }
		void setEditOP(AbstractEditOP* editOP);

		GLCanvas* getCanvas() const { return m_canvas; }
		void setCanvas(GLCanvas* canvas) { m_canvas = canvas; }

		Camera* getCamera() const { return m_camera; }

		// In Stage, class StagePanel can't get focus, only its class GLCanvas has the focus, so 
		// these two func should be called by GLCanvas.
		// While in SymbolEdit, class SymbolEditPanel can get focus.
		void onMouse(wxMouseEvent& event);
		virtual void onKeyDown(wxKeyEvent& event);
		virtual void onKeyUp(wxKeyEvent& event);
		void onMouseWheelRotation(int x, int y, int direction);

		void resetCanvas();

		void undo();
		void redo();
		void addHistoryOP(AbstractAtomicOP* op);

		void saveHistoryList(const std::string& filepath, const std::vector<ISprite*>& sprites);
		void loadHistoryList(const std::string& filepath, const std::vector<ISprite*>& sprites);

		void onSave();
		bool isDirty() const;

	public:
		enum
		{
			Menu_UpOneLayer = 0,
			Menu_DownOneLayer,
			Menu_End,

			Hot_UpOneLayer = 100,
			Hot_DownOneLayer,
			Hot_Delete,
			Hot_End
		};

		static std::string menu_entries[];

		void onMenuUpOneLayer(wxCommandEvent& event);
		void onMenuDownOneLayer(wxCommandEvent& event);

		void onKeyUpOneLayer(wxKeyEvent& event);
		void onKeyDownOneLayer(wxKeyEvent& event);
//		void onKeyDelete(wxKeyEvent& event);

	protected:
		void onSize(wxSizeEvent& event);

	private:
		void setTitleStatus(bool fixed);

	protected:
		AbstractEditOP* m_editOP;

		GLCanvas* m_canvas;
		Camera* m_camera;

		HistoryList m_historyList;

		wxTopLevelWindow* m_frame;

		DECLARE_EVENT_TABLE()

	}; // EditPanel
}

