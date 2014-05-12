#ifndef EPACKER_FILE_ADAPTER_H
#define EPACKER_FILE_ADAPTER_H

#include "ToolbarPanel.h"

#include <drag2d.h>

namespace epacker
{
	class FileAdapter
	{
	public:
		FileAdapter();
		~FileAdapter();

		void resolve(const wxString& filepath);

	public:
		struct Item
		{
			wxString filepath;
			int left, top;
			int width, height;
		};

	public:
		int m_width, m_height;
		std::vector<Item> m_data;

	}; // FileAdapter
}

#endif // EPACKER_FILE_ADAPTER_H