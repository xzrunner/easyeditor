
#ifndef EBUILDER_PREVIEW_SCENE_H
#define EBUILDER_PREVIEW_SCENE_H



namespace ebuilder
{	
	namespace preview
	{
		class Button;
		class Item;
		class Behavior;

		class Scene
		{
		public:
			~Scene();

			Button* queryButtonByPos(const ee::Vector& pos) const;

			void triggerAllBehaviors();

		public:
			ee::Colorf m_color;

			std::vector<Item*> m_items;

			std::vector<Behavior*> m_behaviors;

		}; // Scene
	}
}

#endif // EBUILDER_PREVIEW_SCENE_H
