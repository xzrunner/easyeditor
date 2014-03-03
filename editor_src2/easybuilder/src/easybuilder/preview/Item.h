
#ifndef EBUILDER_ITEM_H
#define EBUILDER_ITEM_H

#include <drag2d.h>

namespace ebuilder
{
	namespace preview
	{
		class Item : public d2d::ImageSprite
		{
		public:
			enum Type
			{
				e_actor = 0,
				e_button
			};

		public:
			Item(Type type, const d2d::ImageSprite& sprite)
				: d2d::ImageSprite(sprite), m_type(type) {}

			Type type() const {
				return m_type;
			}

		private:
			Type m_type;

		}; // Item
	}
}

#endif // EBUILDER_ITEM_H
