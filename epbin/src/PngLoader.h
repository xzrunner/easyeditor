#ifndef _EPBIN_PNG_LOADER_H_
#define _EPBIN_PNG_LOADER_H_

#include "TexLoader.h"
#include "typedef.h"

namespace epbin
{

class PNGLoader : public TexLoader
{
public:
	PNGLoader(bool png8);
	virtual ~PNGLoader();

	virtual void Load(const std::string& filepath);
	virtual void Store(std::ofstream& fout) const;

private:
	void GenPng4();

private:
	int m_width, m_height;

	uint8_t* m_buffer;
	int m_buf_sz;

}; // PngLoader

}

#endif // _EPBIN_PNG_LOADER_H_