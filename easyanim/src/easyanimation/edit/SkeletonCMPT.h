#ifndef _EASYANIM_SKELETON_CMPT_H_
#define _EASYANIM_SKELETON_CMPT_H_

#include <ee/EditCMPT.h>

namespace eanim
{

class StagePanel;

class SkeletonCMPT : public ee::EditCMPT
{
public:
	SkeletonCMPT(wxWindow* parent, const std::string& name, bool vertical);

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* InitEditPanel();

}; // SkeletonCMPT

}

#endif // _EASYANIM_SKELETON_CMPT_H_