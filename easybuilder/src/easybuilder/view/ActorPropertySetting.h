
#ifndef EBUILDER_ACTOR_PROPERTY_SETTING_H
#define EBUILDER_ACTOR_PROPERTY_SETTING_H



namespace ebuilder
{
	class Actor;

	class ActorPropertySetting : public ee::IPropertySetting
	{
	public:
		ActorPropertySetting(Actor* actor, ee::LibraryList* list);

		virtual void updatePanel(ee::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(ee::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(ee::PropertySettingPanel* panel, bool bEnable);

	private:
		void updateActorName(const wxString& name);

	private:
		class UpdateNameVisitor : public ee::IVisitor
		{
		public:
			UpdateNameVisitor(Actor* actor) 
				: m_actor(actor) {}

			virtual void visit(ee::Object* object, bool& bFetchNext);

		private:
			Actor* m_actor;

		}; // UpdateNameVisitor

	private:
		Actor* m_actor;

		ee::LibraryList* m_list;

	}; // ActorPropertySetting
}

#endif // EBUILDER_ACTOR_PROPERTY_SETTING_H
