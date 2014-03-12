#pragma once

#include "interfaces.h"

#include <wx/wx.h>
#include <deque>

namespace d2d
{

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxString& filetag);

	void setTask(ITask* task);

	void initWithFile(const wxString& path);

protected:
	virtual void onNew(wxCommandEvent& event);
	virtual void onOpen(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onSaveAs(wxCommandEvent& event);

	virtual void onSettings(wxCommandEvent& event);

private:
	void onOpenRecent1(wxCommandEvent& event);
	void onOpenRecent2(wxCommandEvent& event);
	void onOpenRecent3(wxCommandEvent& event);
	void onOpenRecent4(wxCommandEvent& event);
	void onOpenRecent5(wxCommandEvent& event);

	void onEJPreview(wxCommandEvent& event);
	void onScreenshot(wxCommandEvent& event);

	void onQuit(wxCommandEvent& event);

	void onClose(wxCloseEvent& event);

	void initMenuBar();

	wxMenu* initFileBar();
	wxMenu* initViewBar();
	wxMenu* initSettingsBar();
	wxMenu* initHelpBar();

	wxString getFileFilter() const;

	void setCurrFilename();

	void openFile(const wxString& filename);

protected:
	wxMenu* m_setting_menu;

	ITask* m_task;

private:
	enum
	{
		ID_EJ_PREVIEW = 1000,
		ID_SCREENSHOT,
		ID_SETTINGS,
		ID_RECENT_FILES = 1050,
	};

private:
	class RecentFiles
	{
	public:
		RecentFiles(wxFrame* frame);

		wxMenu* getMenu() { return m_menu; }

		void insert(const wxString& filename);

		wxString getFilename(int idx) const {
			if (idx >= 0 && idx < m_files.size()) {
				return m_files[idx];
			} else {
				return wxEmptyString;
			}
		}

	private:
		void openFile(wxCommandEvent& event);

		void insertOnlyMenu(const wxString& filename);

	private:
		static const int CAPACITY = 2;

	private:
		wxFrame* m_frame;

		wxMenu* m_menu;

		std::deque<wxString> m_files;

	}; // RecentFiles

private:
	wxString m_filetag;

	wxString m_currFilename;

	RecentFiles m_recent;

	DECLARE_EVENT_TABLE()

}; // Frame 

}