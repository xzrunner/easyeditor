#pragma once

#include "IPropertySetting.h"

namespace d2d
{
	class ISprite;

	class SpritePropertySetting : public IPropertySetting
	{
	public:
		SpritePropertySetting(EditPanel* editPanel, ISprite* sprite);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	protected:
		virtual void updateProperties(wxPropertyGrid* pg);
		virtual void initProperties(wxPropertyGrid* pg);

	private:
		void translate(float x, float y);
		void rotate(float angle);
		void scale(float sx, float sy);
		void shear(float kx, float ky);
		void offset(float ox, float oy);
		void mirror(bool mx, bool my);

		void splitString(const wxAny& value, double* x, double* y);

	protected:
		ISprite* m_sprite;

	}; // SpritePropertySetting
}

