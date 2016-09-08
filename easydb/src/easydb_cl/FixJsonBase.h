#ifndef _EASYDB_FIX_JSON_BASE_H_
#define _EASYDB_FIX_JSON_BASE_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixJsonBase
{
protected:
	virtual bool FixSprite(const std::string& filepath, Json::Value& sprite_val) const = 0;

	void FixComplex(const std::string& filepath) const;
	void FixAnim(const std::string& filepath) const;
	void FixScale9(const std::string& filepath) const;

}; // FixJsonBase

}

#endif // _EASYDB_FIX_JSON_BASE_H_