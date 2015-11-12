#ifndef _EASYUI_WIDGET_ID_H_
#define _EASYUI_WIDGET_ID_H_

#include <string>

namespace eui
{

enum WIDGET_ID {
	ID_WINDOW = 1,
	ID_LIST,
	ID_WRAPPER
};

const std::string& get_widget_desc(WIDGET_ID id);

bool is_widget(const std::string& desc, WIDGET_ID id);

}

#endif // _EASYUI_WIDGET_ID_H_S