
#ifndef EBUILDER_LAYER_H
#define EBUILDER_LAYER_H

#include <drag2d.h>

namespace ebuilder
{
	class Actor;

	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		const std::vector<Actor*>& getActors() const {
			return m_actors;
		}
		const std::vector<d2d::TextSprite*>& getTexts() const {
			return m_texts;
		}

		void insert(Actor* actor);
		void insert(d2d::TextSprite* text);
		void remove(d2d::ISprite* sprite);
		void resetOrder(d2d::ISprite* sprite, bool up);

		void clear();

	public:
		wxString name;

		bool isVisible;
		bool isEditable;

	private:
		std::vector<Actor*> m_actors;

		std::vector<d2d::TextSprite*> m_texts;

	}; // Layer
}

#endif // EBUILDER_LAYER_H