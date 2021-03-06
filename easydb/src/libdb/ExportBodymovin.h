#ifndef _EASYDB_EXPORT_BODYMOVIN_H_
#define _EASYDB_EXPORT_BODYMOVIN_H_

#include "ICommand.h"

#include <vector>

namespace edb
{

class ExportBodymovin : public ICommand
{
public:
	ExportBodymovin() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ExportBodymovin(); }

private:
	void Trigger(const std::string& src_file, const std::string& dst_dir);

	void FixFontLayers(const std::string& dir);
	void FixFontLayer(const std::string& filepath, const std::string& dir);

}; // ExportBodymovin

}

#endif // _EASYDB_EXPORT_BODYMOVIN_H_