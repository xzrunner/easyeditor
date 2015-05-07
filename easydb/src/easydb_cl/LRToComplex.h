#ifndef _EASYDB_LR_TO_COMPLEX_H_
#define _EASYDB_LR_TO_COMPLEX_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class LRToComplex : public ICommand
{
public:
	LRToComplex() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRToComplex(); }

private:
	void Run(const std::string& filepath);

	void LoadSpriteValue(const Json::Value& src_val, Json::Value& dst_val);

}; // LRToComplex

}

#endif // _EASYDB_LR_TO_COMPLEX_H_