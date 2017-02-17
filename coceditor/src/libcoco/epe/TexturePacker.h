#ifndef _COCO_EPE_TEXTURE_PACKER_H_
#define _COCO_EPE_TEXTURE_PACKER_H_

#include <ee/Image.h>

#include <SM_Rect.h>

#include <set>
#include <map>

namespace ee { class Image; class Rect; }

namespace ecoco
{
namespace epe
{

class TexturePacker
{
public:
	enum Type
	{
		e_png,
		e_ppm
	};

public:
	TexturePacker(int padding = 1, int extrude = 1);

	void Pack(const std::set<ee::Image*>& images);

	void StoreToMemory();
	void StoreToFile(const std::string& floder, const std::string& filename);

	const sm::rect* Query(ee::Image* image) const;

private:
	std::map<ee::Image*, sm::rect> m_map_img2rect;
	
	int m_edge;

	int m_xcurr, m_ycurr, m_width;

	uint8_t* m_pixels;

	int m_padding;
	int m_extrude;

}; // TexturePacker

class ImageCmp
{
public:
	enum Type
	{
		e_width = 0,
		e_height,
		e_area
	};

public:
	ImageCmp(Type type = e_width) : m_type(type) {}

	bool operator() (const ee::Image* t0, const ee::Image* t1) const 
	{
		switch (m_type)
		{
		case e_width:
			return t0->GetClippedRegion().Width() > t1->GetClippedRegion().Width();
		case e_height:
			return t0->GetClippedRegion().Height() > t1->GetClippedRegion().Height();
		case e_area:
			return t0->GetClippedRegion().Width() * t0->GetClippedRegion().Height() > t1->GetClippedRegion().Width() * t1->GetClippedRegion().Height();
		default:
			return t0->GetClippedRegion().Width() > t1->GetClippedRegion().Width();
		}
	}

private:
	Type m_type;

}; // ImageCmp

}
}

#endif // _COCO_EPE_TEXTURE_PACKER_H_