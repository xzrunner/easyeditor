#pragma once

#include <drag2d.h>

#include "dataset/LayersMgr.h"
#include "dataset/ResourceMgr.h"

namespace eanim
{

class KeyFrame;

class Context
{
public:
	wxWindow* library;
	wxWindow* property;
	wxWindow* stage;
	wxWindow* toolbar;
	wxWindow* layersPanel;
	wxWindow* keysPanel;
	d2d::ViewlistPanel* viewlist;

	d2d::ILibraryPage* imagePage;

	LayersMgr layers;
	int maxFrame;

	unsigned int fps;

	wxString packer;

	ResourceMgr resource;

	std::string name;

public:
	void setCurrFrame(int layer, int frame);
	KeyFrame* getCurrFrame() { return m_last_keyframe; }

	// layer start from 0
	// frame start from 1
	int layer() const { return m_curr_layer; }
	int frame() const { return m_curr_frame; }

public:
	static Context* Instance();

private:
	Context();

	void reloadViewList(const KeyFrame& frame);

private:
	static Context* m_instance;

	KeyFrame* m_last_keyframe;

	// layer start from 0
	// frame start from 1
	int m_curr_layer;
	int m_curr_frame;

}; // Context

}

