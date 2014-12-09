#ifndef _RT_REGULAR_H_
#define _RT_REGULAR_H_

#include "Sampler.h"

namespace rt
{

class Regular : public Sampler
{
public:
	Regular(int num);

	virtual void GenerateSamples();

}; // Regular

}

#endif // _RT_REGULAR_H_