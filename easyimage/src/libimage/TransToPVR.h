#ifndef _EASYIMAGE_TRANS_TO_PVR_H_
#define _EASYIMAGE_TRANS_TO_PVR_H_



namespace eimage
{

class TransToPVR
{
public:
	TransToPVR(const uint8_t* pixels, int width, int height, int channels,
		bool align_bottom = false);
	~TransToPVR();

	void OutputFile(const std::string& filepath) const;

	uint8_t* GetPixelsData(int& width, int& height) const;

private:
	struct PVRTexHeader {
		uint32_t headerLength;
		uint32_t height;
		uint32_t width;
		uint32_t numMipmaps;
		uint32_t flags;
		uint32_t dataLength;
		uint32_t bpp;
		uint32_t bitmaskRed;
		uint32_t bitmaskGreen;
		uint32_t bitmaskBlue;
		uint32_t bitmaskAlpha;
		uint32_t pvrTag;
		uint32_t numSurfs;
	};

private:
	void InitSrcImage(const uint8_t* pixels, int width, int height, int channels,
		bool align_bottom);

	void InitPVRHeader();

	void TransPVR();

private:
	uint8_t* m_pixels;
	int m_width, m_height;

	size_t m_pvr_size;
	uint8_t* m_pvr_pixels;

	PVRTexHeader m_header;

}; // TransToPVR

}

#endif // _EASYIMAGE_TRANS_TO_PVR_H_