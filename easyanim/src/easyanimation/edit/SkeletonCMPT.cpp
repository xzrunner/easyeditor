#include "SkeletonCMPT.h"
#include "SkeletonOP.h"

#include "view/StagePanel.h"

namespace eanim
{

SkeletonCMPT::SkeletonCMPT(wxWindow* parent, const wxString& name, 
	StagePanel* stage, d2d::PropertySettingPanel* property, bool vertical)
	: d2d::AbstractEditCMPT(parent, name, stage->GetStageImpl())
{
	m_editOP = new SkeletonOP(stage, property);
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