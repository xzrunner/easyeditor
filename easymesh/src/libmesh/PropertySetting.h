#ifndef _EASYMESH_PROPERTY_SETTING_H_
#define _EASYMESH_PROPERTY_SETTING_H_

#include <ee/SpritePropertySetting.h>
#include <ee/OpenSymbolMonitor.h>

namespace emesh
{

class Sprite;

class PropertySetting : public ee::SpritePropertySetting
{
public:
	PropertySetting(ee::EditPanelImpl* edit_impl, Sprite* spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	class OpenSymbolMonitor : public ee::OpenSymbolMonitor
	{
	public:
		OpenSymbolMonitor(Sprite* spr) : m_spr(spr) {}
		virtual void OnOpenSymbol(ee::Symbol* sym);
//		void ChangeSpr(Sprite* spr) { m_spr = spr; }
	private:
		Sprite* m_spr;
	}; // OpenSymbolMonitor

}; // PropertySetting
	
}

#endif // _EASYMESH_PROPERTY_SETTING_H_