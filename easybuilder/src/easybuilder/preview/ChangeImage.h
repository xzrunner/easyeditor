
#ifndef EBUILDER_PREVIEW_CHANGE_IMAGE_H
#define EBUILDER_PREVIEW_CHANGE_IMAGE_H

#include "Behavior.h"

#include <drag2d.h>

namespace ebuilder
{	
	namespace preview
	{
		class ChangeImage : public Behavior
		{
		public:
			ChangeImage(d2d::ISprite* sprite, d2d::ISymbol* symbol);

			virtual void trigger();

		private:
			d2d::ISprite* m_sprite;

			d2d::ISymbol* m_symbol;

		}; // ChangeImage
	}
}

#endif // EBUILDER_PREVIEW_CHANGE_IMAGE_H
