
#ifndef EBUILDER_BEHAVIOR_ITEM_H
#define EBUILDER_BEHAVIOR_ITEM_H

#include "BehaviorDefs.h"

namespace ebuilder
{
	class BehaviorItem : public ee::ListItem
	{
	public:
		BehaviorItem(const wxString& name, BehaviorType type)
			: m_type(type)
		{
			m_name = name;
		}

		virtual BehaviorItem* clone() const {
			return NULL;
		}

 		const BehaviorType& type() const {
 			return m_type;
 		}

	private:
		const BehaviorType m_type;

	}; // BehaviorItem
}

#endif // EBUILDER_BEHAVIOR_ITEM_H
