#ifndef _EASYDB_FRAME_H_
#define _EASYDB_FRAME_H_

#include <wx/wx.h>

namespace edb
{

class Task;

class Frame : public wxFrame
{
public:
	Frame(const std::string& title);
	virtual ~Frame();

private:
	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);

	void OnConnect(wxCommandEvent& event);
	void OnSearch(wxCommandEvent& event);

	void OnQuit(wxCommandEvent& event);

	void InitMenuBar();

	wxMenu* InitFileBar();
	wxMenu* InitToolBar();
	wxMenu* InitHelpBar();

	void Clear();

	void SetCurrFilename();

private:
	enum
	{
		ID_CONNECT = 1000,
		ID_SEARCH,
	};

private:
	Task* m_task;

	std::string m_curr_filename;

#ifdef _DEBUG
	wxLogChain* m_log_chain;
#endif

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYDB_FRAME_H_