#ifndef _LIBRESPACKER_PACK_PKM_H_
#define _LIBRESPACKER_PACK_PKM_H_

#include "PackImage.h"

#include <stdint.h>

namespace librespacker
{

class PackPKM : public PackImage
{
public:
	PackPKM();
	virtual ~PackPKM();

	virtual void Load(const std::string& filepath);
	virtual void Store(std::ofstream& fout) const;

private:
	void LoadCompressed(const std::string& filename, uint8_t*& buf, int& width, int& height);

private:
	int m_width, m_height;

	uint8_t *m_rgb_buf, *m_alpha_buf;

}; // PackPKM

}

#endif // _LIBRESPACKER_PACK_PKM_H_