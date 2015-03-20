#include "CombinedDropTarget.h"
#include "CombinedDropData.h"

namespace d2d
{

CombinedDropTarget::CombinedDropTarget()
{
	SetDataObject(new CombinedDropData(this));
}

wxDragResult CombinedDropTarget::OnData(wxCoord x, wxCoord y, wxDragResult def)
{
	if (!GetData()) {
		return wxDragNone;
	}

	CombinedDropData* dobj = (CombinedDropData*)m_dataObject;
	return dobj->OnData(x, y) ? def : wxDragNone;
}

}