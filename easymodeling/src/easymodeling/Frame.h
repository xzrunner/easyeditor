#ifndef _EASYMODELING_FRAME_H_
#define _EASYMODELING_FRAME_H_

#include <wx/wx.h>

namespace emodeling
{

class Task;

class Frame : public wxFrame
{
public:
	Frame(const std::string& title);

private:
	void onNew(wxCommandEvent& event);
	void onOpen(wxCommandEvent& event);
	void onSave(wxCommandEvent& event);
	void onSaveAs(wxCommandEvent& event);

	void onQuit(wxCommandEvent& event);
	void onAbout(wxCommandEvent& event);

	void onSettingOperatingMenu(wxCommandEvent& event);
	void onSettingViewMenu(wxCommandEvent& event);

	void onPreview(wxCommandEvent& event);

	void onCodeLove2d(wxCommandEvent& event);

	void initMenuBar();

	wxMenu* initFileBar();

	wxMenu* initViewBar();
	wxMenu* initCodesBar();
	wxMenu* initSettingsBar();
	wxMenu* initHelpBar();

	void setCurrFilename();

private:
	Task* m_task;

	std::string m_filename;

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYMODELING_FRAME_H_