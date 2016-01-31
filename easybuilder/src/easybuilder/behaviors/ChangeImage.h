
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

		virtual void update(const ee::LibraryList& list);

		void registerListener(MediaPage* mediaPage);

	private:
		ChangeImage(Actor* actor, ee::LibraryList* mediaList);

	public:
		ee::ISymbol* selected;

		MediaPage* m_mediaPage;

		friend class BehaviorFactory;

	}; // ChangeImage
}

#endif // EBUILDER_CHANGE_IMAGE_H
