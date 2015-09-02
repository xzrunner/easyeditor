#ifndef _DRAG2D_SLIDER_ITEM_H_
#define _DRAG2D_SLIDER_ITEM_H_

namespace d2d
{

struct SliderItem
{
	std::string title;
	std::string name;

	int val;
	int min;
	int max;

	SliderItem(const char* title, const char* name, int val , int min, int max) 
		: title(title), name(name), val(val), min(min), max(max) {}

}; // SliderItem

}

#endif // _DRAG2D_SLIDER_ITEM_H_