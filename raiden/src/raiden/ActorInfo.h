#ifndef RAIDEN_ACTOR_INFO_H
#define RAIDEN_ACTOR_INFO_H



namespace raiden
{
	struct ActorInfo
	{
		int id;

		int speed;

		const ee::ISymbol* symbol;
		ee::Vector offset;

		ActorInfo();

		void resetOffset();

	}; // ActorInfo
}

#endif // RAIDEN_ACTOR_INFO_H