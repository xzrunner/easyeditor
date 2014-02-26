#pragma once

#include "IPropertySetting.h"

#include <vector>

namespace d2d
{
	class ISprite;
	class Vector;

	class MultiSpritesPropertySetting : public IPropertySetting
	{
	public:
		MultiSpritesPropertySetting(EditPanel* editPanel, 
			const std::vector<ISprite*>& sprites);

		virtual void updatePanel(PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(PropertySettingPanel* panel);
		virtual void enablePropertyGrid(PropertySettingPanel* panel, bool bEnable);

	protected:
		virtual void updateProperties(wxPropertyGrid* pg);
		virtual void initProperties(wxPropertyGrid* pg);

	private:
		void align(const wxAny& value);
		void center(const wxAny& value);

		Vector getSamePosition() const;

	protected:
		std::vector<ISprite*> m_sprites;

	}; // MultiSpritesPropertySetting
}

