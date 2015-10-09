#ifndef _LIBRESPACKER_PACK_PNG_H_
#define _LIBRESPACKER_PACK_PNG_H_

#include "PackImage.h"

#include <stdint.h>

namespace librespacker
{

class PackPNG : public PackImage
{
public:
	PackPNG(bool png8);
	virtual ~PackPNG();

	virtual void Load(const std::string& filepath);
	virtual void Store(std::ofstream& fout, float scale) const;

private:
	void Clear();

	void GenPng4();

private:
	int m_width, m_height;

	uint8_t* m_buffer;
	int m_buf_sz;

}; // PackPNG

}

#endif // _LIBRESPACKER_PACK_PNG_H_