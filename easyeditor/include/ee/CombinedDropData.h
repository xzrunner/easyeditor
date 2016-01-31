#ifndef _EASYEDITOR_COMBINED_DROP_DATA_H_
#define _EASYEDITOR_COMBINED_DROP_DATA_H_

#include <wx/dataobj.h>

namespace ee
{

class CombinedDropTarget;

class CombinedDropData : public wxDataObjectComposite
{
public:
	CombinedDropData(CombinedDropTarget* drop);

	virtual bool SetData(const wxDataFormat& format, size_t len, const void *buf);

	bool OnData(wxCoord x, wxCoord y);

private:
	CombinedDropTarget* m_drop;

	wxDataObjectSimple *m_data_object_last;

	wxDECLARE_NO_COPY_CLASS(CombinedDropData);

}; // CombinedDropData 

}

#endif // _EASYEDITOR_COMBINED_DROP_DATA_H_