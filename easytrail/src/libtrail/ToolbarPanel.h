#ifndef _EASYTRAIL_TOOLBAR_PANEL_H_
#define _EASYTRAIL_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>
#include <ee/UICallback.h>
#include <ee/Observer.h>

#include <json/json.h>
#include <wx/dnd.h>

namespace ee { class LibraryPanel; class SliderCtrl; class Symbol; }

namespace etrail
{

class StagePanel;
class ComponentPanel;

class ToolbarPanel : public ee::ToolbarPanel, public ee::UICallback, public ee::Observer
{
public:
	ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library,
		StagePanel* stage);

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

	void Load(const Json::Value& val);
	void Store(Json::Value& val) const;

	void InitTrail();

	void OnDelChild(ComponentPanel* child);

protected:
	virtual wxSizer* InitLayout();

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	wxSizer* CreateMainLayout();
	wxSizer* CreateComponentLayout();

	void Clear();

	void OnChangeMode(wxCommandEvent& event);

	ComponentPanel* OnAddChild(ee::Symbol* symbol);
	void OnDelAllChild(wxCommandEvent& event);

private:
	class DropTarget : public wxTextDropTarget
	{
	public:
		DropTarget(ee::LibraryPanel* library, StagePanel* stage,
			ToolbarPanel* toolbar);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		ee::LibraryPanel* m_library;
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // DropTarget

private:
	StagePanel* m_stage;

	wxChoice* m_mode_choice;

	std::vector<ee::SliderCtrl*> m_sliders;

	wxSizer* m_comp_sizer;
	std::vector<ComponentPanel*> m_children;

}; // ToolbarPanel

}

#endif // _EASYTRAIL_TOOLBAR_PANEL_H_