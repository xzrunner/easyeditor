
#ifndef EBUILDER_LAYER_H
#define EBUILDER_LAYER_H



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
		const std::vector<ee::TextSprite*>& getTexts() const {
			return m_texts;
		}

		void insert(Actor* actor);
		void insert(ee::TextSprite* text);
		void remove(ee::ISprite* sprite);
		void resetOrder(ee::ISprite* sprite, bool up);

		void clear();

	public:
		wxString name;

		bool isVisible;
		bool isEditable;

	private:
		std::vector<Actor*> m_actors;

		std::vector<ee::TextSprite*> m_texts;

	}; // Layer
}

#endif // EBUILDER_LAYER_H