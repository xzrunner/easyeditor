#include "Task.h"
#include "FileIO.h"

#include "dataset/DataMgr.h"
#include "view/PropertySettingPanel.h"
#include "view/StagePanel.h"
#include "view/ToolbarPanel.h"
#include "view/TimeLinePanel.h"
#include "view/ViewMgr.h"
#include "view/LibraryPanel.h"
#include "message/messages.h"

#include <ee/panel_msg.h>
#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/SymbolFile.h>
#include <ee/Exception.h>
#include <ee/ExceptionDlg.h>
#include <ee/FetchAllVisitor.h>
#include <ee/subject_id.h>
#include <ee/ViewlistPanel.h>
#include <ee/SymbolType.h>

#include <sprite2/SymType.h>

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

	RegistSubject(ee::ClearPanelSJ::Instance());
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filepath)
{
	int type = ee::SymbolFile::Instance()->Type(filepath);
	if (type != s2::SYM_ANIMATION && type != ee::SYM_ANIS) {
		return;
	}

	try {
		FileIO::Load(filepath);
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
}

void Task::Store(const char* filepath) const
{
	int type = ee::SymbolFile::Instance()->Type(filepath);
	switch (type)
	{
	case s2::SYM_ANIMATION:
		FileIO::StoreSingle(filepath);
		ViewMgr::Instance()->stage->OnSave();
		break;
	case ee::SYM_ANIS:
		FileIO::StoreTemplate(filepath);
		ViewMgr::Instance()->stage->OnSave();
		break;
	}
}

bool Task::IsDirty() const
{
	return ViewMgr::Instance()->stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<ee::Sprite*>& sprs) const
{
	ViewMgr::Instance()->stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return ViewMgr::Instance()->stage;
}

void Task::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case ee::MSG_CLEAR_PANEL:
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
	mgr->library = new LibraryPanel(split);;

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
	ee::ViewlistPanel* viewlist = new ee::ViewlistPanel(parent);
	ViewMgr::Instance()->viewlist = viewlist;
	return viewlist;
}

}