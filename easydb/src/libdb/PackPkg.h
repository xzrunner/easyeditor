#ifndef _EASYDB_PACK_PKG_H_
#define _EASYDB_PACK_PKG_H_

#include "ICommand.h"

#include <stdint.h>

namespace edb
{

class PackPkg : public ICommand
{
public:
	PackPkg();
	virtual ~PackPkg();

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackPkg(); }

	void Trigger(const std::string& dir, const std::string& name);

private:
	static uint32_t GetFileSize(const std::string& filepath);
	
	void WriteFile(const std::string& src, uint32_t size, std::ofstream& fout);

private:
	unsigned char* m_buf;
	int m_buf_size;

}; // PackPkg

}

#endif // _EASYDB_PACK_PKG_H_