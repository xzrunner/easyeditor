#include "FileSaver.h"

namespace anim
{

void FileSaver::store(const std::string& filepath, const Symbol& symbol)
{
	Json::Value value;

	value["name"] = symbol.name;

	value["fps"] = symbol.m_fps;

	std::string dir = d2d::FilenameTools::getFileDir(filepath);
	for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
		store(value["layer"][i], symbol.m_layers[i], dir);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath.c_str());
	writer.write(fout, value);
	fout.close();
}

void FileSaver::store(Json::Value& value, Symbol::Layer* layer, const wxString& dir)
{
	value["name"] = layer->name;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
		store(value["frame"][i], layer->frames[i], dir);
}

void FileSaver::store(Json::Value& value, Symbol::Frame* frame, const wxString& dir)
{
	value["time"] = frame->index;
	for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
		store(value["actor"][i], frame->sprites[i], dir);
}

void FileSaver::store(Json::Value& value, d2d::ISprite* sprite, const wxString& dir)
{
	value["filepath"] = d2d::FilenameTools::getRelativePath(dir, 
		sprite->getSymbol().getFilepath()).ToStdString();
	sprite->store(value);
}

} // anim