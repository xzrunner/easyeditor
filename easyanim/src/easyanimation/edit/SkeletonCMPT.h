#ifndef _EASYANIM_SKELETON_CMPT_H_
#define _EASYANIM_SKELETON_CMPT_H_

#include <drag2d.h>

namespace eanim
{

class StagePanel;

class SkeletonCMPT : public d2d::AbstractEditCMPT
{
public:
	SkeletonCMPT(wxWindow* parent, const wxString& name, bool vertical);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* InitEditPanel();

}; // SkeletonCMPT

}

#endif // _EASYANIM_SKELETON_CMPT_H_