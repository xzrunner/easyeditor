#include "PropertySetting.h"
#include "Sprite.h"

#include <ee/OpenSymbolProperty.h>
#include <ee/panel_msg.h>

namespace emesh
{

PropertySetting::PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* sprite)
	: ee::SpritePropertySetting(edit_impl, sprite)
{
	m_type = "Mesh";
}

void PropertySetting::OnPropertyGridChange(const std::string& name, const wxAny& value)
{
	ee::SpritePropertySetting::OnPropertyGridChange(name, value);

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	if (name == "OnlyDrawBound")
	{
		spr->SetOnlyDrawBound(wxANY_AS(value, bool));
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

 	Sprite* spr = static_cast<Sprite*>(GetSprite());
// 	m_lsn->ChangeSpr(spr);

//	pg->GetProperty("MeshSymbol")->SetValue(spr->GetSymbol().GetFilepath());

	pg->GetProperty(wxT("OnlyDrawBound"))->SetValue(spr->OnlyDrawBound());
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("MESH", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
	ee::OpenSymbolProperty* prop = new ee::OpenSymbolProperty("MeshSymbol", "zz-name", spr->GetBaseSym()->GetFilepath());
	OpenSymbolMonitor* lsn = new OpenSymbolMonitor(spr);
	prop->SetListener(lsn);
	lsn->Release();
	pg->Append(prop);

	pg->Append(new wxBoolProperty("OnlyDrawBound", wxPG_LABEL, spr->OnlyDrawBound()));
	pg->SetPropertyAttribute("OnlyDrawBound", wxPG_BOOL_USE_CHECKBOX, true, wxPG_RECURSE);
}

/************************************************************************/
/* class PropertySetting::OpenSymbolMonitor                             */
/************************************************************************/

void PropertySetting::OpenSymbolMonitor::OnOpenSymbol(ee::Symbol* sym)
{
	m_spr->SetBaseSym(sym);
}

}