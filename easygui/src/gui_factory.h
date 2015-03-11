#ifndef _EASYGUI_FACTORY_H_
#define _EASYGUI_FACTORY_H_

#include "easygui.h"

#include <string>

namespace egui
{

Frame* create_frame(const std::string& title);

Dialog* create_dialog(Window* parent, const std::string& title);

}

#endif // _EASYGUI_FACTORY_H_