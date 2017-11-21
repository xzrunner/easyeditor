#ifndef _EASYDB_EXPORT_AUDIO_H_
#define _EASYDB_EXPORT_AUDIO_H_

#include "ICommand.h"

#include <sprite2/AnimSymbol.h>

#include <vector>
#include <memory>

namespace eaudio { class Sprite; }

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
	static void LoadTracks(std::ifstream& fin, std::vector<Track>&);
	void LoadMarkers(std::ifstream& fin);

	void OutputAudio(const std::string& dst_dir);

	std::shared_ptr<eaudio::Sprite> CreateAudioSpr(const Track* track, const Track* track_fadeout);
	void CreateAllAudioSpr(s2::AnimSymbol::Layer& layer, const std::vector<Track>& tracks, const std::string start_time, const std::string end_time);

	bool IsAmbTime(const std::string& time) const;

	static float TransTime(const std::string& time);

private:
	std::string m_cfg_dir;

	std::vector<Track> m_tracks_music, m_tracks_amb;
	std::vector<Marker> m_markers;

}; // ExportAudio

}

#endif // _EASYDB_EXPORT_AUDIO_H_