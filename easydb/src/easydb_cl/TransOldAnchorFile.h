#ifndef _EASYDB_TRANS_OLD_ANCHOR_FILE_H_
#define _EASYDB_TRANS_OLD_ANCHOR_FILE_H_

#include "ICommand.h"

namespace ee { class Sprite; }

namespace edb
{

class TransOldAnchorFile : public ICommand
{
public:
	TransOldAnchorFile() {}

	//
	// interface ICommand
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

	bool IsAnchor(const ee::Sprite* spr) const;

}; // TransOldAnchorFile


}

#endif // _EASYDB_TRANS_OLD_ANCHOR_FILE_H_