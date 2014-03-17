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
		value["layer"][i] = store(symbol.m_layers[i], dir);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath.c_str());
	writer.write(fout, value);
	fout.close();
}

Json::Value FileSaver::store(Symbol::Layer* layer, const wxString& dir)
{
	Json::Value value;

	value["name"] = layer->name;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
		value["frame"][i] = store(layer->frames[i], dir);

	return value;
}

Json::Value FileSaver::store(Symbol::Frame* frame, const wxString& dir)
{
	Json::Value value;

	value["time"] = frame->index;
	for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
		value["actor"][i] = store(frame->sprites[i], dir);

	return value;
}

Json::Value FileSaver::store(d2d::ISprite* sprite, const wxString& dir)
{
	Json::Value value;

	value["filepath"] = d2d::FilenameTools::getRelativePath(dir, 
		sprite->getSymbol().getFilepath()).ToStdString();
	sprite->store(value);

	return value;
}

} // anim