#include "PropertySetting.h"
#include "Sprite.h"

#include <ee/OpenSymbolProperty.h>

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
}

void PropertySetting::UpdateProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::UpdateProperties(pg);

// 	Sprite* spr = static_cast<Sprite*>(GetSprite());
// 	m_lsn->ChangeSpr(spr);

//	pg->GetProperty("MeshSymbol")->SetValue(spr->GetSymbol().GetFilepath());
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
}

/************************************************************************/
/* class PropertySetting::OpenSymbolMonitor                             */
/************************************************************************/

void PropertySetting::OpenSymbolMonitor::OnOpenSymbol(ee::Symbol* sym)
{
	m_spr->SetBaseSym(sym);
}

}