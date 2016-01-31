#ifndef _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_
#define _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_



namespace libshape
{

class ChainShape;

class ChainPropertySetting : public ee::PropertySetting
{
public:
	ChainPropertySetting(ee::EditPanelImpl* stage, ChainShape* chain);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	ChainShape* m_chain;

}; // ChainPropertySetting

}

#endif // _LIBSHAPE_CHAIN_PROPERTY_SETTING_H_