#ifndef _EASYMAP_STAGE_PANEL_H_
#define _EASYMAP_STAGE_PANEL_H_

#include <drag2d.h>

namespace emap
{

class StagePanel : public d2d::EditPanel, public d2d::SpritesPanelImpl, 
	public d2d::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		d2d::LibraryPanel* library, d2d::PropertySettingPanel* property,
		d2d::ViewPanelMgr* view_panel_mgr);
	virtual ~StagePanel();

	//
	// d2d::EditPanel interface
	//
	virtual void Clear();

protected:
	virtual void OnMouseHook(wxMouseEvent& event);

private:
	void ChangeEditOP();

private:
	d2d::AbstractEditOP* m_paste_op;
	d2d::AbstractEditOP* m_arrange_op;

}; // StagePanel

}

#endif // _EASYMAP_STAGE_PANEL_H_
