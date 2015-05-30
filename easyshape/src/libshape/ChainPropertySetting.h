#ifndef _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_
#define _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_

#include <drag2d.h>

namespace libshape
{

class ChainShape;

class ChainPropertySetting : public d2d::IPropertySetting
{
public:
	ChainPropertySetting(d2d::EditPanel* editPanel, ChainShape* chain);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ChainShape* m_chain;

}; // ChainPropertySetting

}

#endif // _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_