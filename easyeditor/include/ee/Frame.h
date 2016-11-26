#ifndef _EASYEDITOR_FRAME_H_
#define _EASYEDITOR_FRAME_H_

#include "SpecialConfig.h"

#include <wx/wx.h>

class wxSocketClient;

namespace ee
{

class Task;
class RecentFilesMenu;

class Frame : public wxFrame
{
public:
	Frame(const std::string& title, const std::string& filetag, const wxSize& size = wxSize(800, 600));
	virtual ~Frame();

	void SetTask(Task* task);

	void InitWithFile(const std::string& path);

	void OpenFile(const std::string& filename);

	// F5 refresh with curr file
	void RefreshWithCurrFile();

protected:
	virtual void OnNew(wxCommandEvent& event);
	virtual void OnOpen(wxCommandEvent& event);
	virtual void OnSave(wxCommandEvent& event);
	virtual void OnSaveAs(wxCommandEvent& event);

	virtual void OnFullView(wxCommandEvent& event);
	virtual void OnSettings(wxCommandEvent& event);
	virtual void OnConnDefault(wxCommandEvent& event);
	virtual void OnConnTest(wxCommandEvent& event);

	virtual std::string GetFileFilter() const;

private:
	void Clear();

	void SaveTmpInfo();
	void LoadTmpInfo();

	void OnOpenRecentFile(wxCommandEvent& event);

	void OnEJPreview(wxCommandEvent& event);

	void OnQuit(wxCommandEvent& event);

	void OnClose(wxCloseEvent& event);

	void InitMenuBar();
	void InitStatueBar();

	wxMenu* InitFileBar();
	wxMenu* InitViewBar();
	wxMenu* InitSettingsBar();
	wxMenu* InitCodeBar();
	wxMenu* InitConnBar();
	wxMenu* InitHelpBar();

	void SetCurrFilename();

	void LoadWindowConfig();
	void StoreWindowConfig();

protected:
	wxMenu* m_view_menu;
	wxMenu* m_setting_menu;
	wxMenu* m_code_menu;
	wxMenu* m_conn_menu;

	Task* m_task;

private:
	enum
	{
		ID_FULL_VIEWS		= 1000,
		ID_SETTINGS,
		ID_CONN_DEFAULT,
		ID_CONN_TEST,

		ID_RECENT_FILENAME	= 8000,
	};

protected:
	std::string m_filetag;
	mutable std::string m_curr_filename;

private:
	RecentFilesMenu* m_recent_menu;

	SpecialConfig m_config;

	wxSocketClient* m_sock;

#ifdef _DEBUG
	wxLogChain* m_log_chain;
#endif

private:
	DECLARE_EVENT_TABLE()

}; // Frame 

}

#endif // _EASYEDITOR_FRAME_H_