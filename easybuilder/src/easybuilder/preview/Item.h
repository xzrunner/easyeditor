
#ifndef EBUILDER_ITEM_H
#define EBUILDER_ITEM_H



namespace ebuilder
{
	namespace preview
	{
		class Item : public ee::ImageSprite
		{
		public:
			enum Type
			{
				e_actor = 0,
				e_button
			};

		public:
			Item(Type type, const ee::ImageSprite& sprite)
				: ee::ImageSprite(sprite), m_type(type) {}

			Type type() const {
				return m_type;
			}

		private:
			Type m_type;

		}; // Item
	}
}

#endif // EBUILDER_ITEM_H
