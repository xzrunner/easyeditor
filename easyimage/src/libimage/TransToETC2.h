#ifndef _EASYIMAGE_TRANS_TO_ETC2_H_
#define _EASYIMAGE_TRANS_TO_ETC2_H_

#include <string>

#include <stdint.h>

namespace eimage
{

class TransToETC2
{
public:
	TransToETC2(const uint8_t* pixels, int width, int height, int channels,
		bool align_bottom, bool fastest);
	~TransToETC2();

	void OutputFile(const std::string& filepath) const;

	uint8_t* GetPixelsData(int& width, int& height) const;

private:
	void Prepare(const uint8_t* pixels);
	void Prepare(const uint8_t* pixels, int width, int height, int channels, bool align_bottom);

	void Encode();

	void OutputHeader(std::ofstream& fout) const;

private:
	static void WriteBigEndian2byteWord(unsigned short* blockadr, std::ofstream& fout);
	static void WriteBigEndian4byteWord(unsigned short* blockadr, std::ofstream& fout);

private:
	bool m_fastest;
	bool m_perceptual;

	int m_width, m_height;

	uint8_t *m_rgb_data, *m_alpha_data;

	uint8_t* m_compressed;

}; // TransToETC2

}

#endif // _EASYIMAGE_TRANS_TO_ETC2_H_