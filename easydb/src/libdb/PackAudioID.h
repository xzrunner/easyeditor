#ifndef _EASYDB_PACK_AUDIO_ID_H_
#define _EASYDB_PACK_AUDIO_ID_H_

#include "ICommand.h"

#include <vector>

namespace edb
{

class PackAudioID : public ICommand
{
public:
	PackAudioID() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new PackAudioID(); }

	void Pack(const std::string& src, const std::string& dst);

private:
	void LoadAudioID(const std::string& src);
	void StoreAudioID(const std::string& dst) const;

private:
	struct AudioID
	{
		std::string name;
		int id;
	};

private:
	std::vector<AudioID> m_audios;

}; // PackAudioID

}

#endif // _EASYDB_PACK_AUDIO_ID_H_