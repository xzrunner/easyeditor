#ifndef _EASYDB_LR_EXPANSION_H_
#define _EASYDB_LR_EXPANSION_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class LRExpansion : public ICommand
{
public:
	LRExpansion() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRExpansion(); }

	void Run(const std::string& filepath);

	std::string GetOutputFilepath(const std::string& filepath) const;

private:
	void ExtendLayer(const std::string& filepath, int layer_idx, Json::Value& dst_val);

}; // LRExpansion

}

#endif // _EASYDB_LR_EXPANSION_H_