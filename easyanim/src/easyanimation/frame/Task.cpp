#include "Task.h"
#include "FileIO.h"

#include "dataset/DataMgr.h"
#include "view/PropertySettingPanel.h"
#include "view/StagePanel.h"
#include "view/ToolbarPanel.h"
#include "view/TimeLinePanel.h"
#include "view/ViewMgr.h"
#include "message/messages.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easymesh.h>
#include <easyscale9.h>
#include <easyicon.h>
#include <easyparticle3d.h>

#include <wx/splitter.h>

namespace eanim
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	DataMgr::Instance();

	InitLayout();

	InsertLayerSJ::Instance()->Insert();
	SetSelectedSJ::Instance()->Set(0, 0);

	ViewMgr::Instance()->library->LoadFromConfig();

	RegistSubject(d2d::ClearPanelSJ::Instance());
}

Task::~Task()
{
	d2d::SymbolMgr::Instance()->Clear();
	d2d::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	if (!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim) &&
		!d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anis)) {
		return;
	}

	try {
		FileIO::Load(filepath);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
}

void Task::Store(const char* filepath) const
{
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
		FileIO::StoreSingle(filepath);
		ViewMgr::Instance()->stage->OnSave();
	} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anis)) {
		FileIO::StoreTemplate(filepath);
		ViewMgr::Instance()->stage->OnSave();
	}
}

bool Task::IsDirty() const
{
	return ViewMgr::Instance()->stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
	ViewMgr::Instance()->stage->TraverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return ViewMgr::Instance()->stage;
}

void Task::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case d2d::MSG_CLEAR_PANEL:
		DataMgr::Instance()->GetLayers().Clear();
		ViewMgr::Instance()->library->Clear();
		InsertLayerSJ::Instance()->Insert();
		SetSelectedSJ::Instance()->Set(0, 0);
		break;
	}
}

void Task::InitLayout()
{
	wxSplitterWindow* right_split = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_split = new wxSplitterWindow(right_split);

	wxWindow* left = InitLayoutLeft(left_split);
	wxWindow* center = InitLayoutCenter(left_split);
	wxWindow* right = InitLayoutRight(right_split);

	left_split->SetSashGravity(0.12f);
	left_split->SplitVertically(left, center);

	right_split->SetSashGravity(0.9f);
	right_split->SplitVertically(left_split, right);

	m_root = right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	ViewMgr* mgr = ViewMgr::Instance();

	// library
	d2d::LibraryPanel* library = new d2d::LibraryPanel(split);
	wxWindow* nb = library->GetNotebook();
	library->AddPage(ViewMgr::Instance()->img_page = new d2d::LibraryImagePage(nb));
	library->AddPage(new ecomplex::LibraryPage(nb));
	library->AddPage(new libanim::LibraryPage(nb));
	library->AddPage(new emesh::LibraryPage(nb));
	library->AddPage(new escale9::LibraryPage(nb));
	library->AddPage(new eicon::LibraryPage(nb));
	library->AddPage(new eparticle3d::LibraryPage(nb));
	mgr->library = library;

	// property
	mgr->property = new PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(mgr->library, mgr->property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	wxSplitterWindow* bottom_split = new wxSplitterWindow(parent);
	wxSplitterWindow* top_split = new wxSplitterWindow(bottom_split);

	ViewMgr* mgr = ViewMgr::Instance();

	// stage
	mgr->stage = new StagePanel(top_split, m_parent);
	mgr->property->SetEditPanel(mgr->stage->GetStageImpl());

	// toolbar
	mgr->toolbar = new ToolbarPanel(top_split, false);

	// timeline
	TimeLinePanel* timeline = new TimeLinePanel(bottom_split);

	top_split->SetSashGravity(0.1f);
	top_split->SplitHorizontally(mgr->toolbar, mgr->stage);

	bottom_split->SetSashGravity(0.9f);
	bottom_split->SplitHorizontally(top_split, timeline);

	return bottom_split;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	// viewlist
	d2d::ViewlistPanel* viewlist = new d2d::ViewlistPanel(parent);
	ViewMgr::Instance()->viewlist = viewlist;
	return viewlist;
}

}