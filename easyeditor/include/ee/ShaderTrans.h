#ifndef _EASYEDITOR_SHADER_TRANS_H_
#define _EASYEDITOR_SHADER_TRANS_H_

#include "BlendModes.h"
#include "FilterModes.h"

#include <json/json.h>

namespace ee
{

class ShaderTrans
{
public:
	ShaderTrans();

	void LoadFromFile(const Json::Value& val);
	void StoreToFile(Json::Value& val) const;

public:
	BlendMode blend;
	FilterMode filter;

}; // ShaderTrans

}

#endif // _EASYEDITOR_SHADER_TRANS_H_
