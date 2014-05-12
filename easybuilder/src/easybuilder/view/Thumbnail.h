
#ifndef EBUILDER_THUMBNAIL_H
#define EBUILDER_THUMBNAIL_H

#include <drag2d.h>

namespace ebuilder
{
	class Actor;

	class Thumbnail
	{
	public:
		static const float SCALE;

	public:
		static void draw(const std::vector<Actor*>& actors, wxMemoryDC& memDC);

	}; // Thumbnail
}

#endif // EBUILDER_THUMBNAIL_H