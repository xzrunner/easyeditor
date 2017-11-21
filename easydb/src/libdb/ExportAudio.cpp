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

		std::string start, end, duration, timestamp;
		ss >> start >> end >> duration >> timestamp >> skip;
		track.start = TransTime(start);
		track.end = TransTime(end);
		track.duration = TransTime(duration);
		track.timestamp = TransTime(timestamp);

		m_tracks.insert(std::make_pair(track.start, track));
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
		std::string location;
		ss >> skip >> location >> skip >> skip >> marker.name;
		marker.location = TransTime(location);
		m_markers.push_back(marker);
	}
}

void ExportAudio::OutputAudio(const std::string& dst_dir)
{
	for (int i = 0, n = m_markers.size(); i < n; ++i)
	{
		// region
		float start_time = m_markers[i].location;
		float end_time = 0;
		if (i < n - 1) {
			end_time = m_markers[i + 1].location;
		}

		// create sym
		libanim::Symbol sym;
		auto layer = CU_MAKE_UNIQUE<s2::AnimSymbol::Layer>();
		CreateAllAudioSpr(*layer, start_time, end_time);
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

void ExportAudio::CreateAllAudioSpr(s2::AnimSymbol::Layer& layer, float start_time, float end_time)
{
	auto itr_begin = m_tracks.find(start_time);
	auto itr_end = m_tracks.find(end_time);
	if (end_time == 0) {
		itr_end = m_tracks.end();
	}
	for (std::map<float, Track>::iterator itr_curr = itr_begin; itr_curr != itr_end; )
	{
		float time = itr_curr->second.start - start_time;

		auto itr_next = itr_curr;
		++itr_next;

		std::shared_ptr<eaudio::Sprite> spr;
		if (itr_curr != itr_end && itr_next != m_tracks.end() && itr_next->second.name == FADE_OUT_STR) {
			spr = CreateAudioSpr(&itr_curr->second, &itr_next->second);
			++itr_curr;
			++itr_curr;
		} else {
			spr = CreateAudioSpr(&itr_curr->second, nullptr);
			++itr_curr;
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
		audio_spr->SetAudioDuration(track->duration + track_fadeout->duration);
		audio_spr->SetFadeOut(track_fadeout->duration);
	} else {
		audio_spr->SetAudioDuration(track->duration);
	}

//	audio_spr->SetAudioOffset(TransTime(track->timestamp));

	return audio_spr;
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