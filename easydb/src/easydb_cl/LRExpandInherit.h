#ifndef _EASYDB_LR_EXPAND_INHERIT_H_
#define _EASYDB_LR_EXPAND_INHERIT_H_

#include "ICommand.h"

#include <json/json.h>

namespace edb
{

class LRExpandInherit : public ICommand
{
public:
	LRExpandInherit() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRExpandInherit(); }

	void Run(const std::string& filepath);

	std::string GetOutputFilepath(const std::string& filepath) const;

private:
	void ExtendLayer(const std::string& dir, const std::string& filepath, int layer_idx, Json::Value& dst_val);

}; // LRExpandInherit

}

#endif // _EASYDB_LR_EXPAND_INHERIT_H_