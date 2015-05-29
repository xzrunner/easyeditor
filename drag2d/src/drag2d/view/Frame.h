#pragma once

#include "interfaces.h"

#include <wx/wx.h>
#include <json/json.h>
#include <deque>

#include "common/SpecialConfig.h"

namespace d2d
{

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxString& filetag, const wxSize& size = wxSize(800, 600));
	virtual ~Frame();

	void setTask(ITask* task);

	void initWithFile(const wxString& path);

	void openFile(const wxString& filename);

	// F5 refresh with curr file
	void RefreshWithCurrFile();

protected:
	virtual void onNew(wxCommandEvent& event);
	virtual void onOpen(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onSaveAs(wxCommandEvent& event);

	virtual void onSettings(wxCommandEvent& event);

	wxString GetFileFilter() const;

private:
	void saveTmpInfo();
	void loadTmpInfo();

	void onOpenRecent1(wxCommandEvent& event);
	void onOpenRecent2(wxCommandEvent& event);
	void onOpenRecent3(wxCommandEvent& event);
	void onOpenRecent4(wxCommandEvent& event);
	void onOpenRecent5(wxCommandEvent& event);

	void onEJPreview(wxCommandEvent& event);

	void onQuit(wxCommandEvent& event);

	void onClose(wxCloseEvent& event);

	void initMenuBar();
	void initStatueBar();

	wxMenu* initFileBar();
	wxMenu* initViewBar();
	wxMenu* initSettingsBar();
	wxMenu* InitCodeBar();
	wxMenu* initHelpBar();

	void setCurrFilename();

	void LoadWindowConfig();
	void StoreWindowConfig();

protected:
	wxMenu* m_view_menu;
	wxMenu* m_setting_menu;
	wxMenu* m_code_menu;

	ITask* m_task;

private:
	enum
	{
		ID_VIEWS = 1000,
		ID_SETTINGS,
		ID_RECENT_FILES = 1050,
	};

private:
	class RecentFiles
	{
	public:
		RecentFiles(wxFrame* frame);

		void save(Json::Value& value) const;
		void load(const Json::Value& value);

		wxMenu* getMenu() { return m_menu; }

		void insert(const wxString& filename);

		wxString getFilename(int idx) const {
			if (idx >= 0 && idx < (int)m_files.size()) {
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

protected:
	wxString m_filetag;
	mutable wxString m_currFilename;

private:
	RecentFiles m_recent;

	SpecialConfig m_config;

#ifdef _DEBUG
	wxLogChain* m_log_chain;
#endif

private:
	DECLARE_EVENT_TABLE()

}; // Frame 

}