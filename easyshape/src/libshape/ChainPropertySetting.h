#ifndef _EASYSHAPE_CHAIN_PROPERTY_SETTING_H_
#define _EASYSHAPE_CHAIN_PROPERTY_SETTING_H_

#include <ee/PropertySetting.h>

namespace ee { class EditPanelImpl; }

namespace eshape
{

class ChainShape;

class ChainPropertySetting : public ee::PropertySetting
{
public:
	ChainPropertySetting(ee::EditPanelImpl* stage, ChainShape* chain);

	virtual void OnPropertyGridChange(const std::string& name, const wxAny& value) override;

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg) override;
	virtual void InitProperties(wxPropertyGrid* pg) override;

private:
	ee::EditPanelImpl* m_stage;

	ChainShape* m_chain;

}; // ChainPropertySetting

}

#endif // _EASYSHAPE_CHAIN_PROPERTY_SETTING_H_