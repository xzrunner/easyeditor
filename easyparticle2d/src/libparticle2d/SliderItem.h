#ifndef _EASYPARTICLE2D_SLIDER_ITEM_H_
#define _EASYPARTICLE2D_SLIDER_ITEM_H_

namespace eparticle2d
{

struct SliderItem
{
	std::string title;
	std::string name;

	int default;
	int min;
	int max;

	SliderItem(const char* title, const char* name, int default, int min, int max) 
		: title(title), name(name), default(default), min(min), max(max) {}

}; // SliderItem

}

#endif // _EASYPARTICLE2D_SLIDER_ITEM_H_