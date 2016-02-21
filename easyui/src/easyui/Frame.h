#ifndef _EASYUI_FRAME_H_
#define _EASYUI_FRAME_H_

#include <ee/Frame.h>

namespace eui
{

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title, const std::string& filetag);

private:
	void OnPreview(wxCommandEvent& event);
	void OnCode(wxCommandEvent& event);

private:
	enum
	{
		ID_PREVIEW = 1100,
		ID_SETING_EXTEND,
		ID_CODE,
	};

	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYUI_FRAME_H_
