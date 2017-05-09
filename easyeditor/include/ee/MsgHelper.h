#ifndef _EASYEDITOR_MSG_HELPER_H_
#define _EASYEDITOR_MSG_HELPER_H_

#include <json/json.h>

#include <vector>

namespace ee
{

class Sprite;

class MsgHelper
{
public:
	static void FillingSprites(const std::vector<Sprite*>& children, Json::Value& val);
	static void FillingSprites(const Sprite* child, Json::Value& val);

}; // MsgHelper

}

#endif // _EASYEDITOR_MSG_HELPER_H_