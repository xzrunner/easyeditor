#ifndef _LIBRESPACKER_PACK_IMAGE_H_
#define _LIBRESPACKER_PACK_IMAGE_H_

#include <fstream>

namespace librespacker
{

class PackImage
{
public:
	PackImage() : m_compress(true) {}
	virtual ~PackImage() {}

	virtual void Load(const std::string& filepath) = 0;
	virtual void Store(std::ofstream& fout) const = 0;

protected:
	int m_type;

	bool m_compress;

}; // PackImage

}

#endif // _LIBRESPACKER_PACK_IMAGE_H_