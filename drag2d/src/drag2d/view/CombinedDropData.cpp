#include "CombinedDropData.h"
#include "CombinedDropTarget.h"

namespace d2d
{

CombinedDropData::CombinedDropData(CombinedDropTarget* drop)
	: m_drop(drop)
	, m_data_object_last(NULL)
{
	Add(new wxTextDataObject);
	Add(new wxFileDataObject);
}

bool CombinedDropData::SetData(const wxDataFormat& format, size_t len, const void *buf)
{
	m_data_object_last = GetObject(format);

	wxCHECK_MSG(m_data_object_last, FALSE,
		wxT("unsupported format in wxURLDataObject"));

	return m_data_object_last->SetData(len, buf);
}

bool CombinedDropData::OnData(wxCoord x, wxCoord y)
{
	bool ret = false;

	wxDataFormat format = m_data_object_last->GetPreferredFormat();
	if (format == wxDF_UNICODETEXT)
	{
		wxTextDataObject* obj = static_cast<wxTextDataObject*>(m_data_object_last);
		m_drop->OnDropText(x, y, obj->GetText());
		ret = true;
	}
	else if (format == wxDF_FILENAME)
	{
		wxFileDataObject* obj = static_cast<wxFileDataObject*>(m_data_object_last);
		m_drop->OnDropFiles(x, y, obj->GetFilenames());
		ret = true;
	}

	return ret;
}

}