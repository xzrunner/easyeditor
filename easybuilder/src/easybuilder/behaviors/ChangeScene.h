
#ifndef EBUILDER_CHANGE_SCENE_H
#define EBUILDER_CHANGE_SCENE_H

#include "Behavior.h"

namespace ebuilder
{
	class ScenesPage;

	class ChangeScene : public Behavior
	{
	public:
		virtual ~ChangeScene();

		virtual void update(const ee::LibraryList& list);

		void registerListener(ScenesPage* scenePage);

	private:
		ChangeScene(Actor* actor, ee::LibraryList* sceneList);

	public:
		ee::ISymbol* selected;

		ScenesPage* m_scenePage;

		friend class BehaviorFactory;

	}; // ChangeScene
}

#endif // EBUILDER_CHANGE_SCENE_H
