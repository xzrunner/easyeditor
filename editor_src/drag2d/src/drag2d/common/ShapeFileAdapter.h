#pragma once

#include "IFileAdapter.h"

#include <wx/colourdata.h>
#include <vector>

namespace d2d
{
	class PolygonShape;

	class ShapeFileAdapter : public IFileAdapter
	{
	public:
		~ShapeFileAdapter();

		virtual void load(const char* filename);

	public:
		struct Layer
		{
			std::vector<PolygonShape*> polys;
			std::string name;

			~Layer();

		}; // Layer

	public:
		wxColourData m_color;

		std::vector<Layer*> m_layers;

	}; // ShapeFileAdapter
}

