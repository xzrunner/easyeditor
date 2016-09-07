#ifndef _EASYDB_FIX_OLD_SCALE9_H_
#define _EASYDB_FIX_OLD_SCALE9_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixOldScale9 : public ICommand
{
public:
	FixOldScale9() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new FixOldScale9(); }

private:
	void Trigger(const std::string& dir) const;

	void FixScale9(const std::string& filepath) const;
	bool FixScale9Spr(Json::Value& val) const;

}; // FixOldScale9 


}

#endif // _EASYDB_FIX_OLD_SCALE9_H_