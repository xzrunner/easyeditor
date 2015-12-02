#include "Task.h"
#include "FileIO.h"

#include "view/PropertySettingPanel.h"
#include "view/StagePanel.h"
#include "view/ToolbarPanel.h"
#include "view/TimeLinePanel.h"
#include "message/InsertLayerSJ.h"

#include <easycomplex.h>
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
	, m_controller(&m_widgets)
{
	InitLayout();

	m_widgets.m_library->LoadFromConfig();

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

	m_controller.Clear();

	try {
		FileIO::Load(filepath, &m_controller);
	} catch (d2d::Exception& e) {
		d2d::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	d2d::ResetViewportSJ::Instance()->Reset();
}

void Task::Store(const char* filepath) const
{
	if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
		FileIO::StoreSingle(filepath, const_cast<Controller*>(&m_controller));
		m_widgets.m_stage->OnSave();
	} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anis)) {
		FileIO::StoreTemplate(filepath, const_cast<Controller*>(&m_controller));
		m_widgets.m_stage->OnSave();
	}
}

bool Task::IsDirty() const
{
	return m_widgets.m_stage->IsEditDirty();
}

void Task::GetAllSprite(std::vector<const d2d::ISprite*>& sprites) const
{
	m_widgets.m_stage->TraverseSprites(d2d::FetchAllVisitor<const d2d::ISprite>(sprites));
}

const d2d::EditPanel* Task::GetEditPanel() const
{
	return m_widgets.m_stage;
}

void Task::OnNotify(int sj_id, void* ud)
{
	if (sj_id == d2d::MSG_CLEAR_PANEL) {
		m_controller.Clear();
		m_widgets.m_library->Clear();
		InsertLayerSJ::Instance()->Insert();
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

	// library
	m_widgets.m_library = new d2d::LibraryPanel(split);
	wxWindow* nb = m_widgets.m_library->GetNotebook();
	m_widgets.m_library->AddPage(m_widgets.m_imagePage = new d2d::LibraryImagePage(nb));
	m_widgets.m_library->AddPage(new ecomplex::LibraryPage(nb));
	m_widgets.m_library->AddPage(new emesh::LibraryPage(nb));
	m_widgets.m_library->AddPage(new escale9::LibraryPage(nb));
	m_widgets.m_library->AddPage(new eicon::LibraryPage(nb));
	m_widgets.m_library->AddPage(new eparticle3d::LibraryPage(nb));

	// property
	m_widgets.m_property = new PropertySettingPanel(split);

	split->SetSashGravity(0.55f);
	split->SplitHorizontally(m_widgets.m_library, m_widgets.m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	wxSplitterWindow* bottom_split = new wxSplitterWindow(parent);
	wxSplitterWindow* top_split = new wxSplitterWindow(bottom_split);

	// stage
	m_widgets.m_stage = new StagePanel(top_split, m_parent, m_widgets.m_property, 
		&m_controller);
	m_widgets.m_property->SetEditPanel(m_widgets.m_stage->GetStageImpl());

	// toolbar
	m_widgets.m_toolbar = new ToolbarPanel(top_split, m_widgets.m_stage, 
		m_widgets.m_property, false, &m_controller);

	// timeline
	TimeLinePanel* timeline = new TimeLinePanel(bottom_split, &m_controller);

	top_split->SetSashGravity(0.1f);
	top_split->SplitHorizontally(m_widgets.m_toolbar, m_widgets.m_stage);

	bottom_split->SetSashGravity(0.9f);
	bottom_split->SplitHorizontally(top_split, timeline);

	return bottom_split;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	// viewlist
	m_widgets.m_viewlist = new d2d::ViewlistPanel(parent);
	return m_widgets.m_viewlist;
}

}