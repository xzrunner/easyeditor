#ifndef _EASYDB_PACK_EP_H_
#define _EASYDB_PACK_EP_H_

#include "ICommand.h"

#include <drag2d.h>
#include <easycoco.h>

namespace edb
{

class PackEP : public ICommand
{
public:
	PackEP() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackEP(); }

	void Trigger(const std::string& json_dir, const std::string& tp_json,
		const std::string& tp_dir, const std::string& out_file, const std::string& type,
		int LOD);

}; // PackEP

}

#endif // _EASYDB_PACK_EP_H_