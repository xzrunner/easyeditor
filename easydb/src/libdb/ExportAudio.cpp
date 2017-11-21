#include "ExportAudio.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/StringHelper.h>

#include <easyanim.h>
#include <easyaudio.h>

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
	LoadTracks(fin, m_tracks_music);

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
	LoadTracks(fin, m_tracks_amb);

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

void ExportAudio::LoadTracks(std::ifstream& fin, std::vector<Track>& tracks)
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
		tracks.push_back(track);
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
	for (int i = 0, n = m_markers.size(); i < n; ++i)
	{
		// query track
		std::string start_time = m_markers[i].location;
		std::string end_time;
		bool is_music_tracks = false;
		if (IsAmbTime(start_time)) {
			is_music_tracks = false;
			end_time = "only_one";
		} else {
			is_music_tracks = true;
			if (i < n - 1) {
				end_time = m_markers[i + 1].location;
				if (IsAmbTime(end_time) && i < n - 2) {
					end_time = m_markers[i + 2].location;
				}
			}
		}

		// create sym
		libanim::Symbol sym;
		auto layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
		if (is_music_tracks) {
			CreateAllAudioSpr(*layer, m_tracks_music, start_time, end_time);
		} else {
			CreateAllAudioSpr(*layer, m_tracks_amb, start_time, end_time);
		}
		sym.AddLayer(layer);

		// output
		std::vector<std::string> tokens;
		ee::StringHelper::Split(m_markers[i].name, "-", tokens);
		std::string out_dir = dst_dir + "//manga_" + tokens[0] + "_" + tokens[1] + "_" + tokens[2] + "//json";
		assert(ee::FileHelper::IsDirExist(out_dir));
		std::string filepath = out_dir + "//audio_anim.json";
		libanim::FileSaver::Store(filepath, sym);
	}
}

void ExportAudio::CreateAllAudioSpr(s2::AnimSymbol::Layer& layer, const std::vector<Track>& tracks, const std::string start_time, const std::string end_time)
{
	int start_ptr = -1;
	int end_ptr = -1;
	for (int i = 0, n = tracks.size(); i < n; ++i) {
		if (tracks[i].start == start_time) {
			start_ptr = i;
			break;
		}
	}
	assert(start_ptr != -1);
	if (!end_time.empty()) 
	{
		if (end_time == "only_one") {
			end_ptr = start_ptr + 1;
		} else {
			for (int i = start_ptr + 1, n = tracks.size(); i < n; ++i) {
				if (tracks[i].start == end_time) {
					end_ptr = i;
					break;
				}
			}
		}
	}
	if (end_ptr == -1) {
		end_ptr = tracks.size();
	}

	for (int curr_ptr = start_ptr; curr_ptr < end_ptr; )
	{
		float time = TransTime(tracks[curr_ptr].start) - TransTime(start_time);

		int next_ptr = curr_ptr + 1;
		std::shared_ptr<eaudio::Sprite> spr;
		if (next_ptr < end_ptr && tracks[next_ptr].name == FADE_OUT_STR) {
			spr = CreateAudioSpr(&tracks[curr_ptr], &tracks[next_ptr]);
			curr_ptr += 2;
		} else {
			spr = CreateAudioSpr(&tracks[curr_ptr], nullptr);
			curr_ptr += 1;
		}

		// create spr
		auto frame = CU_MAKE_UNIQUE<s2::AnimSymbol::Frame>();
		frame->index = time * 30 + 1;
		frame->sprs.push_back(spr);
		layer.frames.push_back(std::move(frame));
	}
}

std::shared_ptr<eaudio::Sprite> ExportAudio::CreateAudioSpr(const Track* track, const Track* track_fadeout)
{
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

	return audio_spr;
}

bool ExportAudio::IsAmbTime(const std::string& time) const
{
	for (auto& amb : m_tracks_amb) {
		if (time == amb.start) {
			return true;
		}
	}
	return false;
}

float ExportAudio::TransTime(const std::string& time)
{
	std::vector<std::string> tokens;
	ee::StringHelper::Split(time, ":", tokens);
	float hour, minute, second0, second1;
	ee::StringHelper::FromString(tokens[0], hour);
	ee::StringHelper::FromString(tokens[1], minute);
	ee::StringHelper::FromString(tokens[2], second0);
	ee::StringHelper::FromString(tokens[3], second1);
	return hour * 3600 + minute * 60 + second0 + second1 / 60.0f;
}

}