#include "FileSaver.h"
#include "Symbol.h"

#include <ee/FileHelper.h>
#include <ee/Sprite.h>

#include <sprite2/S2_Sprite.h>

#include <fstream>

namespace eanim
{

void FileSaver::Store(const std::string& filepath, const Symbol& sym)
{
	Json::Value value;

	value["name"] = sym.name;

	value["fps"] = sym.getFPS();

	const std::vector<s2::AnimSymbol::Layer*>& layers = sym.GetLayers();
	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (size_t i = 0, n = layers.size(); i < n; ++i)
		Store(value["layer"][i], layers[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileSaver::Store(Json::Value& value, s2::AnimSymbol::Layer* layer, const std::string& dir)
{
	value["name"] = layer->name;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
		Store(value["frame"][i], layer->frames[i], dir);
}

void FileSaver::Store(Json::Value& value, s2::AnimSymbol::Frame* frame, const std::string& dir)
{
	value["time"] = frame->index;
	for (size_t i = 0, n = frame->sprs.size(); i < n; ++i)
		Store(value["actor"][i], frame->sprs[i], dir);
}

void FileSaver::Store(Json::Value& value, s2::Sprite* spr, const std::string& dir)
{
	ee::Sprite* ee_spr = dynamic_cast<ee::Sprite*>(spr);
	const ee::Symbol* ee_sym = dynamic_cast<const ee::Symbol*>(ee_spr->GetSymbol());

	// filepath
	value["filepath"] = ee::FileHelper::GetRelativePath(dir, ee_sym->GetFilepath());
	// filepaths
	const std::set<std::string>& filepaths = ee_sym->GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	ee_spr->Store(value, dir);
}

} // anim