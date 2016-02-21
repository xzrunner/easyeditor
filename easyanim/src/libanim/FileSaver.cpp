#include "FileSaver.h"

#include <ee/FileHelper.h>
#include <ee/Sprite.h>

#include <fstream>

namespace eanim
{

void FileSaver::Store(const std::string& filepath, const Symbol& symbol)
{
	Json::Value value;

	value["name"] = symbol.name;

	value["fps"] = symbol.getFPS();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
		Store(value["layer"][i], symbol.m_layers[i], dir);

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void FileSaver::Store(Json::Value& value, Symbol::Layer* layer, const std::string& dir)
{
	value["name"] = layer->name;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
		Store(value["frame"][i], layer->frames[i], dir);
}

void FileSaver::Store(Json::Value& value, Symbol::Frame* frame, const std::string& dir)
{
	value["time"] = frame->index;
	for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
		Store(value["actor"][i], frame->sprites[i], dir);
}

void FileSaver::Store(Json::Value& value, ee::Sprite* sprite, const std::string& dir)
{
	const ee::Symbol& symbol = sprite->GetSymbol();

	// filepath
	value["filepath"] = ee::FileHelper::GetRelativePath(dir, symbol.GetFilepath());
	// filepaths
	const std::set<std::string>& filepaths = symbol.GetFilepaths();
	std::set<std::string>::const_iterator itr = filepaths.begin();
	for (int i = 0; itr != filepaths.end(); ++itr, ++i) {
		value["filepaths"][i] = *itr;
	}
	// other
	sprite->Store(value);
}

} // anim