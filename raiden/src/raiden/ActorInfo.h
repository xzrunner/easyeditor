#ifndef RAIDEN_ACTOR_INFO_H
#define RAIDEN_ACTOR_INFO_H

#include <drag2d.h>

namespace raiden
{
	struct ActorInfo
	{
		int id;

		int speed;

		const d2d::ISymbol* symbol;
		d2d::Vector offset;

		ActorInfo();

		void resetOffset();

	}; // ActorInfo
}

#endif // RAIDEN_ACTOR_INFO_H