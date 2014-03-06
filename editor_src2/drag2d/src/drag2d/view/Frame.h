#pragma once

#include "interfaces.h"

#include <wx/wx.h>

namespace d2d
{

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxString& filetag);

	void setTask(ITask* task);

	void initWithFile(const wxString& path);

protected:
	virtual void onSettings(wxCommandEvent& event);

private:
	void onNew(wxCommandEvent& event);
	void onOpen(wxCommandEvent& event);
	void onSave(wxCommandEvent& event);
	void onSaveAs(wxCommandEvent& event);

	void onEJPreview(wxCommandEvent& event);

	void onQuit(wxCommandEvent& event);

	void onClose(wxCloseEvent& event);

	void initMenuBar();

	wxMenu* initFileBar();
	wxMenu* initViewBar();
	wxMenu* initSettingsBar();
	wxMenu* initHelpBar();

	wxString getFileFilter() const;

	void setCurrFilename();

protected:
	wxMenu* m_setting_menu;

	ITask* m_task;

private:
	enum
	{
		ID_EJ_PREVIEW = 1000,
		ID_SETTINGS
	};

private:
	wxString m_filetag;

	wxString m_currFilename;

	DECLARE_EVENT_TABLE()

}; // Frame 

}