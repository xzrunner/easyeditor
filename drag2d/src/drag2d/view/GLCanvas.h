#pragma once

#include <wx/glcanvas.h>

#include "Screen.h"
#include "common/Color.h"

namespace d2d
{
	class Camera;
	class EditPanel;

	class GLCanvas : public wxGLCanvas
	{
	public:
		GLCanvas(EditPanel* editPanel);
		virtual ~GLCanvas();

		virtual void clear() {}

		void resetInitState();		// Another GLCanvas closed, refresh the under one

		void resetViewport();		// On Mouse Wheel
									// onSize no use, if the size not change
									// also can put gluOrtho2D in each onPaint, save this and Camera's observer pattern

		void setBgColor(const Colorf& color);

		void SetCurrentCanvas();

		const Screen& GetScreen() const { return m_screen; }

	protected:
		virtual void initGL();
		virtual void onSize(int w, int h) = 0;
		virtual void onDraw() = 0;

		void onSize(wxSizeEvent& event);
		void onPaint(wxPaintEvent& event);
		void onEraseBackground(wxEraseEvent& event);
 		void onMouse(wxMouseEvent& event);
 		void onKeyDown(wxKeyEvent& event);
		void onKeyUp(wxKeyEvent& event);

	protected:
		EditPanel* m_editPanel;

		Camera* m_camera;

		Colorf m_bgColor;

		int m_width, m_height;

		Screen m_screen;

	private:
		bool m_isInit;
		wxGLContext* m_context;

		DECLARE_EVENT_TABLE()

	}; // GLCanvas
}

