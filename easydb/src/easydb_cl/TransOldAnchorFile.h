#ifndef _EASYDB_TRANS_OLD_ANCHOR_FILE_H_
#define _EASYDB_TRANS_OLD_ANCHOR_FILE_H_

#include "ICommand.h"

#include <drag2d.h>

namespace edb
{

class TransOldAnchorFile : public ICommand
{
public:
	TransOldAnchorFile() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransOldAnchorFile(); }

private:
	void Run(const std::string& folder);

private:
	void TransComplex(const std::string& filepath) const;
	void TransAnimation(const std::string& filepath) const;

	bool IsAnchor(const d2d::ISprite* spr) const;

}; // TransOldAnchorFile


}

#endif // _EASYDB_TRANS_OLD_ANCHOR_FILE_H_