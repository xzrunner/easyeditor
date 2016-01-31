#ifndef _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_
#define _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_



namespace libshape
{

class BezierShape;

class BezierPropertySetting : public ee::PropertySetting
{
public:
	BezierPropertySetting(ee::EditPanelImpl* stage, BezierShape* bezier);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	BezierShape* m_bezier;

}; // BezierPropertySetting

}

#endif // _LIBSHAPE_BEZIER_PROPERTY_SETTING_H_