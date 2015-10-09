#ifndef _EASYTEXPACKER_FILE_ADAPTER_H_
#define _EASYTEXPACKER_FILE_ADAPTER_H_

#include "ToolbarPanel.h"

#include <drag2d.h>

namespace etp
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

#endif // _EASYTEXPACKER_FILE_ADAPTER_H_