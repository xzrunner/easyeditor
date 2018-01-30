#ifndef _S2LOADER_IMAGE_SYM_LOADER_H_
#define _S2LOADER_IMAGE_SYM_LOADER_H_

#include <cu/uncopyable.h>
#include <cu/cu_stl.h>

namespace bimp { class FilePath; }
namespace gum { class ImageSymbol; }

namespace s2loader
{

class ImageSymLoader : private cu::Uncopyable
{
public:
	ImageSymLoader(gum::ImageSymbol& sym);

	void Load(const bimp::FilePath& res_path, float scale, bool async);

private:
	gum::ImageSymbol& m_sym;

}; // ImageSymLoader

}

#endif // _S2LOADER_IMAGE_SYM_LOADER_H_
