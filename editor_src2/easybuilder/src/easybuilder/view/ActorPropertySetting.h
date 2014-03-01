
#ifndef EBUILDER_ACTOR_PROPERTY_SETTING_H
#define EBUILDER_ACTOR_PROPERTY_SETTING_H

#include <drag2d.h>

namespace ebuilder
{
	class Actor;

	class ActorPropertySetting : public d2d::IPropertySetting
	{
	public:
		ActorPropertySetting(Actor* actor, d2d::LibraryList* list);

		virtual void updatePanel(d2d::PropertySettingPanel* panel);

		virtual void onPropertyGridChange(const wxString& name, const wxAny& value);
		virtual void updatePropertyGrid(d2d::PropertySettingPanel* panel);
		virtual void enablePropertyGrid(d2d::PropertySettingPanel* panel, bool bEnable);

	private:
		void updateActorName(const wxString& name);

	private:
		class UpdateNameVisitor : public d2d::IVisitor
		{
		public:
			UpdateNameVisitor(Actor* actor) 
				: m_actor(actor) {}

			virtual void visit(d2d::Object* object, bool& bFetchNext);

		private:
			Actor* m_actor;

		}; // UpdateNameVisitor

	private:
		Actor* m_actor;

		d2d::LibraryList* m_list;

	}; // ActorPropertySetting
}

#endif // EBUILDER_ACTOR_PROPERTY_SETTING_H
