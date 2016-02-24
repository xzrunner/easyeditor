#ifndef _EASYTEXPACKER_FRAME_H_
#define _EASYTEXPACKER_FRAME_H_

#include <wx/wx.h>

namespace etexpacker
{

class Task;

class Frame : public wxFrame
{
public:
	Frame(const std::string& title);

private:
	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);

	void OnQuit(wxCommandEvent& event);
	//		void onAbout(wxCommandEvent& event);

	void OnCodeLove2d(wxCommandEvent& event);

	void InitMenuBar();
	void InitStatueBar();

	wxMenu* InitFileBar();
	wxMenu* InitCodesBar();
	wxMenu* InitHelpBar();

	void Clear();

	void SetCurrFilename();

private:
	Task* m_task;

	std::string m_curr_filename;

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYTEXPACKER_FRAME_H_