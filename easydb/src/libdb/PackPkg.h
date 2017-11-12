#ifndef _EASYDB_PACK_PKG_H_
#define _EASYDB_PACK_PKG_H_

#include "ICommand.h"

#include <bimp/FileLoader.h>

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
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackPkg(); }

	void Trigger(const std::string& dir, const std::string& name);

private:
	static uint32_t GetFileSize(const std::string& filepath);
	
	void WriteFile(const std::string& src, uint32_t size, std::ofstream& fout);

	class EptLoader : public bimp::FileLoader
	{
	public:
		EptLoader(const std::string& filepath);

		int GetCount() const;

	protected:
		virtual void OnLoad(bimp::ImportStream& is) override;

	private:
		int m_count;

	}; // EptLoader

private:
	unsigned char* m_buf;
	int m_buf_size;

}; // PackPkg

}

#endif // _EASYDB_PACK_PKG_H_