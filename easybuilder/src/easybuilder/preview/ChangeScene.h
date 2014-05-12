
#ifndef EBUILDER_PREVIEW_CHANGE_SCENE_H
#define EBUILDER_PREVIEW_CHANGE_SCENE_H

#include "Behavior.h"

namespace ebuilder
{	
	namespace preview
	{
		class Panel;
		class Scene;

		class ChangeScene : public Behavior
		{
		public:
			ChangeScene(Panel* panel, Scene* scene);
			virtual ~ChangeScene();

			virtual void trigger();

		private:
			Panel* m_panel;

			Scene* m_scene;

		}; // ChangeScene
	}
}

#endif // EBUILDER_PREVIEW_CHANGE_SCENE_H
