#ifndef _EASYEDITOR_VIEWLIST_PANEL_H_
#define _EASYEDITOR_VIEWLIST_PANEL_H_

#include <wx/wx.h>

namespace ee
{

class Sprite;
class ViewlistList;
class ViewlistListImpl;

class ViewlistPanel : public wxPanel
{
public:
	ViewlistPanel(wxWindow* parent);

	void Clear();
	void Insert(Sprite* spr, int idx = -1);

	void SetListImpl(ViewlistListImpl* impl);

	void EnableObserve(bool enable);

private:
	void InitLayout();

private:
	ViewlistList* m_list;

}; // ViewlistPanel

}

#endif // _EASYEDITOR_VIEWLIST_PANEL_H_