#ifndef _EASYDB_EXPORT_TEXT_H_
#define _EASYDB_EXPORT_TEXT_H_

#include "ICommand.h"

#include <vector>

namespace edb
{

class ExportText : public ICommand
{
public:
	ExportText() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ExportText(); }

private:
	void Trigger(const std::string& src_dir, const std::string& dst_file);

	void Load(const std::string& dir);
	void Export(const std::string& file);

	void SetTID(const std::string& dir);

private:
	struct Text
	{
		std::string tid;
		std::vector<std::string> language;
	};

private:
	std::vector<Text> m_all_texts;

}; // ExportText

}

#endif // _EASYDB_EXPORT_TEXT_H_