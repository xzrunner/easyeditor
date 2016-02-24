#ifndef _EASYTEXPACKER_FILE_ADAPTER_H_
#define _EASYTEXPACKER_FILE_ADAPTER_H_

#include "ToolbarPanel.h"

namespace etexpacker
{

class FileAdapter
{
public:
	FileAdapter();
	~FileAdapter();

	void Resolve(const std::string& filepath);

public:
	struct Item
	{
		std::string filepath;
		int left, top;
		int width, height;
	};

public:
	int m_width, m_height;
	std::vector<Item> m_data;

}; // FileAdapter

}

#endif // _EASYTEXPACKER_FILE_ADAPTER_H_