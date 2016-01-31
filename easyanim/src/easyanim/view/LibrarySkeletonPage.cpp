#include "LibrarySkeletonPage.h"
#include "LibrarySkeletonList.h"
#include "SkeletonEditDialog.h"
#include "StagePanel.h"

#include "dataset/WholeSkeleton.h"
#include "frame/Context.h"

namespace eanim
{

LibrarySkeletonPage::LibrarySkeletonPage(wxWindow* parent)
	: ee::ILibraryPage(parent, "Skeleton")
{
	initLayout();

	wxSizer* sizer = GetSizer();
	m_list = new LibrarySkeletonList(this);
	sizer->Add(m_list, 1, wxEXPAND);
	SetSizer(sizer);
}

bool LibrarySkeletonPage::isHandleSymbol(ee::ISymbol* symbol) const
{
	return dynamic_cast<WholeSkeleton*>(symbol) != NULL;
}

void LibrarySkeletonPage::onAddPress(wxCommandEvent& event)
{
	WholeSkeleton* skeleton = new WholeSkeleton;
	m_list->insert(skeleton);

	SkeletonEditDialog dlg(this, skeleton);
	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}

} // eanim