#pragma once

#include <ee/Task.h>

#include <wx/aui/framemanager.h>

class wxFrame;
class wxWindow;

namespace ee { class LibraryPanel; }

namespace eone
{

class StagePanel;

class SceneTreeCtrl;

class Task : public ee::Task
{
public:
	Task(wxFrame* frame);
	virtual ~Task();

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override {}

	virtual const ee::EditPanel* GetEditPanel() const override { return nullptr; }

private:
	void InitLayout();
	void InitCallback();

	wxWindow* CreateLibraryPanel();
	wxWindow* CreateStagePanel();
	wxWindow* CreateTreePanel();
	wxWindow* CreateDetailPanel();

private:
	wxFrame* m_frame;

	wxAuiManager m_mgr;

	ee::LibraryPanel* m_library;

	StagePanel* m_stage;

	SceneTreeCtrl* m_tree;

}; // Task

}