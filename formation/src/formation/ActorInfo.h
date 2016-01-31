#ifndef FORMATION_ACTOR_INFO_H
#define FORMATION_ACTOR_INFO_H

//

namespace formation
{
	struct ActorInfo
	{
		int id;

		int speed;

		ActorInfo() {
			id = -1;
			speed = 0;
		}

	}; // ActorInfo
}

#endif // FORMATION_ACTOR_INFO_H