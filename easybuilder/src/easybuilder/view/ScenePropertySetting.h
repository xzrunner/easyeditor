
#ifndef EBUILDER_SCENE_PROPERTY_SETTING_H
#define EBUILDER_SCENE_PROPERTY_SETTING_H

#include <drag2d.h>

namespace ebuilder
{
	class SceneItem;
	class ScenesPage;

	class ScenePropertySetting : public d2d::IPropertySetting
	{
	public:
		ScenePropertySetting(SceneItem* scene, ScenesPage* page);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		void updateSceneName(const wxString& name);

	private:
		class UpdateNameVisitor : public d2d::IVisitor
		{
		public:
			UpdateNameVisitor(SceneItem* scene) 
				: m_scene(scene) {}

			virtual void visit(d2d::Object* object, bool& bFetchNext);

		private:
			SceneItem* m_scene;

		}; // UpdateNameVisitor

	private:
		SceneItem* m_scene;

		ScenesPage* m_page;

	}; // ScenePropertySetting
}

#endif // EBUILDER_SCENE_PROPERTY_SETTING_H
