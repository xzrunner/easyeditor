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
	PropertySetting(ee::EditPanelImpl* edit_impl, const std::shared_ptr<Sprite>& spr);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	class OpenSymbolMonitor : public ee::OpenSymbolMonitor
	{
	public:
		OpenSymbolMonitor(const std::shared_ptr<Sprite>& spr) : m_spr(spr) {}
		virtual void OnOpenSymbol(const ee::SymPtr& sym);
//		void ChangeSpr(Sprite* spr) { m_spr = spr; }
	private:
		std::shared_ptr<Sprite> m_spr;
	}; // OpenSymbolMonitor

}; // PropertySetting
	
}

#endif // _EASYMESH_PROPERTY_SETTING_H_