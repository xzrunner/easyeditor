#include "widget_id.h"

namespace eui
{

static const std::string TYPE_UNKNOWN	= "";
static const std::string TYPE_WINDOW	= "window";
static const std::string TYPE_LIST		= "list";
static const std::string TYPE_WRAPPER	= "wrapper";

const std::string& get_widget_desc(WIDGET_ID id)
{
	if (id == ID_WINDOW) {
		return TYPE_WINDOW;
	} else if (id == ID_LIST) {
		return TYPE_LIST;
	} else if (id == ID_WRAPPER) {
		return TYPE_WRAPPER;
	} else {
		return TYPE_UNKNOWN;
	}
}

bool is_widget(const std::string& desc, WIDGET_ID id)
{
	return get_widget_desc(id) == desc;
}

}