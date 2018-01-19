#pragma once

#include "ICommand.h"

namespace edb
{

class PackSceneNode : public ICommand
{
public:
	PackSceneNode() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new PackSceneNode(); }

private:
	void Pack(const std::string& src_dir, const std::string& dst_dir,
		const std::string& tp_src_dir, const std::string& tp_filepath);

}; // PackSceneNode

}