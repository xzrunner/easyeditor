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

	void InitInvisibleSet(const Json::Value& layer_val, 
		std::set<std::string>& invisible) const;

	Json::Value FilterSprites(const Json::Value& sprite_val, 
		const std::set<std::string>& invisible) const;

}; // LRToComplex

}

#endif // _EASYDB_LR_TO_COMPLEX_H_