#ifndef _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_
#define _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_



namespace libshape
{

class PolygonShape;

class PolygonPropertySetting : public ee::PropertySetting
{
public:
	PolygonPropertySetting(ee::EditPanelImpl* stage, PolygonShape* poly);
	virtual ~PolygonPropertySetting();

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	PolygonShape* m_poly;

}; // PolygonPropertySetting

}

#endif // _LIBSHAPE_POLYGON_PROPERTY_SETTING_H_