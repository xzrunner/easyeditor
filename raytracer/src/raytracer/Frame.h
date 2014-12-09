#ifndef _RT_FRAME_H_
#define _RT_FRAME_H_

#include <wx/wx.h>

namespace rt
{

class Canvas;

class Frame : public wxFrame
{
public:
	Frame(const wxPoint& pos, const wxSize& size);

	//event handlers
	void OnQuit( wxCommandEvent& event );
	void OnOpenFile( wxCommandEvent& event );
	void OnSaveFile( wxCommandEvent& event );
	void OnRenderStart( wxCommandEvent& event );
	void OnRenderCompleted( wxCommandEvent& event );
	void OnRenderPause( wxCommandEvent& event );
	void OnRenderResume( wxCommandEvent& event );

private:
	Canvas* m_canvas; //where the rendering takes place

	wxString m_current_path; //for file dialogues

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _RT_FRAME_H_