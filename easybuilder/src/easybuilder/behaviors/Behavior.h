
#ifndef EBUILDER_BEHAVIOR_H
#define EBUILDER_BEHAVIOR_H

#include "BehaviorDefs.h"

#include <drag2d.h>

namespace ebuilder
{
	class Actor;

	class Behavior : public d2d::ICloneable
	{
	public:
		Behavior(BehaviorType type, Actor* actor, int order = 0) 
			: m_type(type), m_actor(actor), m_order(order) {}
		virtual ~Behavior() {}

		//
		// ICloneable interface
		//
		virtual Behavior* clone() const { return NULL; }

		virtual void update(const d2d::LibraryList& list) {}

		const BehaviorType& type() const {
			return m_type;
		} 

		Actor* getActor() const {
			return m_actor;
		}

		wxString getName() const {
			switch (m_type)
			{
			case e_ChangeImage:
				return CHANGE_IMAGE_NAME;
			case e_ChangeScene:
				return CHANGE_SCENE_NAME;
			case e_ScaleSize:
				return SCALE_SIZE_NAME;

			case e_MouseButton:
				return MOUSE_BUTTON_NAME;

			case e_Move:
				return MOVE_NAME;
			case e_Rotate:
				return ROTATE_NAME;

			default:
				return wxEmptyString;
			}
		}

	public:
		class Cmp
		{
		public:
			bool operator() (const Behavior* b0, const Behavior* b1) {
				return b0->m_order < b1->m_order;
			}
		}; // Cmp

	protected:
		static bool isSymbolInList(const d2d::ISymbol* symbol,
			const d2d::LibraryList& list) {
				int index = 0;
				d2d::ISymbol* s = list.getSymbol(index);
				while (s) {
					if (symbol == s)
						return true;
					s = list.getSymbol(++index);
				}
				return s != NULL;
		}

	private:
		const BehaviorType m_type;

		Actor* m_actor;

		int m_order;

	}; // Behavior
}

#endif // EBUILDER_BEHAVIOR_H
