#include "SkeletonCMPT.h"
#include "SkeletonOP.h"

#include "view/StagePanel.h"

namespace eanim
{
	SkeletonCMPT::SkeletonCMPT(wxWindow* parent, const wxString& name, StagePanel* stage)
		: d2d::AbstractEditCMPT(parent, name, stage)
	{
		m_editOP = new SkeletonOP(stage);
	}

	wxSizer* SkeletonCMPT::initLayout()
	{
		return NULL;
	}

	wxSizer* SkeletonCMPT::initEditPanel()
	{
		return NULL;
	}
}