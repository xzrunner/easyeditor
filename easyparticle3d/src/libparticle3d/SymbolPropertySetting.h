#ifndef _EASYPARTICLE3D_SYMBOL_PROPERTY_SETTING_H_
#define _EASYPARTICLE3D_SYMBOL_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace eparticle3d
{

class ParticleSystem;

class SymbolPropertySetting : public d2d::SymbolPropertySetting
{
public:
	SymbolPropertySetting(ParticleSystem* ps);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ParticleSystem* m_ps;

	std::string m_tag;

}; // SymbolPropertySetting

}

#endif // _EASYPARTICLE3D_SYMBOL_PROPERTY_SETTING_H_