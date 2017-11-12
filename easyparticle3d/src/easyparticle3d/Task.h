#ifndef _EASYPARTICLE3D_TASK_H_
#define _EASYPARTICLE3D_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace eparticle3d
{

class LibraryPanel;
class PropertySettingPanel;
class StagePanel;
class ToolbarPanel;

class Task : public ee::Task
{
public:
	Task(wxFrame* parent);
	virtual ~Task();

	virtual void Load(const char* filepath) override;
	virtual void Store(const char* filepath) const override;

	virtual bool IsDirty() const override;

	virtual void GetAllSprite(std::vector<ee::SprPtr>& sprs) const override {}

	virtual const ee::EditPanel* GetEditPanel() const override;

	void StoreAsAnim(const std::string& filepath) const;
	void StoreAsInvert(const std::string& filepath) const;

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	LibraryPanel* m_library;
	PropertySettingPanel* m_property;
	StagePanel* m_stage;
	ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYPARTICLE3D_TASK_H_
