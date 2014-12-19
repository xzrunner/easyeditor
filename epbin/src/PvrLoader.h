#ifndef _EPBIN_PVR_LOADER_H_
#define _EPBIN_PVR_LOADER_H_

#include "TexLoader.h"

namespace epbin
{

class PvrLoader : public TexLoader
{
public:
	PvrLoader();

	virtual void Load(const std::string& filepath);

}; // PvrLoader

}

#endif // _EPBIN_PVR_LOADER_H_