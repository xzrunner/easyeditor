#ifndef _RT_JITTERED_H_
#define _RT_JITTERED_H_

#include "Sampler.h"

namespace rt
{

class Jittered : public Sampler
{
public:
	Jittered(int num);

	virtual void GenerateSamples();

}; // Jittered

}

#endif // _RT_JITTERED_H_