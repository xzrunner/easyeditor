
#ifndef EBUILDER_CHANGE_IMAGE_H
#define EBUILDER_CHANGE_IMAGE_H

#include "Behavior.h"

namespace ebuilder
{
	class MediaPage;

	class ChangeImage : public Behavior
	{
	public:
		virtual ~ChangeImage();

		virtual void update(const d2d::LibraryList& list);

		void registerListener(MediaPage* mediaPage);

	private:
		ChangeImage(Actor* actor, d2d::LibraryList* mediaList);

	public:
		d2d::ISymbol* selected;

		MediaPage* m_mediaPage;

		friend class BehaviorFactory;

	}; // ChangeImage
}

#endif // EBUILDER_CHANGE_IMAGE_H
