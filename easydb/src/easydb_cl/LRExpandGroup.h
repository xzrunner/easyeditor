#ifndef _EASYDB_LR_EXPAND_GROUP_H_
#define _EASYDB_LR_EXPAND_GROUP_H_

#include "ICommand.h"

#include <json/json.h>

namespace edb
{

class LRExpandGroup : public ICommand
{
public:
	LRExpandGroup() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRExpandGroup(); }

	void Run(const std::string& filepath);

private:
	void Expand(const Json::Value& src_val, Json::Value& dst_val);

	void LoadSprites(const Json::Value& src_spr_val, Json::Value& dst_sprs_val);

}; // LRExpandGroup

}

#endif // _EASYDB_LR_EXPAND_GROUP_H_