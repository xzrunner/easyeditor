#ifndef _EASYPARTICLE2D_FRAME_H_
#define _EASYPARTICLE2D_FRAME_H_

#include <drag2d.h>

namespace eparticle2d
{

class Frame : public d2d::Frame
{
public:
	Frame(const wxString& title, const wxString& filetag);

}; // Frame

}

#endif // _EASYPARTICLE2D_FRAME_H_
