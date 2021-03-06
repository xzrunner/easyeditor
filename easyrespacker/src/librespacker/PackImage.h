#ifndef _EASYRESPACKER_PACK_IMAGE_H_
#define _EASYRESPACKER_PACK_IMAGE_H_

#include <fstream>

namespace erespacker
{

class PackImage
{
public:
	PackImage() : m_compress(true) {}
	virtual ~PackImage() {}

	virtual void Load(const std::string& filepath) = 0;
	virtual void Store(const std::string& filepath, float scale) const = 0;

protected:
	int m_type;

	bool m_compress;

}; // PackImage

}

#endif // _EASYRESPACKER_PACK_IMAGE_H_