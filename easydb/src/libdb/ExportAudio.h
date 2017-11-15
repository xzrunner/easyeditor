#ifndef _EASYDB_EXPORT_AUDIO_H_
#define _EASYDB_EXPORT_AUDIO_H_

#include "ICommand.h"

#include <vector>

namespace edb
{

class ExportAudio : public ICommand
{
public:
	ExportAudio() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const override;
	virtual std::string Description() const override;
	virtual std::string Usage() const override;
	virtual int Run(int argc, char *argv[]) override;

	static ICommand* Create() { return new ExportAudio(); }

private:
	struct Track
	{
		int channel;
		int event;
		std::string name;
		std::string start, end, duration;
		std::string timestamp;
	};

	struct Marker
	{
		std::string location;
		std::string name;
	};

private:
	void Trigger(const std::string& cfg_file, const std::string& dst_dir);

	void LoadFormCfg(const std::string& cfg_file);
	void LoadTracks(std::ifstream& fin);
	void LoadMarkers(std::ifstream& fin);

	void OutputAudio(const std::string& dst_dir);

	static float TransTime(const std::string& time);

private:
	std::string m_cfg_dir;

	std::vector<Track> m_tracks;
	std::vector<Marker> m_markers;

}; // ExportAudio

}

#endif // _EASYDB_EXPORT_AUDIO_H_