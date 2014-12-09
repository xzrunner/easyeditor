#ifndef _RT_MULTI_JITTERED_H_
#define _RT_MULTI_JITTERED_H_

#include "Sampler.h"

namespace rt
{

class MultiJittered : public Sampler
{
public:
	MultiJittered(int num);

	virtual void GenerateSamples();

}; // MultiJittered

}

#endif // _RT_MULTI_JITTERED_H_