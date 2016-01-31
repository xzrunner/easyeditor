#include "ActorPropertySetting.h"
#include "ActorInfo.h"

using namespace raiden;

ActorPropertySetting::ActorPropertySetting(ee::EditPanel* editPanel, 
										   const std::vector<ee::ISprite*>& sprites)
	: ee::MultiSpritesPropertySetting(editPanel, sprites)
{
}

void ActorPropertySetting::updatePanel(ee::PropertySettingPanel* panel)
{
	if (m_sprites.size() > 1)
		ee::MultiSpritesPropertySetting::updatePanel(panel);

	wxPropertyGrid* pg = panel->getPG();

	int id = getID();
	int speed = getSpeed();

	if (getPGType(pg) == m_type)
	{
		pg->GetProperty(wxT("ID"))->SetValue(id);

		pg->GetProperty(wxT("Speed"))->SetValue(speed);
	}
	else
	{
		if (m_sprites.size() <= 1)
		{
			pg->Clear();
			pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));
		}

		pg->Append(new wxIntProperty(wxT("ID"), wxPG_LABEL, id));

		pg->Append(new wxIntProperty(wxT("Speed"), wxPG_LABEL, speed));
	}
}

void ActorPropertySetting::onPropertyGridChange(const wxString& name, const wxAny& value)
{
	if (value.IsNull())
		return;

	if (m_sprites.size() > 1)
		ee::MultiSpritesPropertySetting::onPropertyGridChange(name, value);

	if (name == wxT("ID"))
	{
		int id = wxANY_AS(value, int);
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			static_cast<ActorInfo*>(m_sprites[i]->getUserData())->id = id;
	}
	else if (name == wxT("Speed"))
	{
		int speed = wxANY_AS(value, int);
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
			static_cast<ActorInfo*>(m_sprites[i]->getUserData())->speed = speed;
	}
}

void ActorPropertySetting::updatePropertyGrid(ee::PropertySettingPanel* panel)
{
	if (m_sprites.size() > 1)
		ee::MultiSpritesPropertySetting::updatePanel(panel);
	updatePanel(panel);
}

void ActorPropertySetting::enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable)
{
	if (m_sprites.size() > 1)
		ee::MultiSpritesPropertySetting::enablePropertyGrid(panel, bEnable);

	wxPropertyGrid* pg = panel->getPG();

	if (getPGType(pg) != m_type)
	{
		int id = getID();
		int speed = getSpeed();

		if (m_sprites.size() <= 1)
		{
			pg->Clear();
			pg->Append(new wxStringProperty(wxT("Type"), wxPG_LABEL, m_type));
		}

		pg->Append(new wxIntProperty(wxT("ID"), wxPG_LABEL, id));

		pg->Append(new wxIntProperty(wxT("Speed"), wxPG_LABEL, speed));
	}

	pg->GetProperty(wxT("ID"))->Enable(bEnable);
	pg->GetProperty(wxT("Speed"))->Enable(bEnable);
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

int ActorPropertySetting::getSpeed() const
{
	int speed = -1;
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ActorInfo* info = static_cast<ActorInfo*>(m_sprites[i]->getUserData());
		if (speed == -1)
			speed = info->speed;
		else
		{
			if (speed != info->speed)
			{
				speed = -1;
				break;
			}
		}
	}
	return speed;
}