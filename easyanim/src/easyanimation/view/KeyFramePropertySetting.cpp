#include "KeyFramePropertySetting.h"

#include "dataset/KeyFrame.h"

namespace eanim
{

KeyFramePropertySetting::KeyFramePropertySetting(KeyFrame* frame)
	: ee::PropertySetting("KeyFrame")
	, m_frame(frame)
{
}

void KeyFramePropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("ID"))
		m_frame->SetID(wxANY_AS(value, float));
}

void KeyFramePropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	pg->GetProperty(wxT("ID"))->SetValue(m_frame->GetID());
}

void KeyFramePropertySetting::InitProperties(wxPropertyGrid* pg)
{
	pg->Clear();

	pg->Append(new wxFloatProperty(wxT("ID"), wxPG_LABEL, m_frame->GetID()));
}

} // eanim