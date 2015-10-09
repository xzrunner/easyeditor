#ifndef _LIBRESPACKER_PACK_PVR_H_
#define _LIBRESPACKER_PACK_PVR_H_

#include "PackImage.h"

#include <stdint.h>

namespace librespacker
{

class PackPVR : public PackImage
{
public:
	PackPVR();
	virtual ~PackPVR();

	virtual void Load(const std::string& filepath);
	virtual void Store(std::ofstream& fout) const;

private:
	void ClearImageData();

private:
	struct Slice
	{
		int size;
		uint8_t data[2048*2048];
	};

	struct PvrTexture
	{
		Slice image_data[10];
		int image_data_count;

		int internal_format;
		int width;
		int height;
		int has_alpha;
	};

private:
	PvrTexture m_tex;

}; // PackPVR

}

#endif // _LIBRESPACKER_PACK_PVR_H_