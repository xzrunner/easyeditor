#ifndef _EASYDB_FIX_LR_MIRROR_H_
#define _EASYDB_FIX_LR_MIRROR_H_

#include "ICommand.h"

#include <wx/wx.h>
#include <json/json.h>

namespace edb
{

class FixLRMirror : public ICommand
{
public:
	FixLRMirror() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new FixLRMirror(); }

private:
	void Trigger(const std::string& old_dir, const std::string& new_dir) const;

	bool Fix(const Json::Value& old_val, Json::Value& new_val) const;

	bool FixSprite(const Json::Value& old_val, Json::Value& new_val) const;

	bool StoreToNew(const Json::Value& old_val, Json::Value& new_val) const;

}; // FixLRMirror 

}

#endif // _EASYDB_FIX_LR_MIRROR_H_