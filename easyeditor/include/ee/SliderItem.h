#ifndef _EASYEDITOR_SLIDER_ITEM_H_
#define _EASYEDITOR_SLIDER_ITEM_H_

namespace ee
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

#endif // _EASYEDITOR_SLIDER_ITEM_H_