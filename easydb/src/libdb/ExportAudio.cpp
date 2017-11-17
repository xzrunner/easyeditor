#include "ExportAudio.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>

#include <easyanim.h>
#include <easyaudio.h>

#include <sprite2/AnimSymbol.h>
#include <gum/Audio.h>

#include <fstream>
#include <sstream>

#include <assert.h>

namespace edb
{

static const std::string FADE_OUT_STR = "fadeout";

std::string ExportAudio::Command() const
{
	return "export-audio";
}

std::string ExportAudio::Description() const
{
	return "export audio";
}

std::string ExportAudio::Usage() const
{
	std::string usage = Command() + " [cfg file] [dst dir]";
	return usage;
}

int ExportAudio::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (!check_file(argv[2])) return -1;
	if (!check_folder(argv[3])) return -1;

	gum::Audio::Instance()->Initialize(nullptr, nullptr);

	Trigger(argv[2], argv[3]);

	return 0;
}

void ExportAudio::Trigger(const std::string& cfg_file, const std::string& dst_dir)
{
	m_cfg_dir = ee::FileHelper::GetFileDir(cfg_file);

	LoadFormCfg(cfg_file);
	OutputAudio(dst_dir);
}

void ExportAudio::LoadFormCfg(const std::string& cfg_file)
{
	std::ifstream fin(cfg_file.c_str());

	std::string line_str;
	std::string skip;

	// skip
	while (true) {
		std::getline(fin, line_str);
		if (line_str.find("T R A C K  L I S T I N G") != std::string::npos) {
			break;
		}
	}
	std::getline(fin, line_str);	// TRACK NAME
	std::getline(fin, line_str);	// COMMENTS
	std::getline(fin, line_str);	// USER DELAY
	std::getline(fin, line_str);	// STATE
	std::getline(fin, line_str);	// PROP
	// TRACK NAME:	music (Stereo)
	LoadTracks(fin);

	// skip
	while (true) {
		std::getline(fin, line_str);
		if (line_str.find("TRACK NAME:") != std::string::npos) {
			break;
		}
	}
	std::getline(fin, line_str);	// COMMENTS
	std::getline(fin, line_str);	// USER DELAY
	std::getline(fin, line_str);	// STATE
	std::getline(fin, line_str);	// PROP
	// TRACK NAME:	amb (Stereo)
	LoadTracks(fin);

	// skip
	while (true) {
		std::getline(fin, line_str);
		if (line_str.find("M A R K E R S  L I S T I N G") != std::string::npos) {
			break;
		}
	}
	std::getline(fin, line_str);	// PROP
	// read markers
	LoadMarkers(fin);

	fin.close();
}

void ExportAudio::LoadTracks(std::ifstream& fin)
{
	std::string line_str;
	std::string skip;

	while (true) {
		std::getline(fin, line_str);
		if (line_str.empty()) {
			break;
		}

		std::stringstream ss(line_str.c_str());
		Track track;
		ss >> track.channel
			>> track.event
			>> track.name;
		if (track.name == "(fade") {
			track.name = FADE_OUT_STR;
			ss >> skip;
		}
		ss >> track.start
			>> track.end
			>> track.duration
			>> track.timestamp
			>> skip;
		m_tracks.push_back(track);
	}
}

void ExportAudio::LoadMarkers(std::ifstream& fin)
{
	std::string line_str;
	std::string skip;

	while (true) {
		std::getline(fin, line_str);
		if (line_str.empty()) {
			break;
		}

		std::stringstream ss(line_str.c_str());
		Marker marker;
		ss >> skip >> marker.location >> skip >> skip >> marker.name;
		m_markers.push_back(marker);
	}
}

void ExportAudio::OutputAudio(const std::string& dst_dir)
{
	for (auto& marker : m_markers)
	{
		// query
		Track* track = nullptr;
		Track* track_fadeout = nullptr;
		for (int i = 0, n = m_tracks.size(); i < n; ++i) {
			if (marker.location == m_tracks[i].start) {
				track = &m_tracks[i];
				if (i + 1 < n && m_tracks[i + 1].name == FADE_OUT_STR) {
					track_fadeout = &m_tracks[i + 1];
				}
			}
		}
		assert(track);

		// init sym
		libanim::Symbol sym;
		auto layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
		auto frame = CU_MAKE_UNIQUE<s2::AnimSymbol::Frame>();

		frame->index = 1;

		auto audio_sym = std::make_shared<eaudio::Symbol>();
		std::string clip_name = track->name.substr(0, track->name.find("-"));
		std::string::size_type pos = clip_name.find(".L");
		if (pos != std::string::npos) {
			clip_name = clip_name.substr(0, pos);
		}
		audio_sym->SetFilepath(m_cfg_dir + "\\" + clip_name + ".mp3");
		
		auto audio_spr = std::make_shared<eaudio::Sprite>(audio_sym);
		if (track_fadeout) {
			float duration = TransTime(track->duration);
			float fade_out = TransTime(track_fadeout->duration);
			audio_spr->SetAudioDuration(duration + fade_out);
			audio_spr->SetFadeOut(fade_out);
		} else {
			audio_spr->SetAudioDuration(TransTime(track->duration));
		}

		audio_spr->SetAudioOffset(TransTime(track->timestamp));

		frame->sprs.push_back(audio_spr);

		layer->frames.push_back(std::move(frame));

		sym.AddLayer(layer);

		// output
		std::vector<std::string> tokens;
		ee::StringHelper::Split(marker.name, "-", tokens);
		std::string out_dir = dst_dir + "//manga_" + tokens[0] + "_" + tokens[1] + "_" + tokens[2] + "//json";
		assert(ee::FileHelper::IsDirExist(out_dir));
		std::string filepath = out_dir + "//audio_anim.json";
		libanim::FileSaver::Store(filepath, sym);
	}
}

float ExportAudio::TransTime(const std::string& time)
{
	std::vector<std::string> tokens;
	ee::StringHelper::Split(time, ":", tokens);
	float second0, second1;
	ee::StringHelper::FromString(tokens[2], second0);
	ee::StringHelper::FromString(tokens[3], second1);
	return second0 + second1 / 60.0f;
}

}