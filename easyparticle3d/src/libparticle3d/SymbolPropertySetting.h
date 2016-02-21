#ifndef _EASYPARTICLE3D_SYMBOL_PROPERTY_SETTING_H_
#define _EASYPARTICLE3D_SYMBOL_PROPERTY_SETTING_H_

#include <ee/SymbolPropertySetting.h>

namespace eparticle3d
{

class ParticleSystem;

class SymbolPropertySetting : public ee::SymbolPropertySetting
{
public:
	SymbolPropertySetting(ParticleSystem* ps);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ParticleSystem* m_ps;

	std::string m_tag;

}; // SymbolPropertySetting

}

#endif // _EASYPARTICLE3D_SYMBOL_PROPERTY_SETTING_H_