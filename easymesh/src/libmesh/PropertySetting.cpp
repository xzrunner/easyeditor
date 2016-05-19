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
}

void PropertySetting::InitProperties(wxPropertyGrid* pg)
{
	ee::SpritePropertySetting::InitProperties(pg);

	pg->Append(new wxPropertyCategory("MESH", wxPG_LABEL));

	Sprite* spr = static_cast<Sprite*>(GetSprite());
// 	std::string base_path = spr->GetBaseSym()->GetFilepath();
// 	pg->Append(new wxStringProperty(wxT("BasePath"), wxPG_LABEL, base_path));
// 	pg->SetPropertyReadOnly("BasePath");

	ee::OpenSymbolProperty* prop = new ee::OpenSymbolProperty("MeshSymbol", "zz-name", spr->GetBaseSym()->GetFilepath());
	prop->SetListener(new OpenSymbolMonitor(spr));
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