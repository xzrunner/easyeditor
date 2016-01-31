
#ifndef EBUILDER_SCENE_PROPERTY_SETTING_H
#define EBUILDER_SCENE_PROPERTY_SETTING_H



namespace ebuilder
{
	class SceneItem;
	class ScenesPage;

	class ScenePropertySetting : public ee::IPropertySetting
	{
	public:
		ScenePropertySetting(SceneItem* scene, ScenesPage* page);

		virtual void updatePanel(ee::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(ee::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable);

	private:
		void updateSceneName(const wxString& name);

	private:
		class UpdateNameVisitor : public ee::IVisitor
		{
		public:
			UpdateNameVisitor(SceneItem* scene) 
				: m_scene(scene) {}

			virtual void visit(ee::Object* object, bool& bFetchNext);

		private:
			SceneItem* m_scene;

		}; // UpdateNameVisitor

	private:
		SceneItem* m_scene;

		ScenesPage* m_page;

	}; // ScenePropertySetting
}

#endif // EBUILDER_SCENE_PROPERTY_SETTING_H
