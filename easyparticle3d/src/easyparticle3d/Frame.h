#ifndef _EASYPARTICLE3D_FRAME_H_
#define _EASYPARTICLE3D_FRAME_H_

#include <ee/Frame.h>

namespace eparticle3d
{

class Task;

class Frame : public ee::Frame
{
public:
	Frame(const std::string& title, const std::string& filetag);

protected:
	virtual void OnSaveAs(wxCommandEvent& event);

private:
	void SaveAsParticle3d(const std::string& filepath) const;
	void SaveAsAnim(const std::string& filepath) const;
	void SaveAsInvert(const std::string& filepath) const;

}; // Frame

}

#endif // _EASYPARTICLE3D_FRAME_H_
