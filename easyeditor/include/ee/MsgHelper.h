#ifndef _EASYEDITOR_MSG_HELPER_H_
#define _EASYEDITOR_MSG_HELPER_H_

#include "Sprite.h"

#include <json/json.h>

#include <vector>

namespace ee
{

class MsgHelper
{
public:
	static void FillingSprites(const std::vector<SprPtr>& children, Json::Value& val);
	static void FillingSprites(const Sprite& child, Json::Value& val);

}; // MsgHelper

}

#endif // _EASYEDITOR_MSG_HELPER_H_