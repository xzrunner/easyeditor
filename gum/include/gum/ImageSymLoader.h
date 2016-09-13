#ifndef _GUM_IMAGE_SYM_LOADER_H_
#define _GUM_IMAGE_SYM_LOADER_H_

#include <CU_Uncopyable.h>

#include <string>

namespace gum
{

class ImageSymbol;

class ImageSymLoader : private cu::Uncopyable
{
public:
	ImageSymLoader(ImageSymbol* sym);
	~ImageSymLoader();

	void Load(const std::string& filepath);

private:
	ImageSymbol* m_sym;

}; // ImageSymLoader

}

#endif // _GUM_IMAGE_SYM_LOADER_H_