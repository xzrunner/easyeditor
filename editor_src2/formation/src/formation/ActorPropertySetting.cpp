#include "ActorPropertySetting.h"
#include "ActorInfo.h"

using namespace formation;

ActorPropertySetting::ActorPropertySetting(d2d::EditPanel* editPanel, 
										   const std::vector<d2d::ISprite*>& sprites)
	: IPropertySetting(editPanel, wxT("Actor"))
	, m_sprites(sprites)
{
}

void ActorPropertySetting::updatePanel(d2d::PropertySettingPanel* panel)
{
	wxPropertyGrid* pg = panel->getPG();

	int id = getID();
	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("ID"))->SetValue(id);
	}
	else
	{
		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxIntProperty(wxT("ID"), wxPG_LABEL, id));
	}
}

void ActorPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (name == wxT("ID"))
	{
		int id = wxANY_AS(value, int);
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			static_cast<ActorInfo*>(m_sprites[i]->getUserData())->id = id;
	}
}

void ActorPropertySetting::updatePropertyGrid(d2d::PropertySettingPanel* panel)
{
	updatePanel(panel);
}

void ActorPropertySetting::enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable)
{
	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		int id = getID();

		pg->Clear();

		pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));

		pg->Append(new wxIntProperty(wxT("ID"), wxPG_LABEL, id));
	}

	pg->GetProperty(wxT("ID"))->Enable(bEnable);
}

int ActorPropertySetting::getID() const
{
	int id = -1;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ActorInfo* info = static_cast<ActorInfo*>(m_sprites[i]->getUserData());
		if (id == -1)
			id = info->id;
		else
		{
			if (id != info->id)
			{
				id = -1;
				break;
			}
		}
	}
	return id;
}