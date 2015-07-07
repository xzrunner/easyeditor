//#include <gl/glew.h>
//
//#include "IStageCanvas.h"
//#include "Camera.h"
//
//#include "common/Exception.h"
//#include "view/ExceptionDlg.h"
//#include "view/EditPanel.h"
//#include "render/RenderList.h"
//#include "render/RenderContext.h"
//#include "render/ShaderMgrBase.h"
//#include "render/ShaderMgr.h"
//
//#include <wx/wx.h>
//
//namespace d2d
//{
//
//BEGIN_EVENT_TABLE(IStageCanvas, wxGLCanvas)
//	EVT_SIZE(IStageCanvas::onSize)
//	EVT_PAINT(IStageCanvas::onPaint)
//	EVT_ERASE_BACKGROUND(IStageCanvas::onEraseBackground)
// 	EVT_MOUSE_EVENTS(IStageCanvas::onMouse)
// 	EVT_KEY_DOWN(IStageCanvas::OnKeyDown)
//	EVT_KEY_UP(IStageCanvas::OnKeyUp)
//END_EVENT_TABLE()
//
//int gl_attrib[20] = {WX_GL_RGBA, WX_GL_MIN_RED, 1, WX_GL_MIN_GREEN, 1,
//				WX_GL_MIN_BLUE, 1, WX_GL_DEPTH_SIZE, 1,
//				WX_GL_DOUBLEBUFFER,WX_GL_STENCIL_SIZE, 8, 0};
//
//IStageCanvas::IStageCanvas(EditPanel* editPanel)
//	: wxGLCanvas(editPanel, wxID_ANY, gl_attrib)
//	, m_stage(editPanel)
// 	, m_camera(editPanel->getCamera())
//	, m_screen(editPanel->getCamera())
// 	, m_screen_width(0), m_screen_height(0)
// 	, m_isInit(false)
// 	, m_context(new wxGLContext(this))
//{
//	m_bg_color.set(0.5f, 0.5f, 0.5f, 1);
//}
//
//IStageCanvas::~IStageCanvas()
//{
//	delete m_context;
//}
//
//void IStageCanvas::ResetInitState() 
//{ 
//	m_isInit = false; 
//}
//
//void IStageCanvas::ResetViewport()
//{
////	wxLogDebug(_T("IStageCanvas::ResetViewport()"));
//	onSize(wxSizeEvent(m_parent->GetSize()));
//}
//
//void IStageCanvas::SetBgColor(const Colorf& color)
//{
//	m_bg_color = color;
//}
//
//void IStageCanvas::SetCurrentCanvas()
//{
//	SetCurrent(*m_context);
//}
//
//void IStageCanvas::InitGL()
//{
//	try {
//		wxLogDebug(_T("IStageCanvas::InitGL()"));
//
//		if (glewInit() != GLEW_OK) {
//			exit(1);
//		}
//
//		ShaderMgr::Instance()->Reset();
//
//		ResetViewport();
//
//		glEnable(GL_TEXTURE_2D);
//
//		if (RenderContext::SHADER_MGR) {
//			RenderContext::SHADER_MGR->reload();
//		}	
//	} catch (Exception& e) {
//		ExceptionDlg dlg(m_parent, e);
//		dlg.ShowModal();	
//	}
//}
//
//void IStageCanvas::onSize(wxSizeEvent& event)
//{
//	wxSize size = event.GetSize();
// 	m_screen_width = size.GetWidth();
// 	m_screen_height = size.GetHeight();
//	onSize(m_screen_width, m_screen_height);
//}
//
//void IStageCanvas::onPaint(wxPaintEvent& event)
//{
//	static IStageCanvas* curr_canvas = NULL;
//
//	// Makes the OpenGL state that is represented by the OpenGL rendering 
//	// context context current
//	SetCurrentCanvas();
//
//	if (curr_canvas != this) {
//		RenderContext::SHADER_MGR->Reset();
//		curr_canvas = this;
//	}
//
//	if (!m_isInit)
//	{
//		InitGL();
//		m_isInit = true;
//	}
//
//	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	try {
//		OnDraw();
//	} catch (d2d::Exception& e) {
//		d2d::ExceptionDlg dlg(this, e);
//		dlg.ShowModal();
//	}
//
//	if (RenderContext::SHADER_MGR) {
//		RenderContext::SHADER_MGR->Flush();
//	}
//
//	glFlush();
//	SwapBuffers();
//
////	wxPaintDC dc(this);
//	OnDrawDC();
//}
//
//void IStageCanvas::onEraseBackground(wxEraseEvent& event)
//{	
//}
//
//void IStageCanvas::onMouse(wxMouseEvent& event)
//{
//	m_stage->onMouse(event);
//
//	// The handler of this event should normally call event.Skip() to allow the default processing 
//	// to take place as otherwise the window under mouse wouldn't get the focus.
//	if (event.LeftDown()) {
//		event.Skip();
//	}
//}
//
//void IStageCanvas::OnKeyDown(wxKeyEvent& event)
//{
//	m_stage->OnKeyDown(event);
//}
//
//void IStageCanvas::OnKeyUp(wxKeyEvent& event)
//{
//	m_stage->OnKeyUp(event);
//}
//
//} // d2d