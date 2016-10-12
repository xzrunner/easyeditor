#ifndef _EASYDB_FIX_OLD_SCALE9_H_
#define _EASYDB_FIX_OLD_SCALE9_H_

#include "ICommand.h"
#include "FixJsonBase.h"

#include <SM_Vector.h>

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixMirror : public ICommand, private FixJsonBase
{
public:
	FixMirror() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new FixMirror(); }

	static bool Fix(const Json::Value& sprite_val, sm::vec2& fixed);

protected:
	virtual bool FixSprite(const std::string& filepath, Json::Value& sprite_val) const;

private:
	void Trigger(const std::string& dir) const;

}; // FixMirror 


}

#endif // _EASYDB_FIX_OLD_SCALE9_H_