#ifndef _EASYIMAGE_TRANS_TO_ETC2_H_
#define _EASYIMAGE_TRANS_TO_ETC2_H_

#include <string>

#include <stdint.h>

namespace eimage
{

class TransToETC2
{
public:
	enum Type
	{
		RGB = 0,
		RGBA,
		RGBA1
	};

public:
	TransToETC2(const uint8_t* pixels, int width, int height, int channels,
		Type type = RGBA, bool align_bottom = false, bool fastest = false);
	~TransToETC2();

	void OutputFile(const std::string& filepath) const;

	uint8_t* GetPixelsData(int& width, int& height) const;

private:
	void Prepare(const uint8_t* pixels);
	void Prepare(const uint8_t* pixels, int width, int height, int channels, bool align_bottom);

	void Encode();

	void OutputHeader(std::ofstream& fout) const;

	void ComputeRGB(uint8_t* decoded, int x, int y, unsigned int& block1, unsigned int& block2);
	void ComputeAlpha(int x, int y, uint8_t* alphadata);

private:
	static void WriteBigEndian2byteWord(unsigned short* blockadr, std::ofstream& fout);
	static void WriteBigEndian4byteWord(unsigned short* blockadr, std::ofstream& fout);

private:
	Type m_type;

	bool m_fastest;
	bool m_perceptual;

	int m_width, m_height;
	int m_size;

	uint8_t *m_rgb_data, *m_alpha_data;

	uint8_t* m_compressed;

}; // TransToETC2

}

#endif // _EASYIMAGE_TRANS_TO_ETC2_H_