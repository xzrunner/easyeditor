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
		const std::string& img_id_file);
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

		size_t Size() const;
		void Store(uint8_t** ptr);

	}; // Part

	struct Picture
	{
		std::vector<Part> parts;
		int16_t w, h;
		int16_t x, y;

		std::string path;
		int16_t id;

		uint8_t* pixels;

		size_t Size() const;
		void Store(uint8_t** ptr);

	}; // Picture

private:
	void LoadPictures(const std::vector<std::string>& src_files,
		const std::string& img_id_file);

	Picture* CreatePicture(const std::string& filepath) const;

	void PackPicture(const Picture* pic) const;

private:
	std::vector<Picture*> m_pics;

}; // BinaryRRR

}

#endif // _EPBIN_BINARARY_RRR_H_