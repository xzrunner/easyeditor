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

protected:
	virtual void onSaveAs(wxCommandEvent& event);

private:
	void onCodeCOC(wxCommandEvent& event);

	void SaveAsParticle3d(const wxString& filepath) const;
	void SaveAsAnim(const wxString& filepath) const;
	void SaveAsInvert(const wxString& filepath) const;

private:
	DECLARE_EVENT_TABLE()

}; // Frame

}

#endif // _EASYPARTICLE3D_FRAME_H_
