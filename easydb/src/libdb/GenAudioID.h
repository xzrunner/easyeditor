#ifndef _EASYDB_GEN_AUDIO_ID_H_
#define _EASYDB_GEN_AUDIO_ID_H_

#include "ICommand.h"

#include <json/json.h>

#include <vector>

namespace edb
{

class GenAudioID : public ICommand
{
public:
	GenAudioID() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new GenAudioID(); }

	void Trigger(const std::string& src_dir, const std::string& dst_file);

}; // GenAudioID

}

#endif // _EASYDB_GEN_AUDIO_ID_H_