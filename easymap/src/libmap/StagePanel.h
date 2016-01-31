#ifndef _EASYMAP_STAGE_PANEL_H_
#define _EASYMAP_STAGE_PANEL_H_



namespace emap
{

class StagePanel : public ee::EditPanel, public ee::SpritesPanelImpl, 
	public ee::ShapesPanelImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::LibraryPanel* library, ee::PropertySettingPanel* property);
	virtual ~StagePanel();

protected:
	virtual void OnMouseHook(wxMouseEvent& event);

private:
	void ChangeEditOP();

private:
	ee::EditOP* m_paste_op;
	ee::EditOP* m_arrange_op;

}; // StagePanel

}

#endif // _EASYMAP_STAGE_PANEL_H_
