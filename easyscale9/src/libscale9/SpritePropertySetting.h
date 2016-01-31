#ifndef _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_
#define _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_



namespace escale9
{

class Sprite;

class SpritePropertySetting : public ee::SpritePropertySetting
{
public:
	SpritePropertySetting(ee::EditPanelImpl* stage, Sprite* sprite);

	virtual void OnPropertyGridChange(const wxString& name, const wxAny& value);

protected:
	virtual void UpdateProperties(wxPropertyGrid* pg);
	virtual void InitProperties(wxPropertyGrid* pg);

}; // SpritePropertySetting

}

#endif // _EASYSCALE9_SPRITE_PROPERTY_SETTING_H_