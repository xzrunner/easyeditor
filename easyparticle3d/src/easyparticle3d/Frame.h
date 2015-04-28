#ifndef _EASYPARTICLE3D_FRAME_H_
#define _EASYPARTICLE3D_FRAME_H_

#include <drag2d.h>

namespace eparticle3d
{

class Task;

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

private:
	void onCodeCOC(wxCommandEvent& event);

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYPARTICLE3D_FRAME_H_
