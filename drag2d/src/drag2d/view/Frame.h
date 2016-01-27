#ifndef _DRAG2D_FRAME_H_
#define _DRAG2D_FRAME_H_

#include "interfaces.h"

#include <wx/wx.h>

#include "common/SpecialConfig.h"

namespace d2d
{

class RecentFilesMenu;

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxString& filetag, const wxSize& size = wxSize(800, 600));
	virtual ~Frame();

	void SetTask(ITask* task);

	void InitWithFile(const wxString& path);

	void OpenFile(const wxString& filename);

	// F5 refresh with curr file
	void RefreshWithCurrFile();

protected:
	virtual void OnNew(wxCommandEvent& event);
	virtual void OnOpen(wxCommandEvent& event);
	virtual void OnSave(wxCommandEvent& event);
	virtual void OnSaveAs(wxCommandEvent& event);

	virtual void OnFullView(wxCommandEvent& event);
	virtual void OnSettings(wxCommandEvent& event);

	wxString GetFileFilter() const;
	static wxString GetJsonFileFilter(const wxString& file_tag);

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
	wxMenu* InitHelpBar();

	void SetCurrFilename();

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
		ID_FULL_VIEWS		= 1000,
		ID_SETTINGS,

		ID_RECENT_FILENAME	= 8000,
	};

protected:
	wxString m_filetag;
	mutable std::string m_curr_filename;

private:
	RecentFilesMenu* m_recent_menu;

	SpecialConfig m_config;

#ifdef _DEBUG
	wxLogChain* m_log_chain;
#endif

private:
	DECLARE_EVENT_TABLE()

}; // Frame 

}

#endif // _DRAG2D_FRAME_H_