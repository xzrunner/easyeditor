#include "FileSaver.h"

namespace libanim
{

void FileSaver::store(const std::string& filepath, const d2d::AnimSymbol& symbol)
{
	Json::Value value;

	value["fps"] = symbol.m_fps;

	std::string dlg = d2d::FilenameTools::getFileDir(filepath);
	for (size_t i = 0, n = symbol.m_layers.size(); i < n; ++i)
		value["layer"][i] = store(symbol.m_layers[i], dlg);

	Json::StyledStreamWriter writer;
	std::ofstream fout(filepath.c_str());
	writer.write(fout, value);
	fout.close();
}

Json::Value FileSaver::store(d2d::AnimSymbol::Layer* layer, const wxString& dlg)
{
	Json::Value value;

	value["name"] = layer->name;
	for (size_t i = 0, n = layer->frames.size(); i < n; ++i)
		value["frame"][i] = store(layer->frames[i], dlg);

	return value;
}

Json::Value FileSaver::store(d2d::AnimSymbol::Frame* frame, const wxString& dlg)
{
	Json::Value value;

	value["time"] = frame->index;
	for (size_t i = 0, n = frame->sprites.size(); i < n; ++i)
		value["actor"][i] = store(frame->sprites[i], dlg);

	return value;
}

Json::Value FileSaver::store(d2d::ISprite* sprite, const wxString& dlg)
{
	Json::Value value;

	value["filepath"] = d2d::FilenameTools::getRelativePath(dlg, 
		sprite->getSymbol().getFilepath()).ToStdString();

	value["position"]["x"] = sprite->getPosition().x;
	value["position"]["y"] = sprite->getPosition().y;

	value["angle"] = sprite->getAngle();

	value["scale"] = sprite->getScale().x;

	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
	value["x mirror"] = xMirror;
	value["y mirror"] = yMirror;

	return value;
}
} // libanim