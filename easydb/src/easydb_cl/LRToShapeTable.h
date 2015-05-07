#ifndef _EASYDB_LR_TO_SHAPE_TABLE_H_
#define _EASYDB_LR_TO_SHAPE_TABLE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class LRToShapeTable : public ICommand
{
public:
	LRToShapeTable() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new LRToShapeTable(); }

private:
	void Run(const std::string& filepath);

}; // LRToShapeTable

}

#endif // _EASYDB_LR_TO_SHAPE_TABLE_H_