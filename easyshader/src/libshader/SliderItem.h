#ifndef _EASYSHADER_SLIDER_ITEM_H_
#define _EASYSHADER_SLIDER_ITEM_H_

#include <string>

namespace eshader 
{

template<typename T>
struct SliderItem
{		
	std::string title;

	T default;
	T min, max;

	SliderItem(const std::string& title, T default, T min, T max)
		: title(title), default(default), min(min), max(max) {}

}; // SliderItem

typedef SliderItem<float> SliderItemFloat;
typedef SliderItem<int> SliderItemInt;

}

#endif // _EASYSHADER_SLIDER_ITEM_H_