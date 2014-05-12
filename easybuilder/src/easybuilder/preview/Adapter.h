
#ifndef EBUILDER_PREVIEW_ADAPTER_H
#define EBUILDER_PREVIEW_ADAPTER_H

#include <drag2d.h>

namespace ebuilder
{	
	class Actor;
	class Scene;
	class Behavior;
	class LibraryPanel;

	namespace preview
	{
		class Panel;
		class Scene;
		class Actor;
		class Button;
		class Behavior;

		class Adapter
		{
		public:
			Adapter(Panel* panel);

			Scene* builder2Scene(const ebuilder::Scene& scene);

		private:
			Actor* builder2Actor(Scene* scene, const ebuilder::Actor& actor);
			
			Button* builder2Button(const ebuilder::Actor& actor);

			Behavior* builder2Behavior(d2d::ISprite* sprite,
				const ebuilder::Behavior& behavior);

			Scene* getScene(const ebuilder::Scene* scene);

		private:
			Panel* m_panel;

			std::map<const ebuilder::Scene*, ebuilder::preview::Scene*> m_mapToScene;

		}; // Adapter
	}
}

#endif // EBUILDER_PREVIEW_ADAPTER_H
