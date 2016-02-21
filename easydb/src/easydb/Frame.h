#ifndef _EASYDB_FRAME_H_
#define _EASYDB_FRAME_H_

#include <wx/frame.h>

namespace edb
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

	void OnConnect(wxCommandEvent& event);

	void OnQuit(wxCommandEvent& event);

	void InitMenuBar();

	wxMenu* InitFileBar();
	wxMenu* InitHelpBar();

	void Clear();

	void SetCurrFilename();

private:
	enum
	{
		ID_CONNECT = 1000
	};

private:
	Task* m_task;

	std::string m_curr_filename;

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYDB_FRAME_H_