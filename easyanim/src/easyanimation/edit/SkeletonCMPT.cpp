#include "SkeletonCMPT.h"
#include "SkeletonOP.h"

#include "view/StagePanel.h"
#include "view/ViewMgr.h"

namespace eanim
{

SkeletonCMPT::SkeletonCMPT(wxWindow* parent, const std::string& name, bool vertical)
	: ee::EditCMPT(parent, name, ViewMgr::Instance()->stage->GetStageImpl())
{
	m_editop = new SkeletonOP(ViewMgr::Instance()->stage, ViewMgr::Instance()->property);
}

wxSizer* SkeletonCMPT::InitLayout()
{
	return NULL;
}

wxSizer* SkeletonCMPT::InitEditPanel()
{
	return NULL;
}

}