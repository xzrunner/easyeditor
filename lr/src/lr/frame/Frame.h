#ifndef _LR_FRAME_H_
#define _LR_FRAME_H_

#include <drag2d.h>

namespace lr
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

private:
	void OnPreview(wxCommandEvent& event);
	void OnExtendSetting(wxCommandEvent& event);

private:
	enum
	{
		ID_PREVIEW = 1100,
		ID_SETING_EXTEND,
	};

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _LR_FRAME_H_
