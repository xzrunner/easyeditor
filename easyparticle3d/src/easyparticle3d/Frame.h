#ifndef _EASYPARTICLE3D_FRAME_H_
#define _EASYPARTICLE3D_FRAME_H_

#include <wx/wx.h>

class wxSplitterWindow;

namespace eparticle3d
{

class Task;

class Frame : public wxFrame
{
public:
	Frame(const wxString& title);

private:
	void onNew(wxCommandEvent& event);
	void onOpen(wxCommandEvent& event);
	void onSave(wxCommandEvent& event);
	void onSaveAs(wxCommandEvent& event);

	void onQuit(wxCommandEvent& event);
//		void onAbout(wxCommandEvent& event);

	void onCodeCOC(wxCommandEvent& event);

	void initMenuBar();
	void initStatueBar();

	wxMenu* initFileBar();
	wxMenu* initHelpBar();
	wxMenu* initCodesBar();

	void clear();

	void setCurrFilename();

private:
	Task* m_task;

	wxString m_currFilename;

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYPARTICLE3D_FRAME_H_
