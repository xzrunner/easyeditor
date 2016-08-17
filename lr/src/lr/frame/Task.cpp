#include "Task.h"
#include "FileIO.h"
#include "Frame.h"
#include "ToolBar.h"

#include "view/LibraryPanel.h"
#include "view/StagePanel.h"
#include "view/ViewlistListImpl.h"
#include "dataset/Layer.h"

#include <ee/panel_msg.h>
#include <ee/EditOP.h>
#include <ee/SymbolMgr.h>
#include <ee/Bitmap.h>
#include <ee/FetchAllVisitor.h>
#include <ee/PropertySettingPanel.h>
#include <ee/ViewlistPanel.h>
#include <ee/LayerPanel.h>
#include <ee/GroupTreePanel.h>
#include <ee/Sprite.h>
#include <ee/Shape.h>

namespace lr
{

static const float LEFT_POS = 0.2f;
static const float RIGHT_POS = 0.75f;

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
	, m_full_view(false)
{
	InitLayout();

	Layer* layer = m_library->GetLayer(0);
	ee::ChangeLayerMgrSJ::Instance()->Change(layer->GetLayerMgr());

	m_stage->GetBaseOP()->OnActive();

	static_cast<Frame*>(parent)->GetToolBar()->SetLibrary(m_library);
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	ee::BitmapMgr::Instance()->Clear();
	delete m_root;
}

void Task::Load(const char* filename)
{
	FileIO::Load(filename, m_library, m_stage, m_grouptree);
}

void Task::Store(const char* filename) const
{
	FileIO::Store(filename, m_library, m_stage, m_grouptree);
}

bool Task::IsDirty() const
{
	return true;
}

void Task::GetAllSprite(std::vector<const ee::Sprite*>& sprs) const
{
	m_stage->TraverseSprites(ee::FetchAllVisitor<const ee::Sprite>(sprs));
}

const ee::EditPanel* Task::GetEditPanel() const
{
	return m_stage;
}

void Task::OnFullView()
{
	int w, h;
	m_parent->GetSize(&w, &h);
	if (m_full_view) {
		m_left_split->SetSashPosition(w * LEFT_POS);
		m_right_split->SetSashPosition(w * RIGHT_POS);
	} else {
		m_right_split->SetSashPosition(w - 1);
		m_left_split->SetSashPosition(1);
	}
	m_full_view = !m_full_view;
}

void Task::InitLayout()
{
	m_right_split = new wxSplitterWindow(m_parent);
	m_left_split = new wxSplitterWindow(m_right_split);

	wxWindow* left = InitLayoutLeft(m_left_split);
	wxWindow* center = InitLayoutCenter(m_left_split);
	wxWindow* right = InitLayoutRight(m_right_split);

	m_left_split->SetSashGravity(LEFT_POS);
	m_left_split->SplitVertically(left, center);

	m_right_split->SetSashGravity(RIGHT_POS);
	m_right_split->SplitVertically(m_left_split, right);

	m_root = m_right_split;
}

wxWindow* Task::InitLayoutLeft(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);

	m_library = new LibraryPanel(split);
	m_property = new ee::PropertySettingPanel(split);

	split->SetSashGravity(0.75f);
	split->SplitHorizontally(m_library, m_property);

	return split;
}

wxWindow* Task::InitLayoutCenter(wxWindow* parent)
{
	m_stage = new StagePanel(parent, m_parent, m_property, m_library);

	m_library->SetStagePanel(m_stage);
	m_library->InitPages(m_stage, m_property);
	m_property->SetEditPanel(m_stage->GetStageImpl());

	return m_stage;
}

wxWindow* Task::InitLayoutRight(wxWindow* parent)
{
	wxSplitterWindow* split = new wxSplitterWindow(parent);
	m_viewlist = new ee::ViewlistPanel(split);
	m_viewlist->SetListImpl(new ViewlistListImpl(m_stage, m_stage->GetStageImpl(), m_stage));
	m_library->SetViewlist(m_viewlist);

	wxSplitterWindow* right_split = new wxSplitterWindow(split);
	m_layer = new ee::LayerPanel(right_split);

	m_grouptree = new ee::GroupTreePanel(right_split, m_stage, m_stage->GetKeyState());
	m_library->SetGroupTree(m_grouptree);

	right_split->SetSashGravity(0.5f);
	right_split->SplitHorizontally(m_layer, m_grouptree);

	split->SetSashGravity(0.5f);
	split->SplitVertically(right_split, m_viewlist);

	return split;
}

}