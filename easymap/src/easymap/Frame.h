#ifndef _EASYMAP_FRAME_H_
#define _EASYMAP_FRAME_H_

#include <drag2d.h>

namespace emap
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

private:
	void OnExtendSetting(wxCommandEvent& event);

private:
	enum
	{
		ID_SETING_EXTEND = 1100,
	};

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYMAP_FRAME_H_
