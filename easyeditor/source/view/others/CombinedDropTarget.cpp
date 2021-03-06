#include "CombinedDropTarget.h"
#include "CombinedDropData.h"
#include "Exception.h"
#include "ExceptionDlg.h"

namespace ee
{

CombinedDropTarget::CombinedDropTarget(wxWindow* parent)
	: m_parent(parent)
{
	SetDataObject(new CombinedDropData(this));
}

wxDragResult CombinedDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	if (!GetData()) {
		return wxDragNone;
	}

	CombinedDropData* dobj = (CombinedDropData*)m_dataObject;

	bool b = false;
	try {
		b = dobj->OnData(x, y);
	} catch (Exception& e) {
		ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}

	return b ? def : wxDragNone;
}

}