#include "AnimFileAdapter.h"

#include "common/FileNameTools.h"

#include <fstream>

namespace d2d
{

void AnimFileAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	fps = value["fps"].asInt();

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = loadLayer(layerValue, FilenameTools::getFileDir(filename));
		layers.push_back(layer);
		layerValue = value["layer"][i++];
	}
}

AnimFileAdapter::Layer* AnimFileAdapter::loadLayer(const Json::Value& layerValue, 
												   const wxString& dlg)
{
	Layer* layer = new Layer;

	layer->name = layerValue["name"].asString();

	int i = 0;
	Json::Value frameValue = layerValue["frame"][i++];
	while (!frameValue.isNull()) {
		Frame* frame = loadFrame(frameValue, dlg);
		layer->frames.push_back(frame);
		frameValue = layerValue["frame"][i++];
	}

	return layer;
}

AnimFileAdapter::Frame* AnimFileAdapter::loadFrame(const Json::Value& frameValue, 
												   const wxString& dlg)
{
	int time = frameValue["time"].asInt();

	Frame* frame = new Frame;
	frame->id = frameValue["id"].asInt();
	frame->index = time;
	frame->bClassicTween = frameValue["tween"].asBool();

	int i = 0;
	Json::Value entryValue = frameValue["actor"][i++];
	while (!entryValue.isNull()) {
		Entry* entry = loadEntry(entryValue, dlg);
		frame->entries.push_back(entry);
		entryValue = frameValue["actor"][i++];
	}

	return frame;
}

AnimFileAdapter::Entry* AnimFileAdapter::loadEntry(const Json::Value& entryValue, 
												   const wxString& dlg)
{
	Entry* entry = new Entry;

	entry->filepath = entryValue["filepath"].asString();

	entry->name = entryValue["name"].asString();
	entry->addColor = entryValue["add color"].asString();
	entry->multiColor = entryValue["multi color"].asString();

	entry->pos.x = entryValue["position"]["x"].asDouble();
	entry->pos.y = entryValue["position"]["y"].asDouble();

	entry->angle = entryValue["angle"].asDouble();

	entry->xScale = entryValue["x scale"].asDouble();
	entry->yScale = entryValue["y scale"].asDouble();

	entry->xOffset = entryValue["x offset"].asDouble();
	entry->yOffset = entryValue["y offset"].asDouble();

	if (!entryValue["x shear"].isNull())
	{
		entry->xShear = entryValue["x shear"].asDouble();
		entry->yShear = entryValue["y shear"].asDouble();
	}
	else
	{
		entry->xShear = entry->yShear = 0.0f;
	}

	entry->xMirror = entryValue["x mirror"].asBool();
	entry->yMirror = entryValue["y mirror"].asBool();

	return entry;
}

} // d2d