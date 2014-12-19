#ifndef _EPBIN_PNG_LOADER_H_
#define _EPBIN_PNG_LOADER_H_

#include "TexLoader.h"

namespace epbin
{

class PngLoader : public TexLoader
{
public:
	PngLoader(bool png8 = true);

	virtual void Load(const std::string& filepath);

private:
	void GenPng4();

}; // PngLoader

}

#endif // _EPBIN_PNG_LOADER_H_