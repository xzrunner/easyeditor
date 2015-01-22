#ifndef _EPBIN_BINARARY_RRP_H_
#define _EPBIN_BINARARY_RRP_H_

#include "typedef.h"

#include <vector>

namespace epbin
{

class BinaryRRP
{
public:
	BinaryRRP(const std::string& json_file, const std::string& img_id_file);
	~BinaryRRP();

	void Pack(const std::string& outfile, bool compress) const;

private:
	void LoadMulti(const std::string& json_file, const std::string& img_id_file);
	void LoadSingleParts(const std::string& json_file, const std::string& img_id_file, int pic_idx);

private:
	struct Rect
	{
		int16_t w, h;
		int16_t x, y;
	};

	struct Part
	{
		Rect src, dst;
		std::string filepath;
		int8_t idx;

		size_t Size() const;
		void Store(uint8_t** ptr);
	};

	struct PartCmp
	{
		bool operator () (const Part* lhs, const Part* rhs) const {
			return lhs->filepath.compare(rhs->filepath) < 0;
		}
	};

	struct Picture
	{
		std::vector<Part*> parts;
		int16_t w, h;

		std::string path;
		int16_t id;

		size_t Size() const;
		void Store(uint8_t** ptr);
	};

private:
	std::vector<Part*> m_parts;
	std::vector<Picture*> m_pics;

}; // BinaryRRP

}

#endif // _EPBIN_BINARARY_RRP_H_