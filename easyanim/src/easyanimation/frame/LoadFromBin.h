#pragma once

#include <s2s/AnimSym.h>

#include <string>
#include <memory>

namespace eanim
{

class KeyFrame;

class LoadFromBin
{
public:
	static void Load(const std::string& filepath);

private:
	static void LoadFrame(const s2s::AnimSym::Frame& src, KeyFrame* dst, const std::string& dir);

	static void LoadLerp(const s2s::AnimSym::Lerp& src, KeyFrame* dst_frame);

}; // LoadFromBin

}