#ifndef _EASYCOMPLEX3D_TASK_H_
#define _EASYCOMPLEX3D_TASK_H_

#include <ee/Task.h>

class wxFrame;
class wxWindow;

namespace ee { class LibraryPanel; class ToolbarPanel; }
namespace ee3 { class PropertySettingPanel; }

namespace ecomplex3d
{

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

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	ee::LibraryPanel* m_library;
	ee3::PropertySettingPanel* m_property;
	ee::EditPanel* m_stage;
	ee::ToolbarPanel* m_toolbar;

}; // Task

}

#endif // _EASYCOMPLEX3D_TASK_H_
