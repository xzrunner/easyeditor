#include "Task.h"
#include "StagePanel.h"
#include "ToolbarPanel.h"
#include "FileIO.h"
#include "Context.h"

#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/FileHelper.h>
#include <ee/panel_msg.h>
#include <ee/LibraryPanel.h>
#include <ee/PropertySettingPanel.h>
#include <ee/LibraryImagePage.h>

#include <wx/splitter.h>

namespace etexpacker
{

Task::Task(wxFrame* parent)
	: m_root(NULL)
	, m_parent(parent)
{
	InitLayout();
}

Task::~Task()
{
	ee::SymbolMgr::Instance()->Clear();
	delete m_root;
}

void Task::LoadFromFile(const char* filename)
{
	if (!ee::FileHelper::IsFileExist(filename)) {
		throw ee::Exception("File: %s don't exist!", filename);
	}
	FileIO::Load(filename);
}

void Task::StoreToFile(const char* filename) const
{
	FileIO::Store(filename);
}

void Task::Clear()
{
	ee::ClearPanelSJ::Instance()->Clear();
}

void Task::InitLayout()
{
	wxSplitterWindow* right_splitter = new wxSplitterWindow(m_parent);
	wxSplitterWindow* left_vert_splitter = new wxSplitterWindow(right_splitter);
	wxSplitterWindow* left_hori_splitter = new wxSplitterWindow(left_vert_splitter);

	Context* context = Context::Instance();

	wxWindow* library = context->library = new ee::LibraryPanel(left_hori_splitter);
	context->library->AddPage(new ee::LibraryImagePage(context->library->GetNotebook()));

	wxWindow* property = context->property = new ee::PropertySettingPanel(left_hori_splitter);

	wxWindow* stage = context->stage = new StagePanel(left_vert_splitter, m_parent);
	context->property->SetEditPanel(context->stage->GetStageImpl());

	wxWindow* toolbar = context->toolbar = new ToolbarPanel(right_splitter, context->stage);

	if (library || property)
	{
		if (library && property)
		{
			left_hori_splitter->SetSashGravity(0.8f);
			left_hori_splitter->SplitHorizontally(library, property);
		}
		left_vert_splitter->SetSashGravity(0.15f);
		left_vert_splitter->SplitVertically(left_hori_splitter, stage);
	}

	if (toolbar)
	{
		right_splitter->SetSashGravity(0.83f);
		right_splitter->SplitVertically(left_vert_splitter, toolbar);
	}

	m_root = right_splitter;
}

}
