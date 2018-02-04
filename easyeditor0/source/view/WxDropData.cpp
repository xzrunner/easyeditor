#include "ee0/WxDropData.h"
#include "ee0/WxDropTarget.h"

namespace ee0
{

WxDropData::WxDropData(WxDropTarget* target)
	: m_target(target)
	, m_data_object_last(NULL)
{
	Add(new wxTextDataObject);
	Add(new wxFileDataObject);
}

bool WxDropData::SetData(const wxDataFormat& format, size_t len, const void *buf)
{
	m_data_object_last = GetObject(format);

	wxCHECK_MSG(m_data_object_last, FALSE,
		wxT("unsupported format in wxURLDataObject"));

	return m_data_object_last->SetData(len, buf);
}

bool WxDropData::OnData(wxCoord x, wxCoord y)
{
	bool ret = false;

	wxDataFormat format = m_data_object_last->GetPreferredFormat();
	if (format == wxDF_UNICODETEXT)
	{
		wxTextDataObject* obj = static_cast<wxTextDataObject*>(m_data_object_last);
		m_target->OnDropText(x, y, obj->GetText());
		ret = true;
	}
	else if (format == wxDF_FILENAME)
	{
		wxFileDataObject* obj = static_cast<wxFileDataObject*>(m_data_object_last);
		m_target->OnDropFiles(x, y, obj->GetFilenames());
		ret = true;
	}

	return ret;
}


}