#ifndef _EASYIMAGE_FRAME_H_
#define _EASYIMAGE_FRAME_H_

#include <drag2d.h>

namespace eimage
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title);

protected:
	virtual void onOpen(wxCommandEvent& event);
	virtual void onSave(wxCommandEvent& event);
	virtual void onSaveAs(wxCommandEvent& event);

}; // Frame

}

#endif // _EASYIMAGE_FRAME_H_
