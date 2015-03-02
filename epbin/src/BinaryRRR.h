#ifndef _EPBIN_BINARARY_RRR_H_
#define _EPBIN_BINARARY_RRR_H_

#include "typedef.h"

#include <string>
#include <vector>

namespace epbin
{

class BinaryRRR
{
public:
	BinaryRRR(const std::vector<std::string>& src_files, 
		const std::string& img_id_file, bool is_pvr);
	~BinaryRRR();

	void Pack(const std::string& outfile, bool compress) const;

private:
	struct Picture;

	struct Part
	{
		// block position
		int16_t x, y;
		int16_t w, h;

		Picture* pic;

		Part(Picture* pic) : pic(pic) {}

		size_t Size(bool is_pvr) const;
		void Store(bool is_pvr, uint8_t** ptr);

		static size_t BlockSize(bool is_pvr);

	}; // Part

	struct Picture
	{
		int16_t bmp_w, bmp_h;

		std::vector<Part> parts;
		int16_t w, h;
		int16_t x, y;

		std::string path;
		int16_t id;

		uint8_t* pixels;

		size_t Size(bool is_pvr) const;
		void Store(bool is_pvr, uint8_t** ptr);

	}; // Picture

private:
	void LoadPictures(const std::vector<std::string>& src_files,
		const std::string& img_id_file);

	Picture* CreatePicture(const std::string& filepath) const;

private:
	std::vector<Picture*> m_pics;

	bool m_is_pvr;

}; // BinaryRRR

}

#endif // _EPBIN_BINARARY_RRR_H_