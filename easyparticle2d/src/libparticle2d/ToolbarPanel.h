//#ifndef _EASYPARTICLE2D_TOOLBAR_PANEL_H_
//#define _EASYPARTICLE2D_TOOLBAR_PANEL_H_
//
//#include <drag2d.h>
//
//namespace eparticle2d
//{
//
//class StageData;
//class SettingPageBase;
//
//class ToolbarPanel : public d2d::ToolbarPanel
//{
//public:
//	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library,
//		d2d::EditPanel* stage, StageData* stage_data);
//
//	void Load(const Json::Value& val, int version);
//	void Store(Json::Value& val) const;
//
//	void ResetUIFromPS();
//	void InitPSFromUI();
//
//protected:
//	virtual wxSizer* initLayout();
//
//private:
//	class DropTarget : public wxTextDropTarget
//	{
//	public:
//		DropTarget(d2d::LibraryPanel* library, StageData* stage_data);
//
//		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);
//
//	private:
//		d2d::LibraryPanel* m_library;
//
//		StageData* m_stage_data;
//
//	}; // DropTarget
//
//private:
//	StageData* m_stage_data;
//
//private:
//	wxNotebook* m_notebook;
//
//	std::vector<SettingPageBase*> m_pages;
//
//}; // ToolbarPanel
//
//}
//
//#endif // _EASYPARTICLE2D_TOOLBAR_PANEL_H_