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
		float start, end, duration;
		float timestamp;
	};

	struct Marker
	{
		float location;
		std::string name;
	};

private:
	void Trigger(const std::string& cfg_file, const std::string& dst_dir);

	void LoadFormCfg(const std::string& cfg_file);
	void LoadTracks(std::ifstream& fin);
	void LoadMarkers(std::ifstream& fin);

	void OutputAudio(const std::string& dst_dir);

	std::shared_ptr<eaudio::Sprite> CreateAudioSpr(const Track* track, const Track* track_fadeout);
	void CreateAllAudioSpr(s2::AnimSymbol::Layer& layer, float start_time, float end_time);

	static float TransTime(const std::string& time);

private:
	std::string m_cfg_dir;

	std::map<float, Track> m_tracks;
	std::vector<Marker>    m_markers;

}; // ExportAudio

}

#endif // _EASYDB_EXPORT_AUDIO_H_