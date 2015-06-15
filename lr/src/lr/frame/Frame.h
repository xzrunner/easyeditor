#ifndef _LR_FRAME_H_
#define _LR_FRAME_H_

#include <drag2d.h>

namespace lr
{

class ToolBar;

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

	ToolBar* GetToolBar() { return m_toolbar; }

protected:
	void OnToolBarClick(wxCommandEvent& event);

private:
	void OnPreview(wxCommandEvent& event);
	void OnExtendSetting(wxCommandEvent& event);

private:
	enum
	{
		ID_PREVIEW = 1100,
		ID_SETING_EXTEND,

		ID_TOOLBAR = 1200,
	};

private:
	ToolBar* m_toolbar;

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _LR_FRAME_H_
