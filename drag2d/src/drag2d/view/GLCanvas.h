//#pragma once
//
//#include <wx/glcanvas.h>
//
//#include "Screen.h"
//#include "common/Color.h"
//
//namespace d2d
//{
//	class Camera;
//	class EditPanel;
//
//	class IStageCanvas : public wxGLCanvas
//	{
//	public:
//		IStageCanvas(EditPanel* editPanel);
//		virtual ~IStageCanvas();
//
//		virtual void clear() {}
//
//		void ResetInitState();		// Another IStageCanvas closed, refresh the under one
//
//		void ResetViewport();		// On Mouse Wheel
//									// onSize no use, if the size not change
//									// also can put gluOrtho2D in each onPaint, save this and Camera's observer pattern
//
//		void SetBgColor(const Colorf& color);
//
//		void SetCurrentCanvas();
//
//		Camera* GetCamera() { return m_camera; }
//
//	protected:
//		virtual void InitGL();
//		virtual void onSize(int w, int h) = 0;
//		virtual void OnDraw() = 0;
//		virtual void OnDrawDC() const {}
//
//		void onSize(wxSizeEvent& event);
//		void onPaint(wxPaintEvent& event);
//		void onEraseBackground(wxEraseEvent& event);
// 		void onMouse(wxMouseEvent& event);
// 		void OnKeyDown(wxKeyEvent& event);
//		void OnKeyUp(wxKeyEvent& event);
//
//	protected:
//		EditPanel* m_stage;
//
//		Camera* m_camera;
//
//		Colorf m_bg_color;
//
//		int m_screen_width, m_screen_height;
//
//		Screen m_screen;
//
//		bool m_isInit;
//
//	private:
//		wxGLContext* m_context;
//
//		DECLARE_EVENT_TABLE()
//
//	}; // IStageCanvas
//}
//
