#ifndef _EASYDB_TASK_H_
#define _EASYDB_TASK_H_

class wxFrame;
class wxWindow;

namespace edb
{

class MainPanel;
class StagePanel;
class SearchPanel;
class WarnPanel;

class Task
{
public:
	
	virtual void LoadFromFile(const char* filename) override;
	virtual void StoreToFile(const char* filename) const override;

	virtual void Clear() override;

	MainPanel* GetMainPanel() { return m_main_panel; }
	StagePanel* GetStagePanel() { return m_stage_panel; }
	SearchPanel* GetSearchPanel() { return m_search_panel; }		
	WarnPanel* GetWarnPanel() { return m_warn_panel; }

	static Task* Create(wxFrame* parent)
	{
		return new Task(parent);
	}

protected:
	Task(wxFrame* parent);
	~Task();

private:
	void InitLayout();
	wxWindow* InitLayoutLeft(wxWindow* parent);
	wxWindow* InitLayoutCenter(wxWindow* parent);
	wxWindow* InitLayoutRight(wxWindow* parent);

private:
	wxWindow* m_root;

	wxFrame* m_parent;

	MainPanel* m_main_panel;
	StagePanel* m_stage_panel;
	SearchPanel* m_search_panel;
	WarnPanel* m_warn_panel;

}; // Task

}

#endif // _EASYDB_TASK_H_