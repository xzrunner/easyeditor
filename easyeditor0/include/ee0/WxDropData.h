#pragma once

#include <wx/dataobj.h>

namespace ee0
{

class WxDropTarget;

class WxDropData : public wxDataObjectComposite
{
public:
	WxDropData(WxDropTarget* target);

	virtual bool SetData(const wxDataFormat& format, size_t len, const void *buf) override;

	bool OnData(wxCoord x, wxCoord y);

private:
	WxDropTarget* m_target;

	wxDataObjectSimple* m_data_object_last;

	wxDECLARE_NO_COPY_CLASS(WxDropData);

}; // WxDropData 

}