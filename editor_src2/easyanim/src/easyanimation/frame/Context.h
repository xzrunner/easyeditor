#pragma once

#include <drag2d.h>

#include "dataset/LayersMgr.h"
#include "dataset/ResourceMgr.h"

namespace eanim
{
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
		int currLayer;
		int currFrame;
		int maxFrame;

		unsigned int fps;

		wxString packer;

		ResourceMgr resource;

		std::string name;

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

