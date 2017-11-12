#ifndef _EASYPSEUDO3D_TASK_H_
#define _EASYPSEUDO3D_TASK_H_

#include <ee/Task.h>

#include <wx/splitter.h>

namespace ee { class LibraryPanel; class EditPanel; }

namespace epseudo3d
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

private:
	wxWindow* m_root;

	wxFrame* m_parent;

 	ee::LibraryPanel* m_library;
 	ee::EditPanel* m_stage;

}; // Task

}

#endif // _EASYPSEUDO3D_TASK_H_
