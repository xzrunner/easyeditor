
#ifndef EBUILDER_PREVIEW_CHANGE_IMAGE_H
#define EBUILDER_PREVIEW_CHANGE_IMAGE_H

#include "Behavior.h"



namespace ebuilder
{	
	namespace preview
	{
		class ChangeImage : public Behavior
		{
		public:
			ChangeImage(ee::ISprite* sprite, ee::ISymbol* symbol);

			virtual void trigger();

		private:
			ee::ISprite* m_sprite;

			ee::ISymbol* m_symbol;

		}; // ChangeImage
	}
}

#endif // EBUILDER_PREVIEW_CHANGE_IMAGE_H
