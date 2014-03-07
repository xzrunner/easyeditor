#pragma once

#include "IFileAdapter.h"

#include "common/Vector.h"

#include <json/json.h>

class wxString;

namespace d2d
{
	class AnimFileAdapter
	{
	public:
		struct Entry
		{
			std::string filepath;

			std::string name;
			std::string multiColor, addColor;

			Vector pos;
			float angle;
			float xScale, yScale;
			float xShear, yShear;
			float xOffset, yOffset;
			bool xMirror, yMirror;
		};

		struct Frame
		{
			int id;

			int index;

			std::vector<Entry*> entries;

			bool bClassicTween;
		};

		struct Layer
		{
			std::string name;

			std::vector<Frame*> frames;
		};

	public:
		virtual void load(const char* filename);

	private:
		Layer* loadLayer(const Json::Value& layerValue, const wxString& dlg);
		Frame* loadFrame(const Json::Value& frameValue, const wxString& dlg);
		Entry* loadEntry(const Json::Value& entryValue, const wxString& dlg);

	public:
		std::string name;

		int fps;

		std::vector<Layer*> layers;

	}; // AnimFileAdapter
}

