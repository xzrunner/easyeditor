#include "Task.h"
#include "LibraryPanel.h"
#include "StagePanel.h"
#include "SceneTreeCtrl.h"
#include "DetailPanel.h"

#include <ee0/CompEditor.h>
#include <ee2/StagePage.h>
#include <ee2/StageCanvas.h>
#include <ee2/NodeSelectOP.h>
#include <ee3/StagePage.h>
#include <ee3/StageCanvas.h>
#include <ee3/NodeArrangeOP.h>
#include <ee3/Serializer.h>

#include <node3/ComponentFactory.h>

namespace eone
{

Task::Task(wxFrame* frame)
	: m_frame(frame)
{
	m_mgr.SetManagedWindow(frame);

	InitLayout();
	InitCallback();
}

Task::~Task()
{
}

void Task::Load(const char* filename)
{
//	ee3::Serializer::LoadFroimJson(filename, m_stage);

	m_tree->ExpandAll();
}

void Task::Store(const char* filename) const
{
//	ee3::Serializer::StoreToJson(filename, m_stage);
}

bool Task::IsDirty() const
{
	return false;
}

void Task::InitLayout()
{
	m_mgr.AddPane(CreateLibraryPanel(),
		wxAuiPaneInfo().Name("Library").Caption("Library").
		Left().MinSize(wxSize(100, 0)));
	
	m_mgr.GetPane("Stage");

	m_mgr.AddPane(CreateStagePanel(),
		wxAuiPaneInfo().Name("Stage").Caption("Stage").
		CenterPane().PaneBorder(false));

	m_mgr.AddPane(CreateTreePanel(),
		wxAuiPaneInfo().Name("Tree").Caption("Tree").
		Right().Row(1).MinSize(200, 0).PaneBorder(false));

	m_mgr.AddPane(CreateDetailPanel(),
		wxAuiPaneInfo().Name("Detail").Caption("Detail").
		Right().MinSize(300, 0).PaneBorder(false));

	m_mgr.Update();
}

void Task::InitCallback()
{
	n3::ComponentFactory::Instance()->AddCreator(ee0::CompEditor::TYPE_NAME,
		[](n0::SceneNodePtr& node, const rapidjson::Value& val)
	{
		auto& comp = node->AddComponent<ee0::CompEditor>();
		comp.LoadFromJson(val);
	});
}

wxWindow* Task::CreateLibraryPanel()
{
	m_library = new LibraryPanel(m_frame);
	return m_library;
}

wxWindow* Task::CreateStagePanel()
{
	m_stage = new StagePanel(m_frame);
	m_stage->Freeze();

	wxGLContext* gl_ctx = nullptr;
	{
		auto page = new ee2::StagePage(m_frame, m_frame, m_library);
		auto canvas = std::make_shared<ee2::StageCanvas>(page);
		gl_ctx = canvas->GetGLContext();
		page->SetCanvas(canvas);
		page->SetEditOP(std::make_shared<ee2::NodeSelectOP>(*page));

		m_stage->AddPage(page, ("New 2d"));
	}
	{
		auto page = new ee3::StagePage(m_frame, m_frame, m_library);
		auto canvas = std::make_shared<ee3::StageCanvas>(page, gl_ctx);
		page->SetCanvas(canvas);
		page->SetEditOP(std::make_shared<ee3::NodeArrangeOP>(*page));

		m_stage->AddPage(page, ("New 3d"));
	}

	m_stage->Thaw();

	return m_stage;
}

wxWindow* Task::CreateTreePanel()
{
	auto& sub_mgr = m_stage->GetCurrentStagePage()->GetSubjectMgr();
	m_tree = new SceneTreeCtrl(m_frame, sub_mgr);
	return m_tree;
}

wxWindow* Task::CreateDetailPanel()
{
	auto& sub_mgr = m_stage->GetCurrentStagePage()->GetSubjectMgr();
	return new DetailPanel(m_frame, sub_mgr);
}

}