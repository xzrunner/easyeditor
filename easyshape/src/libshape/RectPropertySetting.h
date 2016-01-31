#ifndef _LIBSHAPE_RECT_PROPERTY_SETTING_H_
#define _LIBSHAPE_RECT_PROPERTY_SETTING_H_



namespace libshape
{

class RectShape;

class RectPropertySetting : public ee::PropertySetting
{
public:
	RectPropertySetting(ee::EditPanelImpl* stage, RectShape* rect);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

private:
	ee::EditPanelImpl* m_stage;

	RectShape* m_rect;

}; // RectPropertySetting

}

#endif // _LIBSHAPE_RECT_PROPERTY_SETTING_H_