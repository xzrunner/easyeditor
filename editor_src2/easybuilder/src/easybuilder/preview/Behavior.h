
#ifndef EBUILDER_PREVIEW_BEHAVIOR_H
#define EBUILDER_PREVIEW_BEHAVIOR_H

namespace ebuilder
{	
	namespace preview
	{	
		class Behavior
		{
		public:
			virtual ~Behavior() {}

			virtual void trigger() = 0;

		}; // Behavior
	}
}

#endif // EBUILDER_PREVIEW_BEHAVIOR_H
