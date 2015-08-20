#ifndef _EPBIN_PVR_LOADER_H_
#define _EPBIN_PVR_LOADER_H_

#include "TexLoader.h"

#include <stdint.h>

namespace epbin
{

class PVRLoader : public TexLoader
{
public:
	PVRLoader();
	virtual ~PVRLoader();

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

}; // PvrLoader

}

#endif // _EPBIN_PVR_LOADER_H_