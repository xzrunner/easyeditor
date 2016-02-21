#ifndef _LR_FRAME_H_
#define _LR_FRAME_H_

#include <ee/Frame.h>

namespace lr
{

class ToolBar;

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title, const std::string& filetag);

	ToolBar* GetToolBar() { return m_toolbar; }

protected:
	virtual void OnSaveAs(wxCommandEvent& event);
	virtual void OnFullView(wxCommandEvent& event);

	void OnToolBarClick(wxCommandEvent& event);

private:
	void OnPreview(wxCommandEvent& event);
	void OnExtendSetting(wxCommandEvent& event);

	void SaveAsPNG(const std::string& filepath) const;
	void SaveAsJson(const std::string& filepath) const;

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
