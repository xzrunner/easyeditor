#include "ee0/WxDropTarget.h"
#include "ee0/WxDropData.h"

namespace ee0
{

WxDropTarget::WxDropTarget()
{
	SetDataObject(new WxDropData(this));
}

wxDragResult WxDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	if (!GetData()) {
		return wxDragNone;
	}

	WxDropData* dobj = (WxDropData*)m_dataObject;

	bool b = false;
	//try {
		b = dobj->OnData(x, y);
	//} catch (Exception& e) {
	//	ExceptionDlg dlg(m_parent, e);
	//	dlg.ShowModal();
	//}

	return b ? def : wxDragNone;
}


}